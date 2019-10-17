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
			const CompileArguments& args)
		{
			// Verify the input
			if (args.SourceFile.GetFileName().empty())
				throw std::runtime_error("Source file cannot be empty.");
			if (args.TargetFile.GetFileName().empty())
				throw std::runtime_error("Target file cannot be empty.");

			// Calculate object output file
			auto commandArgs = std::vector<std::string>();

			// Enable verbose output
			// commandArgs.Add("-v");

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
				auto argument = "-I\"" + directory.ToString() + "\"";
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

			// Add the module references
			for (auto& moduleFile : args.IncludeModules)
			{
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

			// Add the source file
			commandArgs.push_back(args.SourceFile.ToString());

			// Add the target file
			commandArgs.push_back("-o");
			commandArgs.push_back(args.TargetFile.ToString());

			return commandArgs;
		}

		static std::vector<std::string> BuildLinkerArguments(const LinkArguments& args)
		{
			// Verify the input
			if (args.ObjectFiles.empty())
				throw std::runtime_error("Object files cannot be empty.");
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
					commandArgs.push_back(args.TargetFile.ToString());
					break;
				}
				case LinkTarget::Executable:
				{
					// Executables are put together by clang

					// Enable verbose output
					// commandArgs.push_back("-v");

					commandArgs.push_back("-o");
					commandArgs.push_back(args.TargetFile.ToString());
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown LinkTarget.");
				}
			}

			// Add the library files
			for (auto& file : args.LibraryFiles)
			{
				commandArgs.push_back(file.ToString());
			}

			// Add the object files
			for (auto& file : args.ObjectFiles)
			{
				commandArgs.push_back(file.ToString());
			}

			return commandArgs;
		}
	};
}
