// <copyright company="Soup" file="MakeBuildRunner.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Make
{
	using System;
	using System.Collections.Generic;
	using System.Diagnostics;
	using System.IO;
	using System.Text;
	using System.Threading.Tasks;
	using System.Xml.Serialization;

	/// <summary>
	/// The Make Build Runner
	/// </summary>
	internal static class MakeBuildRunner
	{
		public static bool Build(string buildPath, bool showOutput, bool debug)
		{
			string compiler = "make";
			var makeFilePath = buildPath;
			using (Process process = new Process())
			{
				process.StartInfo.UseShellExecute = false;
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.FileName = compiler;
				process.StartInfo.Arguments = $"-C {makeFilePath}";
				process.Start();

				if (showOutput)
				{
					while (!process.StandardOutput.EndOfStream)
					{
						string line = process.StandardOutput.ReadLine();
						WriteBuildLine(line);
					}
				}

				process.WaitForExit();

				return process.ExitCode == 0;
			}
		}

		private static void WriteBuildLine(string line)
		{
			Log.Message(line);
		}
	}
}
