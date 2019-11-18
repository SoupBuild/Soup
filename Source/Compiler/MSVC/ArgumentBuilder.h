// <copyright file="ArgumentBuilder.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::MSVC
{
	/// <summary>
	/// A helper class that builds the correct set of compiler arguments for a given
	/// set of options.
	/// </summary>
	export class ArgumentBuilder
	{
	private:
		static constexpr std::string_view ArgumentFlag_NoLogo = "nologo";

		static constexpr std::string_view Compiler_ArgumentFlag_GenerateDebugInformation = "Z7";
		static constexpr std::string_view Compiler_ArgumentFlag_GenerateDebugInformationExternal = "Zi";
		static constexpr std::string_view Compiler_ArgumentFlag_ShowIncludes = "showIncludes";
		static constexpr std::string_view Compiler_ArgumentFlag_CompileOnly = "c";
		static constexpr std::string_view Compiler_ArgumentFlag_IgnoreStandardIncludePaths = "X";
		static constexpr std::string_view Compiler_ArgumentFlag_Optimization_Disable = "Od";
		static constexpr std::string_view Compiler_ArgumentFlag_Optimization_Speed = "Ot";
		static constexpr std::string_view Compiler_ArgumentFlag_Optimization_Size = "Os";
		static constexpr std::string_view Compiler_ArgumentFlag_RuntimeChecks = "RTC1";
		static constexpr std::string_view Compiler_ArgumentFlag_Runtime_MultithreadedDynamic_Debug = "MDd";
		static constexpr std::string_view Compiler_ArgumentFlag_Runtime_MultithreadedDynamic_Release = "MD";
		static constexpr std::string_view Compiler_ArgumentFlag_Runtime_MultithreadedStatic_Debug = "MTd";
		static constexpr std::string_view Compiler_ArgumentFlag_Runtime_MultithreadedStatic_Release = "MT";
		static constexpr std::string_view Compiler_ArgumentParameter_Standard = "std";
		static constexpr std::string_view Compiler_ArgumentParameter_ObjectFile = "Fo";
		static constexpr std::string_view Compiler_ArgumentParameter_Include = "I";
		static constexpr std::string_view Compiler_ArgumentParameter_PreprocessorDefine = "D";

		static constexpr std::string_view Linker_ArgumentFlag_NoDefaultLibraries = "nodefaultlib";
		static constexpr std::string_view Linker_ArgumentFlag_DLL = "dll";
		static constexpr std::string_view Linker_ArgumentFlag_Verbose = "verbose";
		static constexpr std::string_view Linker_ArgumentParameter_Output = "out";
		static constexpr std::string_view Linker_ArgumentParameter_ImplementationLibrary = "implib";
		static constexpr std::string_view Linker_ArgumentParameter_LibraryPath = "libpath";
		static constexpr std::string_view Linker_ArgumentParameter_Machine = "machine";
		static constexpr std::string_view Linker_ArgumentParameter_DefaultLibrary = "defaultlib";
		static constexpr std::string_view Linker_ArgumentValue_X64 = "X64";

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

			// Disable the logo
			AddFlag(commandArgs, ArgumentFlag_NoLogo);

			// Enable Header includes if needed
			if (args.GenerateIncludeTree)
			{
				AddFlag(commandArgs, Compiler_ArgumentFlag_ShowIncludes);
			}

			// Generate source debug information
			if (args.GenerateSourceDebugInfo)
			{
				AddFlag(commandArgs, Compiler_ArgumentFlag_GenerateDebugInformation);
			}

			// Set the language standard
			switch (args.Standard)
			{
				case LanguageStandard::CPP11:
					AddParameter(commandArgs, Compiler_ArgumentParameter_Standard, "c++11");
					break;
				case LanguageStandard::CPP14:
					AddParameter(commandArgs, Compiler_ArgumentParameter_Standard, "c++14");
					break;
				case LanguageStandard::CPP17:
					AddParameter(commandArgs, Compiler_ArgumentParameter_Standard, "c++17");
					break;
				case LanguageStandard::CPP20:
					AddParameter(commandArgs, Compiler_ArgumentParameter_Standard, "c++latest");
					break;
				default:
					throw std::runtime_error("Unknown language standard.");
			}

			// Set the optimization level
			switch (args.Optimize)
			{
				case OptimizationLevel::None:
					AddFlag(commandArgs, Compiler_ArgumentFlag_Optimization_Disable);
					break;
				case OptimizationLevel::Speed:
					AddFlag(commandArgs, Compiler_ArgumentFlag_Optimization_Speed);
					break;
				case OptimizationLevel::Size:
					AddFlag(commandArgs, Compiler_ArgumentFlag_Optimization_Size);
					break;
				default:
					throw std::runtime_error("Unknown optimization level.");
			}

			// Set the include paths
			for (auto directory : args.IncludeDirectories)
			{
				AddFlagValueWithQuotes(commandArgs, Compiler_ArgumentParameter_Include, directory.ToString());
			}

			// Set the preprocessor definitions
			for (auto& definition : args.PreprocessorDefinitions)
			{
				AddFlagValue(commandArgs, Compiler_ArgumentParameter_PreprocessorDefine, definition);
			}

			// Ignore Standard Include Paths to prevent pulling in accidental headers
			AddFlag(commandArgs, Compiler_ArgumentFlag_IgnoreStandardIncludePaths);

			// Enable basic runtime checks
			AddFlag(commandArgs, Compiler_ArgumentFlag_RuntimeChecks);

			// Enable c++ exceptions
			AddFlag(commandArgs, "EHsc");

			// Enable multithreaded runtime dynamic linked
			if (args.Optimize == OptimizationLevel::None)
			{
				AddFlag(commandArgs, Compiler_ArgumentFlag_Runtime_MultithreadedDynamic_Debug);
			}
			else
			{
				AddFlag(commandArgs, Compiler_ArgumentFlag_Runtime_MultithreadedDynamic_Release);
			}

			// Add the module references
			for (auto& moduleFile : args.IncludeModules)
			{
				// auto argument = "-fmodule-file=\"" + moduleFile.ToString() + "\"";
				// commandArgs.push_back(std::move(argument));
			}

			if (args.ExportModule)
			{
				// commandArgs.push_back("/clang:--precompile");

				// Place the ifc in the output directory
				//var outputFile = "{Path.GetFileNameWithoutExtension(sourceFile)}.{ModuleFileExtension}";
				//commandArgs.AddRange(new string[] { "-o", outputFile });
			}
			else
			{
				// Only run preprocessor, compile and assemble
				AddFlag(commandArgs, Compiler_ArgumentFlag_CompileOnly);
			}

			// Add the source file
			commandArgs.push_back(args.SourceFile.ToString());

			// Add the target file
			AddFlagValueWithQuotes(commandArgs, Compiler_ArgumentParameter_ObjectFile, args.TargetFile.ToString());

			return commandArgs;
		}

		static std::vector<std::string> BuildLinkerArguments(const LinkArguments& args)
		{
			// Verify the input
			if (args.TargetFile.GetFileName().empty())
				throw std::runtime_error("Target file cannot be empty.");

			auto commandArgs = std::vector<std::string>();

			// Disable the logo
			AddFlag(commandArgs, ArgumentFlag_NoLogo);

			// Disable the default libraries, we will set this up
			// AddFlag(commandArgs, Linker_ArgumentFlag_NoDefaultLibraries);

			// Enable verbose output
			// AddFlag(commandArgs, Linker_ArgumentFlag_Verbose);

			// Calculate object output file
			switch (args.TargetType)
			{
				case LinkTarget::StaticLibrary:
				{
					break;
				}
				case LinkTarget::DynamicLibrary:
				{
					// TODO: May want to specify the exact value
					// set the default lib to mutlithreaded
					// AddParameter(commandArgs, "defaultlib", "libcmt");
					AddParameter(commandArgs, "subsystem", "console");

					// Create a dynamic library
					AddFlag(commandArgs, Linker_ArgumentFlag_DLL);

					// Todo: this does not seem like the correct location for this
					auto implemenationLibraryfile = args.TargetFile;
					implemenationLibraryfile.SetFileExtension("lib");
					AddParameterWithQuotes(
						commandArgs,
						Linker_ArgumentParameter_ImplementationLibrary,
						implemenationLibraryfile.ToString());

					break;
				}
				case LinkTarget::Executable:
				{
					// TODO: May want to specify the exact value
					// set the default lib to mutlithreaded
					// AddParameter(commandArgs, "defaultlib", "libcmt");
					AddParameter(commandArgs, "subsystem", "console");

					break;
				}
				default:
				{
					throw std::runtime_error("Unknown LinkTarget.");
				}
			}

			// Add the machine target
			AddParameter(commandArgs, Linker_ArgumentParameter_Machine, Linker_ArgumentValue_X64);

			// Set the library paths
			for (auto directory : args.LibraryPaths)
			{
				AddParameterWithQuotes(commandArgs, Linker_ArgumentParameter_LibraryPath, directory.ToString());
			}

			AddParameterWithQuotes(commandArgs, Linker_ArgumentParameter_Output, args.TargetFile.ToString());

			// Add the library files
			for (auto& file : args.LibraryFiles)
			{
				commandArgs.push_back(file.ToString());
			}

			// Add the external libraries as default libraries so they are resolved last
			for (auto& file : args.ExternalLibraryFiles)
			{
				AddParameter(commandArgs, Linker_ArgumentParameter_DefaultLibrary, file.ToString());
			}

			// Add the object files
			for (auto& file : args.ObjectFiles)
			{
				commandArgs.push_back(file.ToString());
			}

			return commandArgs;
		}

	private:
		static void AddFlag(std::vector<std::string>& args, std::string_view flag)
		{
			auto builder = std::stringstream();
			builder << "/" << flag;
			args.push_back(builder.str());
		}

		static void AddFlagValue(
			std::vector<std::string>& args,
			std::string_view flag,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "/" << flag << value;
			args.push_back(builder.str());
		}

		static void AddFlagValueWithQuotes(
			std::vector<std::string>& args,
			std::string_view flag,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "/" << flag << "\"" << value << "\"";
			args.push_back(builder.str());
		}

		static void AddParameter(
			std::vector<std::string>& args,
			std::string_view name,
			std::string_view value)
		{
			auto builder = std::stringstream();
			builder << "/" << name << ":" << value;
			args.push_back(builder.str());
		}

		static void AddParameterWithQuotes(
			std::vector<std::string>& args,
			std::string_view name,
			std::string_view value)
		{
			auto builder = std::stringstream();
			builder << "/" << name << ":\"" << value << "\"";
			args.push_back(builder.str());
		}
	};
}
