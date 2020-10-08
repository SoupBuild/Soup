// <copyright file="BuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildArguments.h"
#include "BuildResult.h"
#include "BuildUtilities.h"
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
			auto objectDirectory = arguments.WorkingDirectory + arguments.ObjectDirectory;
			auto binaryDirectory = arguments.WorkingDirectory + arguments.BinaryDirectory;
			result.BuildOperations.push_back(
				BuildUtilities::CreateCreateDirectoryOperation(buildState, objectDirectory));
			result.BuildOperations.push_back(
				BuildUtilities::CreateCreateDirectoryOperation(buildState, binaryDirectory));

			// Perform the core compilation of the source files
			CoreCompile(buildState, arguments, result);

			// Copy previous runtime dependencies after linking has completed
			CopyRuntimeDependencies(buildState, arguments, result);

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
			auto rootCompileOperations = std::vector<Soup::Build::Utilities::BuildOperationWrapper>();

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
			auto operation = _compiler->CreateCompileOperation(buildState, compileArguments);

			// Run the core compile next
			Soup::Build::Utilities::BuildOperationExtensions::AddLeafChild(result.BuildOperations, std::move(operation));
		}

		/// <summary>
		/// Copy runtime dependencies
		/// </summary>
		void CopyRuntimeDependencies(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			if (arguments.TargetType == BuildTargetType::Executable)
			{
				auto copyOperations = std::vector<Soup::Build::Utilities::BuildOperationWrapper>();
				for (auto source : arguments.RuntimeDependencies)
				{
					auto target = arguments.WorkingDirectory + arguments.BinaryDirectory + Path(source.GetFileName());
					auto operation = BuildUtilities::CreateCopyFileOperation(buildState, source, target);
					copyOperations.push_back(operation);
				}
				
				if (!copyOperations.empty())
				{
					Soup::Build::Utilities::BuildOperationExtensions::AddLeafChildren(result.BuildOperations, copyOperations);
				}
			}
		}

	private:
		std::shared_ptr<Compiler> _compiler;
	};
}
