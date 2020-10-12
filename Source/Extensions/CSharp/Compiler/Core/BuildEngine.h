// <copyright file="BuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildArguments.h"
#include "BuildResult.h"
#include "Compiler.h"

namespace Soup::CSharp::Compiler
{
	/// <summary>
	/// The build engine
	/// </summary>
	export class BuildEngine
	{
	public:
		BuildEngine(std::shared_ptr<Compiler> compiler) :
			_compiler(std::move(compiler))
		{
		}

		/// <summary>
		/// Generate the required build operations for the requested build
		/// </summary>
		BuildResult Execute(
			Soup::Build::Utilities::BuildStateWrapper buildState,
			const BuildArguments& arguments)
		{
			auto result = BuildResult();

			// Ensure the output directories exists as the first step
			result.BuildOperations.push_back(
				Build::Utilities::SharedOperations::CreateCreateDirectoryOperation(
					arguments.WorkingDirectory,
					arguments.ObjectDirectory));
			result.BuildOperations.push_back(
				Build::Utilities::SharedOperations::CreateCreateDirectoryOperation(
					arguments.WorkingDirectory,
					arguments.BinaryDirectory));

			// Perform the core compilation of the source files
			CoreCompile(buildState, arguments, result);

			// Copy previous runtime dependencies after linking has completed
			CopyRuntimeDependencies(arguments, result);

			return result;
		}

	private:
		/// <summary>
		/// Compile the module and source files
		/// </summary>
		void CoreCompile(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			if (!arguments.SourceFiles.empty())
			{
				CompileSourceFiles(
					buildState,
					arguments,
					result);
			}
		}

		/// <summary>
		/// Compile the supporting source files
		/// </summary>
		void CompileSourceFiles(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			// Check if we can skip the whole dang thing
			buildState.LogInfo("Generate compile source files");

			// Setup the shared properties
			auto compileArguments = CompileArguments();
			compileArguments.EnableOptimize = arguments.EnableOptimization;
			compileArguments.RootDirectory = arguments.WorkingDirectory;
			compileArguments.PreprocessorDefinitions = {};
			compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;
			compileArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;
			compileArguments.SourceFiles = arguments.SourceFiles;
			compileArguments.TargetFile = arguments.ObjectDirectory + Path(arguments.TargetName);

			// Compile the requested target
			auto operation = _compiler->CreateCompileOperation(compileArguments);
			result.BuildOperations.push_back(std::move(operation));
		}

		/// <summary>
		/// Copy runtime dependencies
		/// </summary>
		void CopyRuntimeDependencies(
			const BuildArguments& arguments,
			BuildResult& result)
		{
			if (arguments.TargetType == BuildTargetType::Executable ||
				arguments.TargetType == BuildTargetType::Library)
			{
				for (auto source : arguments.RuntimeDependencies)
				{
					auto target = arguments.BinaryDirectory + Path(source.GetFileName());
					auto operation = Build::Utilities::SharedOperations::CreateCopyFileOperation(
						arguments.WorkingDirectory,
						source,
						target);
					result.BuildOperations.push_back(std::move(operation));
				}
			}
		}

	private:
		std::shared_ptr<Compiler> _compiler;
	};
}
