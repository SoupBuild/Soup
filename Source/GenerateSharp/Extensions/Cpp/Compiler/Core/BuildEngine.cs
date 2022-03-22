// <copyright file="BuildEngine.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;

namespace Soup.Build.Cpp.Compiler
{
	/// <summary>
	/// The build engine
	/// </summary>
	public class BuildEngine
	{
		public BuildEngine(ICompiler compiler)
		{
			_compiler = compiler;
		}

		/// <summary>
		/// Generate the required build operations for the requested build
		/// </summary>
		public BuildResult Execute(
			IBuildState buildState,
			BuildArguments arguments)
		{
			var result = new BuildResult();

			// All dependencies must include the entire interface dependency closure
			result.ModuleDependencies = new List<Path>(arguments.ModuleDependencies);

			// Ensure the output directories exists as the first step
			result.BuildOperations.Add(
				SharedOperations.CreateCreateDirectoryOperation(
					arguments.TargetRootDirectory,
					arguments.ObjectDirectory));
			result.BuildOperations.Add(
				SharedOperations.CreateCreateDirectoryOperation(
					arguments.TargetRootDirectory,
					arguments.BinaryDirectory));

			// Perform the core compilation of the source files
			CoreCompile(buildState, arguments, result);

			// Link the final target after all of the compile graph is done
			CoreLink(buildState, arguments, result);

			// Copy previous runtime dependencies after linking has completed
			CopyRuntimeDependencies(arguments, result);

			return result;
		}

