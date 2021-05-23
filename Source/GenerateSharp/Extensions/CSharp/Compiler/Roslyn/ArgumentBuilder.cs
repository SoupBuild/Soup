// <copyright file="ArgumentBuilder.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build.CSharp.Compiler.Roslyn
{
	/// <summary>
	/// A helper class that builds the correct set of compiler arguments for a given
	/// set of options.
	/// </summary>
	public static class ArgumentBuilder
	{
		public static string ArgumentFlag_NoConfig => "noconfig";

		public static IList<string> BuildSharedCompilerArguments(CompileArguments arguments)
		{
			// Calculate object output file
			var commandArguments = new List<string>();

			// Disable 'unsafe' code
			AddFlag(commandArguments, "unsafe-");

			// Disable generate overflow checks
			AddFlag(commandArguments, "checked-");

			// Disable warnings
			if (arguments.DisabledWarnings.Count > 0)
			{
				var noWarnList = string.Join(",", arguments.DisabledWarnings);
				AddParameter(commandArguments, "nowarn", noWarnList);
			}

			// Generate fully qualified paths
			AddFlag(commandArguments, "fullpaths");

			// Do not reference standard library (mscorlib.dll)
			AddFlag(commandArguments, "nostdlib+");

			// Specify how to handle internal compiler errors
			AddParameter(commandArguments, "errorreport", "prompt");

			// Set the warning level
			AddParameter(commandArguments, "warn", "5");

			// Define conditional compilation symbol(s)
			if (arguments.PreprocessorDefinitions.Count > 0)
			{
				var definesList = string.Join(";", arguments.PreprocessorDefinitions);
				AddParameter(commandArguments, "define", definesList);
			}

			// Output line and column of the end location of each error
			AddFlag(commandArguments, "errorendlocation");

			// Specify the preferred output language name.
			AddParameter(commandArguments, "preferreduilang", "en-US");

			// Enable high-entropy ASLR
			AddFlag(commandArguments, "highentropyva+");

			// Specify nullable context option enabled
			AddParameter(commandArguments, "nullable", "enable");

			// Add the reference libraries
			foreach (var file in arguments.ReferenceLibraries)
			{
				AddParameterWithQuotes(commandArguments, "reference", file.ToString());
			}

			// Emit debugging information
			AddFlag(commandArguments, "debug+");
			AddParameter(commandArguments, "debug", "portable");

			// Specify the alignment used for output file sections
			AddParameter(commandArguments, "filealign", "512");

			// Enable optimizations
			if (arguments.EnableOptimizations)
				AddFlag(commandArguments, "optimize+");
			else
				AddFlag(commandArguments, "optimize-");

			// Specify output file name
			AddParameterWithQuotes(commandArguments, "out", arguments.Target.ToString());

			// Reference assembly output to generate
			AddParameterWithQuotes(commandArguments, "refout", arguments.Target.ToString());

			AddParameter(commandArguments, "target", "library");

			// Report all warnings as errors
			if (arguments.EnableWarningsAsErrors)
				AddFlag(commandArguments, "warnaserror+");
			else
				AddFlag(commandArguments, "warnaserror-");

			// Output compiler messages in UTF-8 encoding
			AddFlag(commandArguments, "utf8output");

			// Produce a deterministic assembly
			AddFlag(commandArguments, "deterministic+");

			// Specify language version
			AddParameter(commandArguments, "langversion", "9.0");

			// Add the source files
			foreach (var file in arguments.SourceFiles)
			{
				AddValueWithQuotes(commandArguments, file.ToString());
			}

			return commandArguments;
		}

		public static IList<string> BuildUniqueCompilerArguments()
		{
			// Calculate object output file
			var commandArguments = new List<string>();

			// Do not auto include CSC.RSP file
			AddFlag(commandArguments, ArgumentFlag_NoConfig);

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
