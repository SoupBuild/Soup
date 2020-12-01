// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Rust::Compiler
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	export class Compiler
	{
	public:
		Compiler(Path toolsPath, Path compilerExecutable) :
			_toolsPath(std::move(toolsPath)),
			_compilerExecutable(std::move(compilerExecutable))
		{
		}

		/// <summary>
		/// Compile
		/// </summary>
		Build::Utilities::BuildOperation CreateCompileOperation(
			const CompileArguments& args) const
		{
			return CompileStandard(args);
		}

	private:
		Build::Utilities::BuildOperation CompileStandard(
			const CompileArguments& args) const
		{
			auto executablePath = _toolsPath + _compilerExecutable;

			// Build the set of input/output files along with the arguments
			auto inputFiles = std::vector<Path>();
			auto outputFiles = std::vector<Path>();
			auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args);

			auto buildOperation = Build::Utilities::BuildOperation(
				"CoreCompile",
				args.RootDirectory,
				executablePath,
				CombineArguments(commandArgs),
				inputFiles,
				outputFiles);

			return buildOperation;
		}

		static std::string CombineArguments(const std::vector<std::string>& args)
		{
			auto argumentString = std::stringstream();
			bool isFirst = true;
			for (auto& arg : args)
			{
				if (!isFirst)
					argumentString << " ";

				argumentString << arg;
				isFirst = false;
			}

			return argumentString.str();
		}

	private:
		Path _toolsPath;
		Path _compilerExecutable;
	};
}