		/// <summary>
		/// Compile the module and source files
		/// </summary>
		private void CoreCompile(
			IBuildState buildState,
			BuildArguments arguments,
			BuildResult result)
		{
			// Ensure there are actually files to build
			if (arguments.ModuleInterfacePartitionSourceFiles.Count != 0 ||
				!arguments.ModuleInterfaceSourceFile.IsEmpty ||
				arguments.SourceFiles.Count != 0)
			{
				// Setup the shared properties
				var compileArguments = new SharedCompileArguments()
				{
					Standard = arguments.LanguageStandard,
					Optimize = Convert(arguments.OptimizationLevel),
					SourceRootDirectory = arguments.SourceRootDirectory,
					TargetRootDirectory = arguments.TargetRootDirectory,
					ObjectDirectory = arguments.ObjectDirectory,
					IncludeDirectories = arguments.IncludeDirectories,
					IncludeModules = arguments.ModuleDependencies,
					PreprocessorDefinitions = arguments.PreprocessorDefinitions,
					GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo,
					EnableWarningsAsErrors = arguments.EnableWarningsAsErrors,
					DisabledWarnings = arguments.DisabledWarnings,
					EnabledWarnings = arguments.EnabledWarnings,
					CustomProperties = arguments.CustomProperties,
				};

				// Compile the resource file if present
				if (!arguments.ResourceFile.IsEmpty)
				{
					buildState.LogTrace(TraceLevel.Information, "Generate Resource File Compile: " + arguments.ResourceFile.ToString());

					var compiledResourceFile =
						arguments.ObjectDirectory +
						new Path(arguments.ResourceFile.GetFileName());
					compiledResourceFile.SetFileExtension(_compiler.ResourceFileExtension);

					var compileResourceFileArguments = new ResourceCompileArguments()
					{
						SourceFile = arguments.ResourceFile,
						TargetFile = compiledResourceFile,
					};

					// Add the resource file arguments to the shared build definition
					compileArguments.ResourceFile = compileResourceFileArguments;
				}

				// Build up the entire Interface Dependency Closure for each file
				var partitionInterfaceDependencyLookup = new Dictionary<Path, IReadOnlyList<Path>>();
				foreach (var file in arguments.ModuleInterfacePartitionSourceFiles)
				{
					partitionInterfaceDependencyLookup.Add(file.File, file.Imports);
				}

				// Compile the individual module interface partition translation units
				var compileInterfacePartitionUnits = new List<InterfaceUnitCompileArguments>();
				var allPartitionInterfaces = new List<Path>();
				foreach (var file in arguments.ModuleInterfacePartitionSourceFiles)
				{
					buildState.LogTrace(TraceLevel.Information, "Generate Module Interface Partition Compile Operation: " + file.File.ToString());

					var objectModuleInterfaceFile =
						arguments.ObjectDirectory +
						new Path(file.File.GetFileName());
					objectModuleInterfaceFile.SetFileExtension(_compiler.ModuleFileExtension);

					var interfaceDependencyClosure = new HashSet<Path>();
					BuildClosure(interfaceDependencyClosure, file.File, partitionInterfaceDependencyLookup);
					if (interfaceDependencyClosure.Contains(file.File))
					{
						throw new InvalidOperationException($"Circular partition references in: {file.File}");
					}

					var partitionImports = new List<Path>();
					foreach (var import in interfaceDependencyClosure)
					{
						var importInterface = arguments.ObjectDirectory + new Path(import.GetFileName());
						importInterface.SetFileExtension(_compiler.ModuleFileExtension);
						partitionImports.Add(arguments.TargetRootDirectory + importInterface);
					}

					var compileFileArguments = new InterfaceUnitCompileArguments()
					{
						SourceFile = file.File,
						TargetFile = arguments.ObjectDirectory + new Path(file.File.GetFileName()),
						IncludeModules = partitionImports,
						ModuleInterfaceTarget = objectModuleInterfaceFile,
					};

					compileFileArguments.TargetFile.SetFileExtension(_compiler.ObjectFileExtension);

					compileInterfacePartitionUnits.Add(compileFileArguments);
					allPartitionInterfaces.Add(arguments.TargetRootDirectory + objectModuleInterfaceFile);
				}

				// Add all partition unit interface files as module dependencies since MSVC does not
				// combine the interfaces into the final interface unit
				foreach (var module in allPartitionInterfaces)
				{
					result.ModuleDependencies.Add(module);
				}

				compileArguments.InterfacePartitionUnits = compileInterfacePartitionUnits;

				// Compile the module interface unit if present
				if (!arguments.ModuleInterfaceSourceFile.IsEmpty)
				{
					buildState.LogTrace(TraceLevel.Information, "Generate Module Interface Unit Compile: " + arguments.ModuleInterfaceSourceFile.ToString());

					var objectModuleInterfaceFile =
						arguments.ObjectDirectory +
						new Path(arguments.ModuleInterfaceSourceFile.GetFileName());
					objectModuleInterfaceFile.SetFileExtension(_compiler.ModuleFileExtension);
					var binaryOutputModuleInterfaceFile =
						arguments.BinaryDirectory +
						new Path(arguments.TargetName + "." + _compiler.ModuleFileExtension);

					var compileModuleFileArguments = new InterfaceUnitCompileArguments()
					{
						SourceFile = arguments.ModuleInterfaceSourceFile,
						TargetFile = arguments.ObjectDirectory + new Path(arguments.ModuleInterfaceSourceFile.GetFileName()),
						IncludeModules = allPartitionInterfaces,
						ModuleInterfaceTarget = objectModuleInterfaceFile,
					};

					compileModuleFileArguments.TargetFile.SetFileExtension(_compiler.ObjectFileExtension);

					// Add the interface unit arguments to the shared build definition
					compileArguments.InterfaceUnit = compileModuleFileArguments;

					// Copy the binary module interface to the binary directory after compiling
					var copyInterfaceOperation =
						SharedOperations.CreateCopyFileOperation(
							arguments.TargetRootDirectory,
							objectModuleInterfaceFile,
							binaryOutputModuleInterfaceFile);
					result.BuildOperations.Add(copyInterfaceOperation);

					// Add output module interface to the parent set of modules
					// This will allow the module implementation units access as well as downstream
					// dependencies to the public interface.
					result.ModuleDependencies.Add(
						binaryOutputModuleInterfaceFile.HasRoot ?
							binaryOutputModuleInterfaceFile :
							arguments.TargetRootDirectory + binaryOutputModuleInterfaceFile);
				}

				// Compile the individual translation units
				var compileImplementationUnits = new List<TranslationUnitCompileArguments>(compileArguments.ImplementationUnits);
				foreach (var file in arguments.SourceFiles)
				{
					buildState.LogTrace(TraceLevel.Information, "Generate Compile Operation: " + file.ToString());

					var compileFileArguments = new TranslationUnitCompileArguments();
					compileFileArguments.SourceFile = file;
					compileFileArguments.TargetFile = arguments.ObjectDirectory + new Path(file.GetFileName());
					compileFileArguments.TargetFile.SetFileExtension(_compiler.ObjectFileExtension);

					compileImplementationUnits.Add(compileFileArguments);
				}

				compileArguments.ImplementationUnits = compileImplementationUnits;

				// Compile all source files as a single call
				var compileOperations = _compiler.CreateCompileOperations(compileArguments);
				foreach (var operation in compileOperations)
					result.BuildOperations.Add(operation);
			}
		}

