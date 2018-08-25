// <copyright company="Soup" file="Compiler.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler.MSVC
{
	using System;
	using System.Diagnostics;
	using System.Threading.Tasks;

	public class Compiler : ICompiler
	{
		public Task ExecuteAsync(CompilerArguments args)
		{
			string compiler = @"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.15.26726\bin\Hostx64\x64\cl.exe";
			using (Process process = new Process())
			{
				var commandArgs = BuildCompilerArguments(args);
				Log.Message(commandArgs);

				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.FileName = compiler;
				process.StartInfo.WorkingDirectory = args.RootDirectory;
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

		private static string BuildCompilerArguments(CompilerArguments args)
		{
			// Start by disabling the logo output and enable two stage compile/link
			var commandArgs = "/nologo /c";
			
			// Lastly add the files
			commandArgs += string.Join(" ", args.SourceFiles);;

			return commandArgs;
		}

		private static void ProcessLine(string line)
		{
			Log.Message(line);
		}
	}
}
