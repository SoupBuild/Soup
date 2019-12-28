// <copyright file="ArgumentBuilder.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::Clang
{
	/// <summary>
	/// A helper class that builds the correct set of compiler arguments for a given
	/// set of options.
	/// </summary>
	export class ArgumentBuilder
	{
	public:
		static std::vector<std::string> BuildCompilerArguments(
			const CompileArguments& args,
			std::vector<Path>& inputFiles,
			std::vector<Path>& outputFiles)
		{
			// Verify the input
			if (args.SourceFile.GetFileName().empty())
				throw std::runtime_error("Source file cannot be empty.");
			if (args.TargetFile.GetFileName().empty())
				throw std::runtime_error("Target file cannot be empty.");

			// Calculate object output file
			auto commandArgs = std::vector<std::string>();

			// Enable verbose output
			// commandArgs.push_back("-v");

			// Disable std include paths
			// It is not the job of the compiler to set this up
			commandArgs.push_back("-nostdinc");

			// Disable warnings on unknown attributes to allow test attributes
			commandArgs.push_back("-Wno-unknown-attributes");

			// Disable ms compatibility (workaround for bug with inplicit types in pcm)
			// commandArgs.push_back("-fno-ms-compatibility");

			// Allow public std visible during link time
			commandArgs.push_back("-Xclang");
			commandArgs.push_back("-flto-visibility-public-std");

			// Enable Header includes if needed
			if (args.GenerateIncludeTree)
			{
				commandArgs.push_back("-H");
			}

			// Generate source debug information
			if (args.GenerateSourceDebugInfo)
			{
				commandArgs.push_back("-g");
			}

			// Set the language standard
			switch (args.Standard)
			{
				case LanguageStandard::CPP11:
					commandArgs.push_back("-std=c++11");
					break;
				case LanguageStandard::CPP14:
					commandArgs.push_back("-std=c++14");
					break;
				case LanguageStandard::CPP17:
					commandArgs.push_back("-std=c++17");
					break;
				case LanguageStandard::CPP20:
					commandArgs.push_back("-std=c++2a");
					break;
				default:
					throw std::runtime_error("Unknown language standard.");
			}

			// Set the optimization level
			switch (args.Optimize)
			{
				case OptimizationLevel::None:
					// DEFAULT: "-O0";
					break;
				case OptimizationLevel::Speed:
					commandArgs.push_back("-O3");
					break;
				case OptimizationLevel::Size:
					commandArgs.push_back("-Oz");
					break;
				default:
					throw std::runtime_error("Unknown optimization level.");
			}

			// Set the include paths
			for (auto directory : args.IncludeDirectories)
			{
				// TODO: May want to have flag for system rooted includes
				auto argument = std::string();
				if (directory.HasRoot())
				{
					// Treat the include as a system path to not produce warnings
					argument = "-isystem \"" + directory.ToString() + "\"";
				}
				else
				{
					argument = "-I\"" + directory.ToString() + "\"";
				}
				
				commandArgs.push_back(std::move(argument));
			}

			// Set the preprocessor definitions
			for (auto& definition : args.PreprocessorDefinitions)
			{
				auto argument = "-D" + definition;
				commandArgs.push_back(std::move(argument));
			}

			// Enable experimental features for C++ 20
			if (args.Standard == LanguageStandard::CPP20)
			{
				// NO LONGER NEEDED commandArgs.push_back("-fmodules-ts");
			}

			// Ignore Standard Include Paths to prevent pulling in accidental headers
			// commandArgs.Add("-X");

			// Add in the std include paths

			// Enable c++ exceptions
			// commandArgs.Add("-EHs");

			// Add the module references as input
			for (auto& moduleFile : args.IncludeModules)
			{
				inputFiles.push_back(moduleFile);
				auto argument = "-fmodule-file=\"" + moduleFile.ToString() + "\"";
				commandArgs.push_back(std::move(argument));
			}

			if (args.ExportModule)
			{
				commandArgs.push_back("--precompile");

				// Place the ifc in the output directory
				//var outputFile = "{Path.GetFileNameWithoutExtension(sourceFile)}.{ModuleFileExtension}";
				//commandArgs.AddRange(new string[] { "-o", outputFile });
			}
			else
			{
				// Only run preprocessor, compile and assemble
				commandArgs.push_back("-c");
			}

			// Add the source file as input
			inputFiles.push_back(args.SourceFile);
			commandArgs.push_back(args.SourceFile.ToString());

			// Add the target file as output
			outputFiles.push_back(args.TargetFile);
			commandArgs.push_back("-o");
			commandArgs.push_back(args.TargetFile.ToString());

			return commandArgs;
		}

		static std::vector<std::string> BuildLinkerArguments(
			const LinkArguments& args,
			std::vector<Path>& inputFiles,
			std::vector<Path>& outputFiles)
		{
			// Verify the input
			if (args.TargetFile.GetFileName().empty())
				throw std::runtime_error("Target file cannot be empty.");

			// Calculate object output file
			auto commandArgs = std::vector<std::string>();

			switch (args.TargetType)
			{
				case LinkTarget::StaticLibrary:
				{
					// Static libraries are linked with ar
					// r - Replace existing
					// c - Create without warning if does not exist
					commandArgs.push_back("rc");

					// Add the output file
					outputFiles.push_back(args.TargetFile);
					commandArgs.push_back(args.TargetFile.ToString());

					// Add the library files
					for (auto& file : args.LibraryFiles)
					{
						// Add the library files as input
						inputFiles.push_back(file);
						commandArgs.push_back(file.ToString());
					}

					// Add the object files
					for (auto& file : args.ObjectFiles)
					{
						// Add the object files as input
						inputFiles.push_back(file);
						commandArgs.push_back(file.ToString());
					}

					break;
				}
				case LinkTarget::DynamicLibrary:
				case LinkTarget::Executable:
				{
					// Dynamic libraries and executables targeting windows are put together by lld-link
					commandArgs = MSVC::ArgumentBuilder::BuildLinkerArguments(
						args,
						inputFiles,
						outputFiles);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown LinkTarget.");
				}
			}

			return commandArgs;
		}
	};
}
