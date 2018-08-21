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
		public Task ExecuteAsync()
		{
			string compiler = "cl.exe";
			using (Process process = new Process())
			{
				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.FileName = compiler;
				process.StartInfo.Arguments = $"-C";
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
