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
		Build::Extensions::BuildOperationWrapper CreateCompileNode(
			Build::Extensions::BuildStateWrapper& state,
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
		Build::Extensions::BuildOperationWrapper CreateLinkNode(
			Build::Extensions::BuildStateWrapper& state,
			const LinkArguments& args) const override final
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

			// Build the set of input/output files along with the arguments
			auto inputFiles = std::vector<Path>();
			auto outputFiles = std::vector<Path>();
			auto commandArgs = ArgumentBuilder::BuildLinkerArguments(args, inputFiles, outputFiles);

			auto buildOperation = Build::Extensions::BuildOperationWrapper(
				new Build::Extensions::BuildOperation(
					args.TargetFile.ToString(),
					executablePath,
					CombineArguments(commandArgs),
					args.RootDirectory,
					inputFiles,
					outputFiles));

			return buildOperation;
		}

	private:
		Build::Extensions::BuildOperationWrapper CompileStandard(
			Build::Extensions::BuildStateWrapper& state,
			const CompileArguments& args) const
		{
			auto executablePath = _toolPath + Path(CompilerExecutable);
			
			// Build the set of input/output files along with the arguments
			auto inputFiles = std::vector<Path>();
			auto outputFiles = std::vector<Path>();
			auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args, inputFiles, outputFiles);

			auto buildOperation = Build::Extensions::BuildOperationWrapper(
				new Build::Extensions::BuildOperation(
					args.SourceFile.ToString(),
					executablePath,
					CombineArguments(commandArgs),
					args.RootDirectory,
					inputFiles,
					outputFiles));

			return buildOperation;
		}

		Build::Extensions::BuildOperationWrapper CompileModuleInterfaceUnit(
			Build::Extensions::BuildStateWrapper& state,
			const CompileArguments& args) const
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

			// Build the set of input/output files along with the arguments
			auto generatePrecompiledModuleInputFiles = std::vector<Path>();
			auto generatePrecompiledModuleOutputFiles = std::vector<Path>();
			auto generatePrecompiledModuleCommandArgs = 
				ArgumentBuilder::BuildCompilerArguments(
					generatePrecompiledModuleArgs,
					generatePrecompiledModuleInputFiles,
					generatePrecompiledModuleOutputFiles);

			auto precompiledModuleBuildOperation = Build::Extensions::BuildOperationWrapper(
				new Build::Extensions::BuildOperation(
					generatePrecompiledModuleArgs.SourceFile.ToString(),
					executablePath,
					CombineArguments(generatePrecompiledModuleCommandArgs),
					args.RootDirectory,
					generatePrecompiledModuleInputFiles,
					generatePrecompiledModuleOutputFiles));

			// Now we can compile the object file from the precompiled module
			auto compileObjectArgs = CompileArguments();
			compileObjectArgs.Standard = args.Standard;
			compileObjectArgs.Optimize = args.Optimize;
			compileObjectArgs.RootDirectory = args.RootDirectory;
			compileObjectArgs.SourceFile = generatePrecompiledModuleArgs.TargetFile;
			compileObjectArgs.TargetFile = args.TargetFile;

			// Build the set of input/output files along with the arguments
			auto compileObjectInputFiles = std::vector<Path>();
			auto compileObjectOutputFiles = std::vector<Path>();
			auto compileObjectCommandArgs = 
				ArgumentBuilder::BuildCompilerArguments(
					compileObjectArgs,
					compileObjectInputFiles,
					compileObjectOutputFiles);

			auto compileBuildOperation = Build::Extensions::BuildOperationWrapper(
				new Build::Extensions::BuildOperation(
					compileObjectArgs.SourceFile.ToString(),
					executablePath,
					CombineArguments(compileObjectCommandArgs),
					args.RootDirectory,
					compileObjectInputFiles,
					compileObjectOutputFiles));

			// Ensure the compile node runs after the precompile
			Build::Extensions::BuildOperationExtensions::AddLeafChild(
				precompiledModuleBuildOperation,
				compileBuildOperation);

			return precompiledModuleBuildOperation;
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
