// <copyright file="BuildEngine.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;

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
			result.BuildOperations.Add(
				SharedOperations.CreateCreateDirectoryOperation(
					arguments.WorkingDirectory,
					arguments.ObjectDirectory));
			result.BuildOperations.Add(
				SharedOperations.CreateCreateDirectoryOperation(
					arguments.WorkingDirectory,
					arguments.BinaryDirectory));

			// Perform the core compilation of the source files
			CoreCompile(buildState, arguments, result);

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
			if (arguments.SourceFiles.Count != 0)
			{
				// Setup the shared properties
				var compileArguments = new CompileArguments()
				{
					RootDirectory = arguments.WorkingDirectory,
					ObjectDirectory = arguments.ObjectDirectory,
					SourceFiles = arguments.SourceFiles,
					PreprocessorDefinitions = arguments.PreprocessorDefinitions,
					GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo,
					EnableWarningsAsErrors = arguments.EnableWarningsAsErrors,
					DisabledWarnings = arguments.DisabledWarnings,
					EnabledWarnings = arguments.EnabledWarnings,
					CustomProperties = arguments.CustomProperties,
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
				arguments.TargetType == BuildTargetType.DynamicLibrary)
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
