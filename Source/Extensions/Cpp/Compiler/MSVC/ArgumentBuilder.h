// <copyright file="ArgumentBuilder.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::MSVC
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
		static constexpr std::string_view Compiler_ArgumentParameter_Experimental = "experimental";
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
		static constexpr std::string_view Linker_ArgumentValue_X86 = "X86";

	public:
		static std::vector<std::string> BuildSharedCompilerArguments(
			const SharedCompileArguments& arguments)
		{
			// Calculate object output file
			auto commandArguments = std::vector<std::string>();

			// Disable the logo
			AddFlag(commandArguments, ArgumentFlag_NoLogo);

			// Enable standards-conforming compiler behavior
			// https://docs.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance?view=vs-2019
			// Note: Enables /Zc:referenceBinding, /Zc:strictStrings, and /Zc:rvalueCast
			// And after 15.3 /Zc:ternary
			AddFlag(commandArguments, "permissive-");

			// Enable the __cplusplus macro to report the supported standard
			// https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=vs-2019
			AddParameter(commandArguments, "Zc", "__cplusplus");

			// Enable external linkage for constexpr variables
			// https://docs.microsoft.com/en-us/cpp/build/reference/zc-externconstexpr?view=vs-2019
			AddParameter(commandArguments, "Zc", "externConstexpr");

			// Remove unreferenced function or data if it is COMDAT or has internal linkage only
			// https://docs.microsoft.com/en-us/cpp/build/reference/zc-inline-remove-unreferenced-comdat?view=vs-2019
			AddParameter(commandArguments, "Zc", "inline");

			// Assume operator new throws on failure
			// https://docs.microsoft.com/en-us/cpp/build/reference/zc-throwingnew-assume-operator-new-throws?view=vs-2019
			AddParameter(commandArguments, "Zc", "throwingNew");

			// Generate source debug information
			if (arguments.GenerateSourceDebugInfo)
			{
				AddFlag(commandArguments, Compiler_ArgumentFlag_GenerateDebugInformation);
			}

			// Disabled individual warnings
			if (arguments.EnableWarningsAsErrors)
			{
				AddFlag(commandArguments, "WX");
			}

			AddFlag(commandArguments, "W4");

			// Disable any requested warnings
			for (auto& warning : arguments.DisabledWarnings)
			{
				AddFlagValue(commandArguments, "wd", warning);
			}

			// Enable any requested warnings
			for (auto& warning : arguments.EnabledWarnings)
			{
				AddFlagValue(commandArguments, "w", warning);
			}

			// Set the language standard
			switch (arguments.Standard)
			{
				case LanguageStandard::CPP11:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++11");
					break;
				case LanguageStandard::CPP14:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++14");
					break;
				case LanguageStandard::CPP17:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++17");
					break;
				case LanguageStandard::CPP20:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++latest");
					break;
				default:
					throw std::runtime_error("Unknown language standard.");
			}

			// Set the optimization level
			switch (arguments.Optimize)
			{
				case OptimizationLevel::None:
					AddFlag(commandArguments, Compiler_ArgumentFlag_Optimization_Disable);
					break;
				case OptimizationLevel::Speed:
					AddFlag(commandArguments, Compiler_ArgumentFlag_Optimization_Speed);
					break;
				case OptimizationLevel::Size:
					AddFlag(commandArguments, Compiler_ArgumentFlag_Optimization_Size);
					break;
				default:
					throw std::runtime_error("Unknown optimization level.");
			}

			// Set the include paths
			for (auto directory : arguments.IncludeDirectories)
			{
				AddFlagValueWithQuotes(commandArguments, Compiler_ArgumentParameter_Include, directory.ToString());
			}

			// Set the preprocessor definitions
			for (auto& definition : arguments.PreprocessorDefinitions)
			{
				AddFlagValue(commandArguments, Compiler_ArgumentParameter_PreprocessorDefine, definition);
			}

			// Ignore Standard Include Paths to prevent pulling in accidental headers
			AddFlag(commandArguments, Compiler_ArgumentFlag_IgnoreStandardIncludePaths);

			// Enable basic runtime checks
			AddFlag(commandArguments, Compiler_ArgumentFlag_RuntimeChecks);

			// Enable c++ exceptions
			AddFlag(commandArguments, "EHsc");

			// Enable multithreaded runtime static linked
			if (arguments.GenerateSourceDebugInfo)
			{
				AddFlag(commandArguments, Compiler_ArgumentFlag_Runtime_MultithreadedStatic_Debug);
			}
			else
			{
				AddFlag(commandArguments, Compiler_ArgumentFlag_Runtime_MultithreadedStatic_Release);
			}

			// Add the module references as input
			for (auto& moduleFile : arguments.IncludeModules)
			{
				AddFlag(commandArguments, "reference");
				AddValueWithQuotes(commandArguments, moduleFile.ToString());
			}

			// TODO: For now we allow exports to be large
			AddFlag(commandArguments, "bigobj");

			// Only run preprocessor, compile and assemble
			AddFlag(commandArguments, Compiler_ArgumentFlag_CompileOnly);

			return commandArguments;
		}

		static std::vector<std::string> BuildInterfaceUnitCompilerArguments(
			const InterfaceUnitCompileArguments& arguments,
			const Path& responseFile)
		{
			// Build the arguments for a standard translation unit
			auto commandArguments = std::vector<std::string>();

			// Add the response file
			commandArguments.push_back("@" + responseFile.ToString());

			// Add the source file as input
			commandArguments.push_back(arguments.SourceFile.ToString());

			// Add the target file as outputs
			AddFlagValueWithQuotes(commandArguments, Compiler_ArgumentParameter_ObjectFile, arguments.TargetFile.ToString());

			// Add the unique arguments for an interface unit
			AddFlag(commandArguments, "interface");

			// Specify the module interface file output
			AddFlag(commandArguments, "ifcOutput");
			AddValueWithQuotes(commandArguments, arguments.ModuleInterfaceTarget.ToString());

			return commandArguments;
		}

		static std::vector<std::string> BuildTranslationUnitCompilerArguments(
			const TranslationUnitCompileArguments& arguments,
			const Path& responseFile,
			const std::vector<Path>& internalModules)
		{
			// Calculate object output file
			auto commandArguments = std::vector<std::string>();

			// Add the response file
			commandArguments.push_back("@" + responseFile.ToString());

			// Add the internal module references as input
			for (auto& moduleFile : internalModules)
			{
				AddFlag(commandArguments, "reference");
				AddValueWithQuotes(commandArguments, moduleFile.ToString());
			}

			// Add the source file as input
			commandArguments.push_back(arguments.SourceFile.ToString());

			// Add the target file as outputs
			AddFlagValueWithQuotes(commandArguments, Compiler_ArgumentParameter_ObjectFile, arguments.TargetFile.ToString());

			return commandArguments;
		}

		static std::vector<std::string> BuildLinkerArguments(
			const LinkArguments& arguments)
		{
			// Verify the input
			if (arguments.TargetFile.GetFileName().empty())
				throw std::runtime_error("Target file cannot be empty.");

			auto commandArguments = std::vector<std::string>();

			// Disable the logo
			AddFlag(commandArguments, ArgumentFlag_NoLogo);

			// Disable the default libraries, we will set this up
			// AddFlag(commandArguments, Linker_ArgumentFlag_NoDefaultLibraries);

			// Enable verbose output
			// AddFlag(commandArguments, Linker_ArgumentFlag_Verbose);

			// Generate source debug information
			if (arguments.GenerateSourceDebugInfo)
			{
				AddParameter(commandArguments, "debug", "full");
			}

			// Calculate object output file
			switch (arguments.TargetType)
			{
				case LinkTarget::StaticLibrary:
				{
					break;
				}
				case LinkTarget::DynamicLibrary:
				{
					// TODO: May want to specify the exact value
					// set the default lib to mutlithreaded
					// AddParameter(commandArguments, "defaultlib", "libcmt");
					AddParameter(commandArguments, "subsystem", "console");

					// Create a dynamic library
					AddFlag(commandArguments, Linker_ArgumentFlag_DLL);

					// Set the output implementation library
					AddParameterWithQuotes(
						commandArguments,
						Linker_ArgumentParameter_ImplementationLibrary,
						arguments.ImplementationFile.ToString());

					break;
				}
				case LinkTarget::Executable:
				{
					// TODO: May want to specify the exact value
					// set the default lib to multithreaded
					// AddParameter(commandArguments, "defaultlib", "libcmt");
					AddParameter(commandArguments, "subsystem", "console");

					break;
				}
				default:
				{
					throw std::runtime_error("Unknown LinkTarget.");
				}
			}

			// Add the machine target
			if (arguments.TargetArchitecture == "x64")
				AddParameter(commandArguments, Linker_ArgumentParameter_Machine, Linker_ArgumentValue_X64);
			else if (arguments.TargetArchitecture == "x86")
				AddParameter(commandArguments, Linker_ArgumentParameter_Machine, Linker_ArgumentValue_X86);
			else
				throw std::runtime_error("Unknown target architecture.");

			// Set the library paths
			for (auto directory : arguments.LibraryPaths)
			{
				AddParameterWithQuotes(commandArguments, Linker_ArgumentParameter_LibraryPath, directory.ToString());
			}

			// Add the target as an output
			AddParameterWithQuotes(commandArguments, Linker_ArgumentParameter_Output, arguments.TargetFile.ToString());

			// Add the library files
			for (auto& file : arguments.LibraryFiles)
			{
				// Add the library files as input
				commandArguments.push_back(file.ToString());
			}

			// Add the external libraries as default libraries so they are resolved last
			for (auto& file : arguments.ExternalLibraryFiles)
			{
				// Add the external library files as input
				// TODO: Explicitly ignore these files from the input for now
				AddParameter(commandArguments, Linker_ArgumentParameter_DefaultLibrary, file.ToString());
			}

			// Add the object files
			for (auto& file : arguments.ObjectFiles)
			{
				// Add the object files as input
				commandArguments.push_back(file.ToString());
			}

			return commandArguments;
		}

	private:
		static void AddValueWithQuotes(
			std::vector<std::string>& arguments,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "\"" << value << "\"";
			arguments.push_back(builder.str());
		}

		static void AddFlag(std::vector<std::string>& arguments, std::string_view flag)
		{
			auto builder = std::stringstream();
			builder << "/" << flag;
			arguments.push_back(builder.str());
		}

		static void AddFlagValue(
			std::vector<std::string>& arguments,
			std::string_view flag,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "/" << flag << value;
			arguments.push_back(builder.str());
		}

		static void AddFlagValueWithQuotes(
			std::vector<std::string>& arguments,
			std::string_view flag,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "/" << flag << "\"" << value << "\"";
			arguments.push_back(builder.str());
		}

		static void AddParameter(
			std::vector<std::string>& arguments,
			std::string_view name,
			std::string_view value)
		{
			auto builder = std::stringstream();
			builder << "/" << name << ":" << value;
			arguments.push_back(builder.str());
		}

		static void AddParameterWithQuotes(
			std::vector<std::string>& arguments,
			std::string_view name,
			std::string_view value)
		{
			auto builder = std::stringstream();
			builder << "/" << name << ":\"" << value << "\"";
			arguments.push_back(builder.str());
		}
	};
}
