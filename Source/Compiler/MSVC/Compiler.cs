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
				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.FileName = compiler;
				process.StartInfo.WorkingDirectory = args.WorkingDirectory;
				process.StartInfo.Arguments = $"{args.FileName}";
				process.Start();

				while (!process.StandardOutput.EndOfStream)
				{
					string line = process.StandardOutput.ReadLine();
					ProcessLine(line);
				}

				process.WaitForExit();

				if (process.ExitCode == 0)
				{
					throw new InvalidOperationException();
				}

				return Task.CompletedTask;
			}
		}

		private static void ProcessLine(string line)
		{
			Log.Message(line);
		}
	}
}
