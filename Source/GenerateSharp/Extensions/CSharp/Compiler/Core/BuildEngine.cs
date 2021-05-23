// <copyright file="BuildEngine.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System;

namespace Soup.Build.CSharp.Compiler
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

			// Ensure the output directories exists as the first step
			var referenceDirectory = arguments.BinaryDirectory + new Path("ref/");
			result.BuildOperations.Add(
				SharedOperations.CreateCreateDirectoryOperation(
					arguments.WorkingDirectory,
					arguments.ObjectDirectory));
			result.BuildOperations.Add(
				SharedOperations.CreateCreateDirectoryOperation(
					arguments.WorkingDirectory,
					arguments.BinaryDirectory));
			result.BuildOperations.Add(
				SharedOperations.CreateCreateDirectoryOperation(
					arguments.WorkingDirectory,
					referenceDirectory));

			// Perform the core compilation of the source files
			CoreCompile(buildState, arguments, referenceDirectory, result);

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
			Path referenceDirectory,
			BuildResult result)
		{
			// Ensure there are actually files to build
			if (arguments.SourceFiles.Count != 0)
			{
				Path targetFile;
				Path referenceTargetFile;
				switch (arguments.TargetType)
				{
					case BuildTargetType.Library:
						targetFile =
							arguments.BinaryDirectory +
							new Path(arguments.TargetName + "." + _compiler.DynamicLibraryFileExtension);
						referenceTargetFile =
							referenceDirectory +
							new Path(arguments.TargetName + "." + _compiler.DynamicLibraryFileExtension);

						// Add the DLL as a runtime dependency
						result.RuntimeDependencies.Add(targetFile);

						// Link against the reference target
						result.LinkDependencies.Add(referenceTargetFile);

						break;
					case BuildTargetType.Executable:
						targetFile =
							arguments.BinaryDirectory +
							new Path(arguments.TargetName + ".exe");
						referenceTargetFile =
							referenceDirectory +
							new Path(arguments.TargetName + "." + _compiler.DynamicLibraryFileExtension);

						// Add the Executable as a runtime dependency
						result.RuntimeDependencies.Add(targetFile);

						// All link dependencies stop here.

						break;
					default:
						throw new InvalidOperationException("Unknown build target type.");
				}

				// Setup the shared properties
				var compileArguments = new CompileArguments()
				{
					Target = targetFile,
					ReferenceTarget = referenceTargetFile,
					RootDirectory = arguments.WorkingDirectory,
					ObjectDirectory = arguments.ObjectDirectory,
					SourceFiles = arguments.SourceFiles,
					PreprocessorDefinitions = arguments.PreprocessorDefinitions,
					GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo,
					EnableWarningsAsErrors = arguments.EnableWarningsAsErrors,
					DisabledWarnings = arguments.DisabledWarnings,
					EnabledWarnings = arguments.EnabledWarnings,
					CustomProperties = arguments.CustomProperties,
					ReferenceLibraries = arguments.LinkDependencies,
				};

				// Compile all source files as a single call
				var compileOperations = _compiler.CreateCompileOperations(compileArguments);
				foreach (var operation in compileOperations)
					result.BuildOperations.Add(operation);
			}
		}

		/// <summary>
		/// Copy runtime dependencies
		/// </summary>
		private void CopyRuntimeDependencies(
			BuildArguments arguments,
			BuildResult result)
		{
			if (arguments.TargetType == BuildTargetType.Executable ||
				arguments.TargetType == BuildTargetType.Library)
			{
				foreach (var source in arguments.RuntimeDependencies)
				{
					var target = arguments.BinaryDirectory + new Path(source.GetFileName());
					var operation = SharedOperations.CreateCopyFileOperation(
						arguments.WorkingDirectory,
						source,
						target);
					result.BuildOperations.Add(operation);
				}
			}
		}

		private ICompiler _compiler;
	}
}