		/// <summary>
		/// Link the library
		/// </summary>
		private void CoreLink(
			IBuildState buildState,
			BuildArguments arguments,
			BuildResult result)
		{
			buildState.LogTrace(TraceLevel.Information, "CoreLink");

			Path targetFile;
			var implementationFile = new Path();
			switch (arguments.TargetType)
			{
				case BuildTargetType.StaticLibrary:
					targetFile = 
						arguments.BinaryDirectory + 
						new Path(arguments.TargetName + "." + _compiler.StaticLibraryFileExtension);
					break;
				case BuildTargetType.DynamicLibrary:
					targetFile = 
						arguments.BinaryDirectory +
						new Path(arguments.TargetName + "." + _compiler.DynamicLibraryFileExtension);
					implementationFile = 
						arguments.BinaryDirectory +
						new Path(arguments.TargetName + "." + _compiler.StaticLibraryFileExtension);
					break;
				case BuildTargetType.Executable:
				case BuildTargetType.WindowsApplication:
					targetFile = 
						arguments.BinaryDirectory + 
						new Path(arguments.TargetName + ".exe");
					break;
				default:
					throw new InvalidOperationException("Unknown build target type.");
			}

			buildState.LogTrace(TraceLevel.Information, "Linking target");

			var linkArguments = new LinkArguments()
			{
				TargetFile = targetFile,
				TargetArchitecture = arguments.TargetArchitecture,
				ImplementationFile = implementationFile,
				TargetRootDirectory = arguments.TargetRootDirectory,
				LibraryPaths = arguments.LibraryPaths,
				GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo,
			};

			// Only resolve link libraries if not a library ourself
			if (arguments.TargetType != BuildTargetType.StaticLibrary)
			{
				linkArguments.ExternalLibraryFiles = arguments.PlatformLinkDependencies;
				linkArguments.LibraryFiles = arguments.LinkDependencies;
			}

			// Translate the target type into the link target
			// and determine what dependencies to inject into downstream builds

			switch (arguments.TargetType)
			{
				case BuildTargetType.StaticLibrary:
				{
					linkArguments.TargetType = LinkTarget.StaticLibrary;
					
					// Add the library as a link dependency and all recursive libraries
					result.LinkDependencies = new List<Path>(arguments.LinkDependencies);
					var absoluteTargetFile = linkArguments.TargetFile.HasRoot ? linkArguments.TargetFile : linkArguments.TargetRootDirectory + linkArguments.TargetFile;
					result.LinkDependencies.Add(absoluteTargetFile);
					break;
				}
				case BuildTargetType.DynamicLibrary:
				{
					linkArguments.TargetType = LinkTarget.DynamicLibrary;

					// Add the DLL as a runtime dependency
					var absoluteTargetFile = linkArguments.TargetFile.HasRoot ? linkArguments.TargetFile : linkArguments.TargetRootDirectory + linkArguments.TargetFile;
					result.RuntimeDependencies.Add(absoluteTargetFile);

					// Clear out all previous link dependencies and replace with the 
					// single implementation library for the DLL
					var absoluteImplementationFile = linkArguments.ImplementationFile.HasRoot ? linkArguments.ImplementationFile : linkArguments.TargetRootDirectory + linkArguments.ImplementationFile;
					result.LinkDependencies.Add(absoluteImplementationFile);

					// Set the targe file
					result.TargetFile = absoluteTargetFile;

					break;
				}
				case BuildTargetType.Executable:
				{
					linkArguments.TargetType = LinkTarget.Executable;

					// Add the Executable as a runtime dependency
					var absoluteTargetFile = linkArguments.TargetFile.HasRoot ? linkArguments.TargetFile : linkArguments.TargetRootDirectory + linkArguments.TargetFile;
					result.RuntimeDependencies.Add(absoluteTargetFile);

					// All link dependencies stop here.

					// Set the targe file
					result.TargetFile = absoluteTargetFile;
					break;
				}
				case BuildTargetType.WindowsApplication:
				{
					linkArguments.TargetType = LinkTarget.WindowsApplication;

					// Add the Executable as a runtime dependency
					var absoluteTargetFile = linkArguments.TargetFile.HasRoot ? linkArguments.TargetFile : linkArguments.TargetRootDirectory + linkArguments.TargetFile;
					result.RuntimeDependencies.Add(absoluteTargetFile);

					// All link dependencies stop here.

					// Set the targe file
					result.TargetFile = absoluteTargetFile;
					break;
				}
				default:
				{
					throw new InvalidOperationException("Unknown build target type.");
				}
			}

			// Build up the set of object files
			var objectFiles = new List<Path>();

			// Add the resource file if present
			if (!arguments.ResourceFile.IsEmpty)
			{
				var compiledResourceFile =
					arguments.ObjectDirectory +
					new Path(arguments.ResourceFile.GetFileName());
				compiledResourceFile.SetFileExtension(_compiler.ResourceFileExtension);

				objectFiles.Add(compiledResourceFile);
			}

			// Add the partition object files
			foreach (var sourceFile in arguments.ModuleInterfacePartitionSourceFiles)
			{
				var objectFile = arguments.ObjectDirectory + new Path(sourceFile.File.GetFileName());
				objectFile.SetFileExtension(_compiler.ObjectFileExtension);
				objectFiles.Add(objectFile);
			}

			// Add the module interface object file if present
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty)
			{
				var objectFile = arguments.ObjectDirectory + new Path(arguments.ModuleInterfaceSourceFile.GetFileName());
				objectFile.SetFileExtension(_compiler.ObjectFileExtension);
				objectFiles.Add(objectFile);
			}

