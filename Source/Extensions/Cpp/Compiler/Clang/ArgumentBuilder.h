// <copyright file="ArgumentBuilder.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::Clang
{
	/// <summary>
	/// A helper class that builds the correct set of compiler arguments for a given
	/// set of options.
	/// </summary>
	export class ArgumentBuilder
	{
	public:
		static std::vector<std::string> BuildSharedCompilerArguments(
			const SharedCompileArguments& arguments)
		{
			// Calculate object output file
			auto commandArguments = std::vector<std::string>();

			// Enable verbose output
			// commandArguments.push_back("-v");

			// Disable std include paths
			// It is not the job of the compiler to set this up
			commandArguments.push_back("-nostdinc");

			// Disable warnings on unknown attributes to allow test attributes
			commandArguments.push_back("-Wno-unknown-attributes");

			// Disable ms compatibility (workaround for bug with inplicit types in pcm)
			// commandArguments.push_back("-fno-ms-compatibility");

			// Allow public std visible during link time
			commandArguments.push_back("-Xclang");
			commandArguments.push_back("-flto-visibility-public-std");

			// Generate source debug information
			if (arguments.GenerateSourceDebugInfo)
			{
				commandArguments.push_back("-g");
			}

			// Set the language standard
			switch (arguments.Standard)
			{
				case LanguageStandard::CPP11:
					commandArguments.push_back("-std=c++11");
					break;
				case LanguageStandard::CPP14:
					commandArguments.push_back("-std=c++14");
					break;
				case LanguageStandard::CPP17:
					commandArguments.push_back("-std=c++17");
					break;
				case LanguageStandard::CPP20:
					commandArguments.push_back("-std=c++2a");
					break;
				default:
					throw std::runtime_error("Unknown language standard.");
			}

			// Set the optimization level
			switch (arguments.Optimize)
			{
				case OptimizationLevel::None:
					// DEFAULT: "-O0";
					break;
				case OptimizationLevel::Speed:
					commandArguments.push_back("-O3");
					break;
				case OptimizationLevel::Size:
					commandArguments.push_back("-Oz");
					break;
				default:
					throw std::runtime_error("Unknown optimization level.");
			}

			// Set the include paths
			for (auto directory : arguments.IncludeDirectories)
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
				
				commandArguments.push_back(std::move(argument));
			}

			// Set the preprocessor definitions
			for (auto& definition : arguments.PreprocessorDefinitions)
			{
				auto argument = "-D" + definition;
				commandArguments.push_back(std::move(argument));
			}

			// Enable experimental features for C++ 20
			if (arguments.Standard == LanguageStandard::CPP20)
			{
				// NO LONGER NEEDED commandArguments.push_back("-fmodules-ts");
			}

			// Ignore Standard Include Paths to prevent pulling in accidental headers
			// commandArguments.Add("-X");

			// Add in the std include paths

			// Enable c++ exceptions
			// commandArguments.Add("-EHs");

			// Add the module references as input
			for (auto& moduleFile : arguments.IncludeModules)
			{
				auto argument = "-fmodule-file=\"" + moduleFile.ToString() + "\"";
				commandArguments.push_back(std::move(argument));
			}

			return commandArguments;
		}

		static std::vector<std::string> BuildInterfaceUnitCompilerArguments(
			const InterfaceUnitCompileArguments& arguments,
			const Path& responseFile)
		{
			// Calculate object output file
			auto commandArguments = std::vector<std::string>();

			// Add the response file
			commandArguments.push_back("@" + responseFile.ToString());

			commandArguments.push_back("--precompile");

			// Place the ifc in the output directory
			//var outputFile = "{Path.GetFileNameWithoutExtension(sourceFile)}.{ModuleFileExtension}";
			//commandArguments.AddRange(new string[] { "-o", outputFile });

			// Add the source file as input
			commandArguments.push_back(arguments.SourceFile.ToString());

			// Add the target file as output
			commandArguments.push_back("-o");
			commandArguments.push_back(arguments.ModuleInterfaceTarget.ToString());

			return commandArguments;
		}

		static std::vector<std::string> BuildTranslationUnitCompilerArguments(
			const TranslationUnitCompileArguments& arguments,
			const Path& responseFile)
		{
			// Calculate object output file
			auto commandArguments = std::vector<std::string>();

			// Add the response file
			commandArguments.push_back("@" + responseFile.ToString());

			// Only run preprocessor, compile and assemble
			commandArguments.push_back("-c");

			// Add the source file as input
			commandArguments.push_back(arguments.SourceFile.ToString());

			// Add the target file as output
			commandArguments.push_back("-o");
			commandArguments.push_back(arguments.TargetFile.ToString());

			return commandArguments;
		}

		static std::vector<std::string> BuildLinkerArguments(
			const LinkArguments& arguments)
		{
			// Verify the input
			if (arguments.TargetFile.GetFileName().empty())
				throw std::runtime_error("Target file cannot be empty.");

			// Calculate object output file
			auto commandArguments = std::vector<std::string>();

			switch (arguments.TargetType)
			{
				case LinkTarget::StaticLibrary:
				{
					// Static libraries are linked with ar
					// r - Replace existing
					// c - Create without warning if does not exist
					commandArguments.push_back("rc");

					// Add the output file
					commandArguments.push_back(arguments.TargetFile.ToString());

					// Add the library files
					for (auto& file : arguments.LibraryFiles)
					{
						// Add the library files as input
						commandArguments.push_back(file.ToString());
					}

					// Add the object files
					for (auto& file : arguments.ObjectFiles)
					{
						// Add the object files as input
						commandArguments.push_back(file.ToString());
					}

					break;
				}
				case LinkTarget::DynamicLibrary:
				case LinkTarget::Executable:
				{
					// Dynamic libraries and executables targeting windows are put together by lld-link
					commandArguments = MSVC::ArgumentBuilder::BuildLinkerArguments(arguments);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown LinkTarget.");
				}
			}

			return commandArguments;
		}
	};
}
