// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Cpp::Compiler::Clang
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
		/// Gets the dynamic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		std::string_view GetDynamicLibraryFileExtension() const override final
		{
			return "dll";
		}

		/// <summary>
		/// Compile
		/// </summary>
		std::vector<Build::Utilities::BuildOperation> CreateCompileOperations(
			const SharedCompileArguments& arguments) const override final
		{
			auto operations = std::vector<Build::Utilities::BuildOperation>();

			// Write the shared arguments to the response file
			auto responseFile = arguments.ObjectDirectory + Path("SharedCompileArguments.txt");
			auto sharedCommandArguments = ArgumentBuilder::BuildSharedCompilerArguments(arguments);
			auto writeSharedArgumentsOperation = Build::Utilities::SharedOperations::CreateWriteFileOperation(
				arguments.RootDirectory,
				responseFile,
				CombineArguments(sharedCommandArguments));
			operations.push_back(std::move(writeSharedArgumentsOperation));

			// Initialize a shared input set
			auto sharedInputFiles = arguments.IncludeModules;

			// Generate the interface build operation if present
			auto executablePath = _toolPath + Path(CompilerExecutable);
			if (arguments.InterfaceUnit.has_value())
			{
				auto& interfaceUnitArguments = arguments.InterfaceUnit.value();

				// Precompile the module interface unit
				auto precompiledModuleInputFiles = sharedInputFiles;
				precompiledModuleInputFiles.push_back(interfaceUnitArguments.SourceFile);
				precompiledModuleInputFiles.push_back(responseFile);
				auto precompiledModuleOutputFiles = std::vector<Path>({
					interfaceUnitArguments.ModuleInterfaceTarget,
				});
				auto precompiledModuleCommandArguments =
					ArgumentBuilder::BuildInterfaceUnitCompilerArguments(
						interfaceUnitArguments,
						responseFile);

				// Generate the operation
				auto precompiledModuleOperation = Build::Utilities::BuildOperation(
					interfaceUnitArguments.SourceFile.ToString(),
					arguments.RootDirectory,
					executablePath,
					CombineArguments(precompiledModuleCommandArguments),
					std::move(precompiledModuleInputFiles),
					std::move(precompiledModuleOutputFiles));
				operations.push_back(std::move(precompiledModuleOperation));

				// Compile the precompiled module
				auto compilePrecompiledArguments = TranslationUnitCompileArguments();
				compilePrecompiledArguments.SourceFile = interfaceUnitArguments.ModuleInterfaceTarget;
				compilePrecompiledArguments.TargetFile = interfaceUnitArguments.TargetFile;

				auto compilePrecompiledInputFiles = std::vector<Path>({
					interfaceUnitArguments.ModuleInterfaceTarget,
					responseFile,
				});
				auto compilePrecompiledOutputFiles = std::vector<Path>({
					interfaceUnitArguments.TargetFile,
				});
				auto compilePrecompiledCommandArguments =
					ArgumentBuilder::BuildTranslationUnitCompilerArguments(
						compilePrecompiledArguments,
						responseFile);

				// Generate the operation
				auto compilePrecompiledOperation = Build::Utilities::BuildOperation(
					interfaceUnitArguments.ModuleInterfaceTarget.ToString(),
					arguments.RootDirectory,
					executablePath,
					CombineArguments(compilePrecompiledCommandArguments),
					std::move(compilePrecompiledInputFiles),
					std::move(compilePrecompiledOutputFiles));
				operations.push_back(std::move(compilePrecompiledOperation));
			}

			for (auto& implementationUnitArguments : arguments.ImplementationUnits)
			{
				// Build up the input/output sets
				auto inputFiles = sharedInputFiles;
				inputFiles.push_back(implementationUnitArguments.SourceFile);
				inputFiles.push_back(responseFile);
				auto outputFiles = std::vector<Path>({
					implementationUnitArguments.TargetFile,
				});

				// Build the unique arguments for this translation unit
				auto commandArguments = ArgumentBuilder::BuildTranslationUnitCompilerArguments(
					implementationUnitArguments,
					responseFile);

				// Generate the operation
				auto buildOperation = Build::Utilities::BuildOperation(
					implementationUnitArguments.SourceFile.ToString(),
					arguments.RootDirectory,
					executablePath,
					CombineArguments(commandArguments),
					std::move(inputFiles),
					std::move(outputFiles));
				operations.push_back(std::move(buildOperation));
			}

			return operations;
		}

		/// <summary>
		/// Link
		/// </summary>
		Build::Utilities::BuildOperation CreateLinkOperation(
			const LinkArguments& arguments) const override final
		{
			// Select the correct executable for linking libraries or executables
			Path executablePath;
			switch (arguments.TargetType)
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

			// Build the set of input/output sets
			auto inputFiles = std::vector<Path>();
			inputFiles.insert(inputFiles.end(), arguments.LibraryFiles.begin(), arguments.LibraryFiles.end());
			inputFiles.insert(inputFiles.end(), arguments.ObjectFiles.begin(), arguments.ObjectFiles.end());
			auto outputFiles = std::vector<Path>({
				arguments.TargetFile,
			});

			// Generate the link arguments
			auto commandArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto buildOperation = Build::Utilities::BuildOperation(
				arguments.TargetFile.ToString(),
				arguments.RootDirectory,
				executablePath,
				CombineArguments(commandArguments),
				std::move(inputFiles),
				std::move(outputFiles));

			return buildOperation;
		}

	private:
		static std::string CombineArguments(const std::vector<std::string>& arguments)
		{
			auto argumentString = std::stringstream();
			bool isFirst = true;
			for (auto& arg : arguments)
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
