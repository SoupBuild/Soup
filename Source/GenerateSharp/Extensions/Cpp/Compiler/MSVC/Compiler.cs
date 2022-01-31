// <copyright file="Compiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Cpp.Compiler.MSVC
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	public class Compiler : ICompiler
	{
		private Path _compilerExecutable;
		private Path _linkerExecutable;
		private Path _libraryExecutable;
		private Path _rcExecutable;

		public Compiler(Path compilerExecutable, Path linkerExecutable, Path libraryExecutable, Path rcExecutable)
		{
			_compilerExecutable = compilerExecutable;
			_linkerExecutable = linkerExecutable;
			_libraryExecutable = libraryExecutable;
			_rcExecutable = rcExecutable;
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		public string Name => "MSVC";

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		public string ObjectFileExtension => "obj";

		/// <summary>
		/// Gets the module file extension for the compiler
		/// </summary>
		public string ModuleFileExtension => "ifc";

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		public string StaticLibraryFileExtension => "lib";

		/// <summary>
		/// Gets the dynamic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		public string DynamicLibraryFileExtension => "dll";

		/// <summary>
		/// Gets the resource file extension for the compiler
		/// </summary>
		public string ResourceFileExtension => "res";

		/// <summary>
		/// Compile
		/// </summary>
		public IList<BuildOperation> CreateCompileOperations(
			SharedCompileArguments arguments)
		{
			var operations = new List<BuildOperation>();

			// Write the shared arguments to the response file
			var responseFile = arguments.ObjectDirectory + new Path("SharedCompileArguments.rsp");
			var sharedCommandArguments = ArgumentBuilder.BuildSharedCompilerArguments(arguments);
			var writeSharedArgumentsOperation = SharedOperations.CreateWriteFileOperation(
				arguments.TargetRootDirectory,
				responseFile,
				CombineArguments(sharedCommandArguments));
			operations.Add(writeSharedArgumentsOperation);

			// Initialize a shared input set
			var sharedInputFiles = new List<Path>();
			sharedInputFiles.AddRange(arguments.IncludeModules);

			var absoluteResponseFile = arguments.TargetRootDirectory + responseFile;

			// Generate the resource build operation if present
			if (arguments.ResourceFile is not null)
			{
				var resourceFileArguments = arguments.ResourceFile;

				// Build up the input/output sets
				var inputFiles = sharedInputFiles.ToList();
				inputFiles.Add(resourceFileArguments.SourceFile);
				// TODO: The temp files require read access, need a way to tell build operation
				inputFiles.Add(arguments.TargetRootDirectory + new Path("fake_file"));
				var outputFiles = new List<Path>()
				{
					arguments.TargetRootDirectory + resourceFileArguments.TargetFile,
				};

				// Build the unique arguments for this resource file
				var commandArguments = ArgumentBuilder.BuildResourceCompilerArguments(
					arguments.TargetRootDirectory,
					arguments);

				// Generate the operation
				var buildOperation = new BuildOperation(
					resourceFileArguments.SourceFile.ToString(),
					arguments.SourceRootDirectory,
					_rcExecutable,
					CombineArguments(commandArguments),
					inputFiles,
					outputFiles);
				operations.Add(buildOperation);
			}

			foreach (var partitionUnitArguments in arguments.InterfacePartitionUnits)
			{
				// Build up the input/output sets
				var inputFiles = sharedInputFiles.ToList();
				inputFiles.Add(partitionUnitArguments.SourceFile);
				inputFiles.Add(absoluteResponseFile);
				inputFiles.AddRange(partitionUnitArguments.IncludeModules);

				var outputFiles = new List<Path>()
				{
					arguments.TargetRootDirectory + partitionUnitArguments.TargetFile,
					arguments.TargetRootDirectory + partitionUnitArguments.ModuleInterfaceTarget,
				};

				// Build the unique arguments for this translation unit
				var commandArguments = ArgumentBuilder.BuildInterfaceUnitCompilerArguments(
					arguments.TargetRootDirectory,
					partitionUnitArguments,
					absoluteResponseFile);

				// Generate the operation
				var buildOperation = new BuildOperation(
					partitionUnitArguments.SourceFile.ToString(),
					arguments.SourceRootDirectory,
					_compilerExecutable,
					CombineArguments(commandArguments),
					inputFiles.ToArray(),
					outputFiles);
				operations.Add(buildOperation);
			}

			// Generate the interface build operation if present
			var internalModules = new List<Path>();
			if (arguments.InterfaceUnit is not null)
			{
				var interfaceUnitArguments = arguments.InterfaceUnit;

				// Build up the input/output sets
				var inputFiles = sharedInputFiles.ToList();
				inputFiles.Add(interfaceUnitArguments.SourceFile);
				inputFiles.Add(absoluteResponseFile);
				inputFiles.AddRange(interfaceUnitArguments.IncludeModules);

				var outputFiles = new List<Path>()
				{
					arguments.TargetRootDirectory + interfaceUnitArguments.TargetFile,
					arguments.TargetRootDirectory + interfaceUnitArguments.ModuleInterfaceTarget,
				};

				// Build the unique arguments for this translation unit
				var commandArguments = ArgumentBuilder.BuildInterfaceUnitCompilerArguments(
					arguments.TargetRootDirectory,
					interfaceUnitArguments,
					absoluteResponseFile);

				// Generate the operation
				var buildOperation = new BuildOperation(
					interfaceUnitArguments.SourceFile.ToString(),
					arguments.SourceRootDirectory,
					_compilerExecutable,
					CombineArguments(commandArguments),
					inputFiles,
					outputFiles);
				operations.Add(buildOperation);

				// Add our module interface back in for the downstream compilers
				internalModules.Add(arguments.TargetRootDirectory + interfaceUnitArguments.ModuleInterfaceTarget);
			}

			foreach (var implementationUnitArguments in arguments.ImplementationUnits)
			{
				// Build up the input/output sets
				var inputFiles = sharedInputFiles.ToList();
				inputFiles.Add(implementationUnitArguments.SourceFile);
				inputFiles.Add(absoluteResponseFile);
				inputFiles.AddRange(implementationUnitArguments.IncludeModules);
				inputFiles.AddRange(internalModules);

				var outputFiles = new List<Path>()
				{
					arguments.TargetRootDirectory + implementationUnitArguments.TargetFile,
				};

				// Build the unique arguments for this translation unit
				var commandArguments = ArgumentBuilder.BuildTranslationUnitCompilerArguments(
					arguments.TargetRootDirectory,
					implementationUnitArguments,
					absoluteResponseFile,
					internalModules);

				// Generate the operation
				var buildOperation = new BuildOperation(
					implementationUnitArguments.SourceFile.ToString(),
					arguments.SourceRootDirectory,
					_compilerExecutable,
					CombineArguments(commandArguments),
					inputFiles.ToArray(),
					outputFiles);
				operations.Add(buildOperation);
			}

			return operations;
		}

		/// <summary>
		/// Link
		/// </summary>
		public BuildOperation CreateLinkOperation(
			LinkArguments arguments)
		{
			// Select the correct executable for linking libraries or executables
			Path executablePath;
			switch (arguments.TargetType)
			{
				case LinkTarget.StaticLibrary:
					executablePath = _libraryExecutable;
					break;
				case LinkTarget.DynamicLibrary:
				case LinkTarget.Executable:
				case LinkTarget.WindowsApplication:
					executablePath = _linkerExecutable;
					break;
				default:
					throw new InvalidOperationException("Unknown LinkTarget.");
			}

			// Build the set of input/output files along with the arguments
			var inputFiles = new List<Path>();
			inputFiles.AddRange(arguments.LibraryFiles);
			inputFiles.AddRange(arguments.ObjectFiles);
			var outputFiles = new List<Path>()
			{
				arguments.TargetRootDirectory + arguments.TargetFile,
			};
			var commandarguments = ArgumentBuilder.BuildLinkerArguments(arguments);

			var buildOperation = new BuildOperation(
				arguments.TargetFile.ToString(),
				arguments.TargetRootDirectory,
				executablePath,
				CombineArguments(commandarguments),
				inputFiles,
				outputFiles);

			return buildOperation;
		}

		private static string CombineArguments(IList<string> arguments)
		{
			return string.Join(" ", arguments);
		}
	}
}