			// Add the implementation unit object files
			foreach (var sourceFile in arguments.SourceFiles)
			{
				var objectFile = arguments.ObjectDirectory + new Path(sourceFile.GetFileName());
				objectFile.SetFileExtension(_compiler.ObjectFileExtension);
				objectFiles.Add(objectFile);
			}

			linkArguments.ObjectFiles = objectFiles;

			// Perform the link
			buildState.LogTrace(TraceLevel.Information, "Generate Link Operation: " + linkArguments.TargetFile.ToString());
			var linkOperation = _compiler.CreateLinkOperation(linkArguments);
			result.BuildOperations.Add(linkOperation);

			// Pass along the link arguments for internal access
			result.InternalLinkDependencies = new List<Path>(arguments.LinkDependencies);
			foreach (var file in linkArguments.ObjectFiles)
				result.InternalLinkDependencies.Add(file);
		}

		/// <summary>
		/// Copy runtime dependencies
		/// </summary>
		private void CopyRuntimeDependencies(
			BuildArguments arguments,
			BuildResult result)
		{
			if (arguments.TargetType == BuildTargetType.Executable ||
				arguments.TargetType == BuildTargetType.WindowsApplication ||
				arguments.TargetType == BuildTargetType.DynamicLibrary)
			{
				foreach (var source in arguments.RuntimeDependencies)
				{
					var target = arguments.BinaryDirectory + new Path(source.GetFileName());
					var operation = SharedOperations.CreateCopyFileOperation(
						arguments.TargetRootDirectory,
						source,
						target);
					result.BuildOperations.Add(operation);

					// Add the copied file as the new runtime dependency
					result.RuntimeDependencies.Add(target);
				}
			}
			else
			{
				// Pass along all runtime dependencies in their original location
				foreach (var source in arguments.RuntimeDependencies)
				{
					result.RuntimeDependencies.Add(source);
				}
			}
		}

		private void BuildClosure(
			ISet<Path> closure,
			Path file,
			IDictionary<Path, IReadOnlyList<Path>> partitionInterfaceDependencyLookup)
		{
			foreach (var childFile in partitionInterfaceDependencyLookup[file])
			{
				closure.Add(childFile);
				BuildClosure(closure, childFile, partitionInterfaceDependencyLookup);
			}
		}

		private OptimizationLevel Convert(BuildOptimizationLevel value)
		{
			switch (value)
			{
				case BuildOptimizationLevel.None:
					return OptimizationLevel.None;
				case BuildOptimizationLevel.Speed:
					return OptimizationLevel.Speed;
				case BuildOptimizationLevel.Size:
					return OptimizationLevel.Size;
				default:
					throw new InvalidOperationException("Unknown BuildOptimizationLevel.");
			}
		}

		private ICompiler _compiler;
	}
}
