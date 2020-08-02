// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Compiler::MSVC
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	export class Compiler : public ICompiler
	{
	public:
		Compiler(Path toolsPath, Path compilerExecutable, Path linkerExecutable, Path libraryExecutable) :
			_toolsPath(std::move(toolsPath)),
			_compilerExecutable(std::move(compilerExecutable)),
			_linkerExecutable(std::move(linkerExecutable)),
			_libraryExecutable(std::move(libraryExecutable))
		{
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		std::string_view GetName() const override final
		{
			return "MSVC";
		}

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		std::string_view GetObjectFileExtension() const override final
		{
			return "obj";
		}

		/// <summary>
		/// Gets the module file extension for the compiler
		/// </summary>
		std::string_view GetModuleFileExtension() const override final
		{
			return "ifc";
		}

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		std::string_view GetStaticLibraryFileExtension() const override final
		{
			return "lib";
		}

		/// <summary>
		/// Gets the dynmaic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		std::string_view GetDynamicLibraryFileExtension() const override final
		{
			return "dll";
		}

		/// <summary>
		/// Compile
		/// </summary>
		Build::Utilities::BuildOperationWrapper CreateCompileOperation(
			Build::Utilities::BuildStateWrapper& state,
			const CompileArguments& args) const override final
		{
			// Clang decided to do their module compilation in two stages
			// Now we have to also generate the object file from the precompiled module
			if (args.ExportModule)
			{
				return CompileModuleInterfaceUnit(state, args);
			}
			else
			{
				return CompileStandard(state, args);
			}
		}

		/// <summary>
		/// Link
		/// </summary>
		Build::Utilities::BuildOperationWrapper CreateLinkOperation(
			Build::Utilities::BuildStateWrapper& state,
			const LinkArguments& args) const override final
		{
			// Select the correct executable for linking libraries or executables
			Path executablePath;
			switch (args.TargetType)
			{
				case LinkTarget::StaticLibrary:
					executablePath = _toolsPath + _libraryExecutable;
					break;
				case LinkTarget::DynamicLibrary:
				case LinkTarget::Executable:
					executablePath = _toolsPath + _linkerExecutable;
					break;
				default:
					throw std::runtime_error("Unknown LinkTarget.");
			}

			// Build the set of input/output files along with the arguments
			auto inputFiles = std::vector<Path>();
			auto outputFiles = std::vector<Path>();
			auto commandArgs = ArgumentBuilder::BuildLinkerArguments(args, inputFiles, outputFiles);

			auto buildOperation = Build::Utilities::BuildOperationWrapper(
				new Build::Utilities::BuildOperation(
					args.TargetFile.ToString(),
					executablePath,
					CombineArguments(commandArgs),
					args.RootDirectory,
					inputFiles,
					outputFiles));

			return buildOperation;
		}

	private:
		Build::Utilities::BuildOperationWrapper CompileStandard(
			Build::Utilities::BuildStateWrapper& state,
			const CompileArguments& args) const
		{
			auto executablePath = _toolsPath + _compilerExecutable;

			// Build the set of input/output files along with the arguments
			auto inputFiles = std::vector<Path>();
			auto outputFiles = std::vector<Path>();
			auto commandArgs = ArgumentBuilder::BuildCompilerArguments(
				args,
				_toolsPath,
				inputFiles,
				outputFiles);

			auto buildOperation = Build::Utilities::BuildOperationWrapper(
				new Build::Utilities::BuildOperation(
					args.SourceFile.ToString(),
					executablePath,
					CombineArguments(commandArgs),
					args.RootDirectory,
					inputFiles,
					outputFiles));

			return buildOperation;
		}

		Build::Utilities::BuildOperationWrapper CompileModuleInterfaceUnit(
			Build::Utilities::BuildStateWrapper& state,
			const CompileArguments& args) const
		{
			auto executablePath = _toolsPath + _compilerExecutable;

			// Replace the final object target with the intermediate precompiled module
			auto compiledModuleArgs = CompileArguments();
			compiledModuleArgs.Standard = args.Standard;
			compiledModuleArgs.Optimize = args.Optimize;
			compiledModuleArgs.RootDirectory = args.RootDirectory;
			compiledModuleArgs.IncludeDirectories = args.IncludeDirectories;
			compiledModuleArgs.IncludeModules = args.IncludeModules;
			compiledModuleArgs.ExportModule = true;
			compiledModuleArgs.PreprocessorDefinitions = args.PreprocessorDefinitions;
			compiledModuleArgs.GenerateSourceDebugInfo = args.GenerateSourceDebugInfo;

			// Use the target file as input to the build and generate an object with the same name
			compiledModuleArgs.SourceFile = args.SourceFile;
			compiledModuleArgs.TargetFile = args.TargetFile;

			// Build the set of input/output files along with the arguments
			auto inputFiles = std::vector<Path>();
			auto outputFiles = std::vector<Path>();
			auto compiledModuleCommandArgs = ArgumentBuilder::BuildCompilerArguments(
				compiledModuleArgs,
				_toolsPath,
				inputFiles,
				outputFiles);

			auto buildOperation = Build::Utilities::BuildOperationWrapper(
				new Build::Utilities::BuildOperation(
					args.SourceFile.ToString(),
					executablePath,
					CombineArguments(compiledModuleCommandArgs),
					args.RootDirectory,
					inputFiles,
					outputFiles));

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
		Path _linkerExecutable;
		Path _libraryExecutable;
	};
}
