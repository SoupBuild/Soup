// <copyright file="ArgumentBuilder.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Cpp.Compiler.MSVC
{
	/// <summary>
	/// A helper class that builds the correct set of compiler arguments for a given
	/// set of options.
	/// </summary>
	public static class ArgumentBuilder
	{
		public static string ArgumentFlag_NoLogo => "nologo";

		public static string Compiler_ArgumentFlag_GenerateDebugInformation => "Z7";
		public static string Compiler_ArgumentFlag_GenerateDebugInformationExternal => "Zi";
		public static string Compiler_ArgumentFlag_CompileOnly => "c";
		public static string Compiler_ArgumentFlag_IgnoreStandardIncludePaths => "X";
		public static string Compiler_ArgumentFlag_Optimization_Disable => "Od";
		public static string Compiler_ArgumentFlag_Optimization_Speed => "Ot";
		public static string Compiler_ArgumentFlag_Optimization_Size => "Os";
		public static string Compiler_ArgumentFlag_RuntimeChecks => "RTC1";
		public static string Compiler_ArgumentFlag_Runtime_MultithreadedDynamic_Debug => "MDd";
		public static string Compiler_ArgumentFlag_Runtime_MultithreadedDynamic_Release => "MD";
		public static string Compiler_ArgumentFlag_Runtime_MultithreadedStatic_Debug => "MTd";
		public static string Compiler_ArgumentFlag_Runtime_MultithreadedStatic_Release => "MT";
		public static string Compiler_ArgumentParameter_Standard => "std";
		public static string Compiler_ArgumentParameter_Experimental => "experimental";
		public static string Compiler_ArgumentParameter_ObjectFile => "Fo";
		public static string Compiler_ArgumentParameter_Include => "I";
		public static string Compiler_ArgumentParameter_PreprocessorDefine => "D";

		public static string Linker_ArgumentFlag_NoDefaultLibraries => "nodefaultlib";
		public static string Linker_ArgumentFlag_DLL => "dll";
		public static string Linker_ArgumentFlag_Verbose => "verbose";
		public static string Linker_ArgumentParameter_Output => "out";
		public static string Linker_ArgumentParameter_ImplementationLibrary => "implib";
		public static string Linker_ArgumentParameter_LibraryPath => "libpath";
		public static string Linker_ArgumentParameter_Machine => "machine";
		public static string Linker_ArgumentParameter_DefaultLibrary => "defaultlib";
		public static string Linker_ArgumentValue_X64 => "X64";
		public static string Linker_ArgumentValue_X86 => "X86";

		public static IList<string> BuildSharedCompilerArguments(
			SharedCompileArguments arguments)
		{
			// Calculate object output file
			var commandArguments = new List<string>();

			// Disable the logo
			AddFlag(commandArguments, ArgumentFlag_NoLogo);

			// Enable full paths for errors
			AddFlag(commandArguments, "FC");

			// Enable standards-conforming compiler behavior
			// https://docs.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance?view=vs-2019
			// Note: Enables /Zc:referenceBinding, /Zc:strictStrings, and /Zc:rvalueCast
			// And after 15.3 /Zc:ternary
			AddFlag(commandArguments, "permissive-");

			// Enable the __cplusplus macro to report the supported standard
			// https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=vs-2019
			bool disableCPlusPlusMacroConformance = arguments.CustomProperties.Contains("DisableCPlusPlusMacroConformance");
			if (!disableCPlusPlusMacroConformance)
			{
				AddParameter(commandArguments, "Zc", "__cplusplus");
			}

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
			foreach (var warning in arguments.DisabledWarnings)
			{
				AddFlagValue(commandArguments, "wd", warning);
			}

			// Enable any requested warnings
			foreach (var warning in arguments.EnabledWarnings)
			{
				AddFlagValue(commandArguments, "w", warning);
			}

			// Set the language standard
			switch (arguments.Standard)
			{
				case LanguageStandard.CPP11:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++11");
					break;
				case LanguageStandard.CPP14:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++14");
					break;
				case LanguageStandard.CPP17:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++17");
					break;
				case LanguageStandard.CPP20:
					AddParameter(commandArguments, Compiler_ArgumentParameter_Standard, "c++latest");
					break;
				default:
					throw new InvalidOperationException("Unknown language standard.");
			}

			// Set the optimization level
			switch (arguments.Optimize)
			{
				case OptimizationLevel.None:
					AddFlag(commandArguments, Compiler_ArgumentFlag_Optimization_Disable);
					break;
				case OptimizationLevel.Speed:
					AddFlag(commandArguments, Compiler_ArgumentFlag_Optimization_Speed);
					break;
				case OptimizationLevel.Size:
					AddFlag(commandArguments, Compiler_ArgumentFlag_Optimization_Size);
					break;
				default:
					throw new InvalidOperationException("Unknown optimization level.");
			}

			// Set the include paths
			foreach (var directory in arguments.IncludeDirectories)
			{
				AddFlagValueWithQuotes(commandArguments, Compiler_ArgumentParameter_Include, directory.ToString());
			}

			// Set the preprocessor definitions
			foreach (var definition in arguments.PreprocessorDefinitions)
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
			foreach (var moduleFile in arguments.IncludeModules)
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

		public static IList<string> BuildResourceCompilerArguments(
			Path targetRootDirectory,
			SharedCompileArguments arguments)
		{
			if (arguments.ResourceFile == null)
				throw new ArgumentNullException(nameof(arguments));

			// Build the arguments for a standard translation unit
			var commandArguments = new List<string>();

			// Disable the logo
			AddFlag(commandArguments, ArgumentFlag_NoLogo);

			// TODO: Defines?
			AddFlagValue(commandArguments, Compiler_ArgumentParameter_PreprocessorDefine, "_UNICODE");
			AddFlagValue(commandArguments, Compiler_ArgumentParameter_PreprocessorDefine, "UNICODE");

			// Specify default language using language identifier
			AddFlagValueWithQuotes(commandArguments, "l", "0x0409");

			// Set the include paths
			foreach (var directory in arguments.IncludeDirectories)
			{
				AddFlagValueWithQuotes(commandArguments, Compiler_ArgumentParameter_Include, directory.ToString());
			}

			// Add the target file as outputs
			var absoluteTargetFile = targetRootDirectory + arguments.ResourceFile.TargetFile;
			AddFlagValueWithQuotes(
				commandArguments,
				Compiler_ArgumentParameter_ObjectFile,
				absoluteTargetFile.ToString());

			// Add the source file as input
			commandArguments.Add(arguments.ResourceFile.SourceFile.ToString());

			return commandArguments;
		}

		public static IList<string> BuildInterfaceUnitCompilerArguments(
			Path targetRootDirectory,
			InterfaceUnitCompileArguments arguments,
			Path responseFile)
		{
			// Build the arguments for a standard translation unit
			var commandArguments = new List<string>();

			// Add the response file
			commandArguments.Add("@" + responseFile.ToString());

			// Add the source file as input
			commandArguments.Add(arguments.SourceFile.ToString());

			// Add the target file as outputs
			var absoluteTargetFile = targetRootDirectory + arguments.TargetFile;
			AddFlagValueWithQuotes(
				commandArguments,
				Compiler_ArgumentParameter_ObjectFile,
				absoluteTargetFile.ToString());

			// Add the unique arguments for an interface unit
			AddFlag(commandArguments, "interface");

			// Specify the module interface file output
			AddFlag(commandArguments, "ifcOutput");

			var absoluteModuleInterfaceFile = targetRootDirectory + arguments.ModuleInterfaceTarget;
			AddValueWithQuotes(commandArguments, absoluteModuleInterfaceFile.ToString());

			return commandArguments;
		}

		public static IList<string> BuildTranslationUnitCompilerArguments(
			Path targetRootDirectory,
			TranslationUnitCompileArguments arguments,
			Path responseFile,
			IList<Path> internalModules)
		{
			// Calculate object output file
			var commandArguments = new List<string>();

			// Add the response file
			commandArguments.Add("@" + responseFile.ToString());

			// Add the internal module references as input
			foreach (var moduleFile in internalModules)
			{
				AddFlag(commandArguments, "reference");
				AddValueWithQuotes(commandArguments, moduleFile.ToString());
			}

			// Add the source file as input
			commandArguments.Add(arguments.SourceFile.ToString());

			// Add the target file as outputs
			var absoluteTargetFile = targetRootDirectory + arguments.TargetFile;
			AddFlagValueWithQuotes(
				commandArguments,
				Compiler_ArgumentParameter_ObjectFile,
				absoluteTargetFile.ToString());

			return commandArguments;
		}

		public static IList<string> BuildLinkerArguments(
			LinkArguments arguments)
		{
			// Verify the input
			if (string.IsNullOrEmpty(arguments.TargetFile.GetFileName()))
				throw new InvalidOperationException("Target file cannot be empty.");

			var commandArguments = new List<string>();

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
				case LinkTarget.StaticLibrary:
				{
					break;
				}
				case LinkTarget.DynamicLibrary:
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
				case LinkTarget.Executable:
				{
					// TODO: May want to specify the exact value
					// set the default lib to multithreaded
					// AddParameter(commandArguments, "defaultlib", "libcmt");
					AddParameter(commandArguments, "subsystem", "console");

					break;
				}
				case LinkTarget.WindowsApplication:
				{
					// TODO: May want to specify the exact value
					// set the default lib to multithreaded
					// AddParameter(commandArguments, "defaultlib", "libcmt");
					AddParameter(commandArguments, "subsystem", "windows");

					break;
				}
				default:
				{
					throw new InvalidOperationException("Unknown LinkTarget.");
				}
			}

			// Add the machine target
			if (arguments.TargetArchitecture == "x64")
				AddParameter(commandArguments, Linker_ArgumentParameter_Machine, Linker_ArgumentValue_X64);
			else if (arguments.TargetArchitecture == "x86")
				AddParameter(commandArguments, Linker_ArgumentParameter_Machine, Linker_ArgumentValue_X86);
			else
				throw new InvalidOperationException("Unknown target architecture.");

			// Set the library paths
			foreach (var directory in arguments.LibraryPaths)
			{
				AddParameterWithQuotes(commandArguments, Linker_ArgumentParameter_LibraryPath, directory.ToString());
			}

			// Add the target as an output
			AddParameterWithQuotes(commandArguments, Linker_ArgumentParameter_Output, arguments.TargetFile.ToString());

			// Add the library files
			foreach (var file in arguments.LibraryFiles)
			{
				// Add the library files as input
				commandArguments.Add(file.ToString());
			}

			// Add the external libraries as default libraries so they are resolved last
			foreach (var file in arguments.ExternalLibraryFiles)
			{
				// Add the external library files as input
				// TODO: Explicitly ignore these files from the input for now
				AddParameter(commandArguments, Linker_ArgumentParameter_DefaultLibrary, file.ToString());
			}

			// Add the object files
			foreach (var file in arguments.ObjectFiles)
			{
				// Add the object files as input
				commandArguments.Add(file.ToString());
			}

			return commandArguments;
		}

		private static void AddValueWithQuotes(
			IList<string> arguments,
			string value)
		{
			arguments.Add($"\"{value}\"");
		}

		private static void AddFlag(IList<string> arguments, string flag)
		{
			arguments.Add($"/{flag}");
		}

		private static void AddFlagValue(
			IList<string> arguments,
			string flag,
			string value)
		{
			arguments.Add($"/{flag}{value}");
		}

		private static void AddFlagValueWithQuotes(
			IList<string> arguments,
			string flag,
			string value)
		{
			arguments.Add($"/{flag}\"{value}\"");
		}

		private static void AddParameter(
			IList<string> arguments,
			string name,
			string value)
		{
			arguments.Add($"/{name}:{value}");
		}

		private static void AddParameterWithQuotes(
			IList<string> arguments,
			string name,
			string value)
		{
			arguments.Add($"/{name}:\"{value}\"");
		}
	}
}
