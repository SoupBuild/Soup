// <copyright company="Soup" file="Compiler.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler.GCC
{
	using System;
	using System.Collections.Generic;
	using System.Diagnostics;
	using System.Threading.Tasks;

	public class Compiler : ICompiler
	{
		public Task CompileAsync(CompilerArguments args)
		{
			string compiler = "gcc";
			using (Process process = new Process())
			{
				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.FileName = compiler;
				process.StartInfo.WorkingDirectory = args.RootDirectory;
				process.StartInfo.Arguments = BuildCompilerArguments(args);
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
			return Task.CompletedTask;
		}

		private static string BuildCompilerArguments(CompilerArguments args)
		{
			var commandArgs = new List<string>();
			
			// Set the Standard Library implementation
			commandArgs.Add("-stdlib=libc++");
			
			// Set the language version
			commandArgs.Add("-std=c++1z");
			
			// Enable experimental modules
			commandArgs.Add("-fmodules-ts");

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
