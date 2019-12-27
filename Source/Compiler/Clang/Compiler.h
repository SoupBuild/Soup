// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Compiler::Clang
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	export class Compiler : public ICompiler
	{
	private:
		static constexpr std::string_view CompilerExecutable = "clang++.exe";
		static constexpr std::string_view ArchiverExecutable = "llvm-ar.exe";
		static constexpr std::string_view MSVCLinkerExecutable = "lld-link.exe";

	public:
		Compiler(Path toolPath) :
			_toolPath(std::move(toolPath))
		{
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		std::string_view GetName() const override final
		{
			return "Clang";
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
			return "pcm";
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
					executablePath = _toolPath + Path(ArchiverExecutable);
					break;
				case LinkTarget::DynamicLibrary:
				case LinkTarget::Executable:
					executablePath = _toolPath + Path(MSVCLinkerExecutable);
					break;
				default:
					throw std::runtime_error("Unknown LinkTarget.");
			}

			auto commandArgs = ArgumentBuilder::BuildLinkerArguments(args);

			auto buildNode = std::make_shared<Build::BuildGraphNode>(
				std::move(executablePath),
				CombineArguments(commandArgs),
				args.RootDirectory,
				std::vector<Path>(),
				std::vector<Path>());

			return buildNode;
		}

	private:
		std::shared_ptr<Build::BuildGraphNode> CompileStandard(const CompileArguments& args)
		{
			auto executablePath = _toolPath + Path(CompilerExecutable);
			auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args);

			auto buildNode = std::make_shared<Build::BuildGraphNode>(
				std::move(executablePath),
				CombineArguments(commandArgs),
				args.RootDirectory,
				std::vector<Path>(),
				std::vector<Path>());

			return buildNode;
		}

		std::shared_ptr<Build::BuildGraphNode> CompileModuleInterfaceUnit(const CompileArguments& args)
		{
			auto executablePath = _toolPath + Path(CompilerExecutable);

			// Replace the final object target with the intermediate precompiled module
			auto generatePrecompiledModuleArgs = CompileArguments();
			generatePrecompiledModuleArgs.Standard = args.Standard;
			generatePrecompiledModuleArgs.Optimize = args.Optimize;
			generatePrecompiledModuleArgs.RootDirectory = args.RootDirectory;
			generatePrecompiledModuleArgs.IncludeDirectories = args.IncludeDirectories;
			generatePrecompiledModuleArgs.IncludeModules = args.IncludeModules;
			generatePrecompiledModuleArgs.ExportModule = true;
			generatePrecompiledModuleArgs.PreprocessorDefinitions = args.PreprocessorDefinitions;
			generatePrecompiledModuleArgs.GenerateIncludeTree = args.GenerateIncludeTree;
			generatePrecompiledModuleArgs.GenerateSourceDebugInfo = args.GenerateSourceDebugInfo;

			// Use the target file as input to the build and generate an object with the same name
			generatePrecompiledModuleArgs.SourceFile = args.SourceFile;
			generatePrecompiledModuleArgs.TargetFile = args.TargetFile;
			generatePrecompiledModuleArgs.TargetFile.SetFileExtension(GetModuleFileExtension());

			auto generatePrecompiledModuleCommandArgs = ArgumentBuilder::BuildCompilerArguments(generatePrecompiledModuleArgs);

			auto precompiledModuleBuildNode = std::make_shared<Build::BuildGraphNode>(
				executablePath,
				CombineArguments(generatePrecompiledModuleCommandArgs),
				args.RootDirectory,
				std::vector<Path>(),
				std::vector<Path>());

			// Now we can compile the object file from the precompiled module
			auto compileObjectArgs = CompileArguments();
			compileObjectArgs.Standard = args.Standard;
			compileObjectArgs.Optimize = args.Optimize;
			compileObjectArgs.RootDirectory = args.RootDirectory;
			compileObjectArgs.SourceFile = generatePrecompiledModuleArgs.TargetFile;
			compileObjectArgs.TargetFile = args.TargetFile;

			auto compileObjectCommandArgs = ArgumentBuilder::BuildCompilerArguments(compileObjectArgs);

			auto compileBuildNode = std::make_shared<Build::BuildGraphNode>(
				std::move(executablePath),
				CombineArguments(compileObjectCommandArgs),
				args.RootDirectory,
				std::vector<Path>(),
				std::vector<Path>());

			// Ensure the compile node runs after the precompile
			Build::BuildGraphNode::AddLeafChild(precompiledModuleBuildNode, compileBuildNode);

			return precompiledModuleBuildNode;
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
		Path _toolPath;
	};
}
