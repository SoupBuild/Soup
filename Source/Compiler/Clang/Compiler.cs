// <copyright company="Soup" file="Compiler.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler.Clang
{
	using System;
	using System.Collections.Generic;
	using System.Diagnostics;
	using System.IO;
	using System.Linq;
	using System.Threading.Tasks;

	public class Compiler : ICompiler
	{
		public Task ExecuteAsync(CompilerArguments args)
		{
			// Set the working directory to the output directory
			var workingDirectory = Path.Combine(args.RootDirectory, args.ObjectDirectory);

			string compiler = "/usr/bin/clang++";
			var commandArgs = BuildCompilerArguments(workingDirectory, args);

			Log.Message($"{compiler} {commandArgs}");
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

		private static string BuildCompilerArguments(string workingDirectory, CompilerArguments args)
		{
			var commandArgs = new List<string>();
			
			// Only run preprocess, compile, and assemble steps
			commandArgs.Add("-c"); // --compile

			// Set the Standard Library implementation
			commandArgs.Add("-stdlib=libc++");
			
			// Set the language version
			commandArgs.Add("-std=c++1z");
			
			// Enable experimental modules
			commandArgs.Add("-fmodules-ts");

			// Precompile only
			commandArgs.Add("-o");
			commandArgs.Add("MultiVersion.ifc");

			// Lastly add the files
			var relativePath = Path.GetRelativePath(workingDirectory, args.RootDirectory);
			commandArgs.AddRange(args.SourceFiles.Select(value => relativePath + value));

			return string.Join(" ", commandArgs);
		}

		private static void ProcessLine(string line)
		{
			Log.Message(line);
		}
	}
}
