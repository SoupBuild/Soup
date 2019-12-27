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
		std::shared_ptr<Build::BuildGraphNode> CreateCompileNode(const CompileArguments& args) override final
		{
			// Clang decided to do their module compilation in two stages
			// Now we have to also generate the object file from the precompiled module
			if (args.ExportModule)
			{
				return CompileModuleInterfaceUnit(args);
			}
			else
			{
				return CompileStandard(args);
			}
		}

		/// <summary>
		/// Link
		/// </summary>
		std::shared_ptr<Build::BuildGraphNode> CreateLinkNode(const LinkArguments& args) override final
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

			auto buildNode = std::make_shared<Build::BuildGraphNode>(
				std::move(executablePath),
				CombineArguments(commandArgs),
				args.RootDirectory,
				std::move(inputFiles),
				std::move(outputFiles));

			return buildNode;
		}

	private:
		std::shared_ptr<Build::BuildGraphNode> CompileStandard(const CompileArguments& args)
		{
			auto executablePath = _toolsPath + _compilerExecutable;

			// Build the set of input/output files along with the arguments
			auto inputFiles = std::vector<Path>();
			auto outputFiles = std::vector<Path>();
			auto commandArgs = 
				ArgumentBuilder::BuildCompilerArguments(args, _toolsPath, inputFiles, outputFiles);

			auto buildNode = std::make_shared<Build::BuildGraphNode>(
				std::move(executablePath),
				CombineArguments(commandArgs),
				args.RootDirectory,
				std::move(inputFiles),
				std::move(outputFiles));

			return buildNode;
		}

		std::shared_ptr<Build::BuildGraphNode> CompileModuleInterfaceUnit(const CompileArguments& args)
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
			compiledModuleArgs.GenerateIncludeTree = args.GenerateIncludeTree;
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

			auto buildNode = std::make_shared<Build::BuildGraphNode>(
				std::move(executablePath),
				CombineArguments(compiledModuleCommandArgs),
				args.RootDirectory,
				std::move(inputFiles),
				std::move(outputFiles));

			return buildNode;
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
