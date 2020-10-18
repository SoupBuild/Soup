// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Cpp::Compiler::MSVC
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
			auto executablePath = _toolsPath + _compilerExecutable;
			auto internalModules = std::vector<Path>();
			if (arguments.InterfaceUnit.has_value())
			{
				auto& interfaceUnitArguments = arguments.InterfaceUnit.value();

				// Build up the input/output sets
				auto inputFiles = sharedInputFiles;
				inputFiles.push_back(interfaceUnitArguments.SourceFile);
				inputFiles.push_back(responseFile);
				auto outputFiles = std::vector<Path>({
					interfaceUnitArguments.TargetFile,
					interfaceUnitArguments.ModuleInterfaceTarget,
				});

				// Build the unique arguments for this translation unit
				auto commandArguments = ArgumentBuilder::BuildInterfaceUnitCompilerArguments(
					interfaceUnitArguments,
					responseFile);

				// Generate the operation
				auto buildOperation = Build::Utilities::BuildOperation(
					interfaceUnitArguments.SourceFile.ToString(),
					arguments.RootDirectory,
					executablePath,
					CombineArguments(commandArguments),
					std::move(inputFiles),
					std::move(outputFiles));
				operations.push_back(std::move(buildOperation));

				// Add our module interface back in for the downstream compilers
				internalModules.push_back(interfaceUnitArguments.ModuleInterfaceTarget);
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
					responseFile,
					internalModules);

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
			inputFiles.insert(inputFiles.end(), arguments.LibraryFiles.begin(), arguments.LibraryFiles.end());
			inputFiles.insert(inputFiles.end(), arguments.ObjectFiles.begin(), arguments.ObjectFiles.end());
			auto outputFiles = std::vector<Path>({
				arguments.TargetFile,
			});
			auto commandarguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto buildOperation = Build::Utilities::BuildOperation(
				arguments.TargetFile.ToString(),
				arguments.RootDirectory,
				executablePath,
				CombineArguments(commandarguments),
				inputFiles,
				outputFiles);

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
		Path _toolsPath;
		Path _compilerExecutable;
		Path _linkerExecutable;
		Path _libraryExecutable;
	};
}
