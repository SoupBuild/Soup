// <copyright company="Soup" file="Compiler.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler.MSVC
{
	using System;
	using System.Collections.Generic;
	using System.Diagnostics;
	using System.IO;
	using System.Threading.Tasks;

	public class Compiler : ICompiler
	{
		public Task CompileAsync(CompilerArguments args)
		{
			// Set the working directory to the output directory
			var workingDirectory = args.RootDirectory;

			string compiler = @"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.15.26726\bin\Hostx64\x64\cl.exe";
			var commandArgs = BuildCompilerArguments(args);

			Log.Info($"{compiler} {commandArgs}");

			using (Process process = new Process())
			{
				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.FileName = compiler;
				process.StartInfo.WorkingDirectory = workingDirectory;
				process.StartInfo.Arguments = commandArgs;
				process.Start();

				while (!process.StandardOutput.EndOfStream)
				{
					string line = process.StandardOutput.ReadLine();
					ProcessLine(line);
				}

				process.WaitForExit();

				if (process.ExitCode != 0)
				{
					throw new InvalidOperationException();
				}

				return Task.CompletedTask;
			}
		}

		public Task LinkAsync(LinkerArguments args)
		{
			// Set the working directory to the output directory
			var workingDirectory = args.RootDirectory;

			string linker = @"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.15.26726\bin\Hostx64\x64\lib.exe";
			var linkerArgs = BuildLinkerArguments(args);

			Log.Info($"{linker} {linkerArgs}");

			using (Process process = new Process())
			{
				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.FileName = linker;
				process.StartInfo.WorkingDirectory = workingDirectory;
				process.StartInfo.Arguments = linkerArgs;
				process.Start();

				while (!process.StandardOutput.EndOfStream)
				{
					string line = process.StandardOutput.ReadLine();
					ProcessLine(line);
				}

				process.WaitForExit();

				if (process.ExitCode != 0)
				{
					throw new InvalidOperationException();
				}

				return Task.CompletedTask;
			}
		}

		private static string BuildCompilerArguments(CompilerArguments args)
		{
			var commandArgs = new List<string>();

			// Disable the logo text
			commandArgs.Add("-nologo");

			// Set the language standard
			switch (args.Standard)
			{
				case LanguageStandard.CPP11:
					throw new NotSupportedException("11 is not supported any longer. 14 is not the default.");
				case LanguageStandard.CPP14:
					// Default value commandArgs.Add("-std:c++14");
					break;
				case LanguageStandard.CPP17:
					commandArgs.Add("-std:c++17");
					break;
				case LanguageStandard.Latest:
					commandArgs.Add("-std:c++latest");
					break;
				default:
					throw new NotSupportedException("Unknown language standard.");
			}
			
			// Only run preprocess, compile, and assemble steps
			commandArgs.Add("-c");

			// Add the object output file
			var objectPath = args.OutputDirectory.EnsureTrailingSlash().Replace(@"\", @"\\");
			commandArgs.Add($"-Fo\"{objectPath}\"");

			// Enable experimental features
			if (args.Standard == LanguageStandard.Latest)
			{
				commandArgs.Add("-experimental:module");
			}

			// Add the module references
			foreach (var module in args.Modules)
			{
				commandArgs.Add("-module:reference");
				commandArgs.Add(module);
			}

			if (args.ExportModule)
			{
				commandArgs.Add("-module:export");

				// There must be only one source file
				if (args.SourceFiles.Count != 1)
				{
					throw new ArgumentException("Export module expects only one source file.");
				}

				// Place the ifc in the output directory
				var sourceFile = args.SourceFiles[0];
				var outputFile = $"{args.OutputDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(sourceFile)}.ifc";
				commandArgs.AddRange(new string[] { "-module:output", outputFile });
			}

			// Lastly add the file
			commandArgs.AddRange(args.SourceFiles);

			return string.Join(" ", commandArgs);
		}

		private static string BuildLinkerArguments(LinkerArguments args)
		{
			var commandArgs = new List<string>();

			// Disable the logo text
			commandArgs.Add("-nologo");

			// Add the library output file
			var ouputPath = args.OutputDirectory.EnsureTrailingSlash().Replace(@"\", @"\\");
			commandArgs.Add($"-out:\"{ouputPath}{args.Name}.lib\"");

			// Lastly add the file
			commandArgs.AddRange(args.SourceFiles);

			return string.Join(" ", commandArgs);
		}

		private static void ProcessLine(string line)
		{
			Log.Info(line);
		}
	}
}
