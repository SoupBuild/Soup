// <copyright file="VSWhereUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Opal;
using Opal.System;

namespace Soup.Build.Discover
{
	class VSWhereUtilities
	{
		/// <summary>
		/// Attempt to find Roslyn compiler installation 
		/// </summary>
		public static async Task<Path> FindRoslynInstallAsync()
		{
			// Find the location of the Windows SDK
			var visualStudioInstallRoot = await FindVSInstallRootAsync("Microsoft.VisualStudio.Component.Roslyn.Compiler");
			Log.HighPriority("Using VS Installation: " + visualStudioInstallRoot.ToString());

			// Calculate the final Roslyn binaries folder
			var roslynFolder =
				visualStudioInstallRoot + new Path("/MSBuild/Current/Bin/Roslyn/");

			return roslynFolder;
		}

		private static async Task<Path> FindVSInstallRootAsync(string requires)
		{
			// Find a copy of visual studio that has the required VisualCompiler
			var executablePath = new Path("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
			var workingDirectory = new Path("./");
			var argumentList = new List<string>()
			{
				"-latest",
				"-products",
				"*",
				"-requires",
				requires,
				"-property",
				"installationPath",
			};

			// Check if we should include pre-release versions
			bool includePrerelease = true;
			if (includePrerelease)
			{
				argumentList.Add("-prerelease");
			}

			// Execute the requested target
			var arguments = CombineArguments(argumentList);
			Log.Info(executablePath.ToString() + " " + arguments);
			var process = LifetimeManager.Get<IProcessManager>().CreateProcess(
				executablePath,
				arguments,
				workingDirectory);
			process.Start();
			await process.WaitForExitAsync();

			var stdOut = process.GetStandardOutput();
			var stdErr = process.GetStandardError();
			var exitCode = process.GetExitCode();

			if (!string.IsNullOrEmpty(stdErr))
			{
				Log.Error("VSWhere failed.");
				Log.Error(stdErr);
				throw new HandledException();
			}

			if (exitCode != 0)
			{
				// TODO: Return error code
				Log.Error("VSWhere failed.");
				throw new HandledException();
			}

			// The first line is the path
			using (var reader = new System.IO.StringReader(stdOut))
			{
				var path = reader.ReadLine();
				if (path is null)
				{
					Log.Error("Failed to parse vswhere output.");
					throw new HandledException();
				}

				return new Path(path);
			}
		}

		private static string CombineArguments(IList<string> args)
		{
			var argumentString = new StringBuilder();
			bool isFirst = true;
			foreach (var arg in args)
			{
				if (!isFirst)
					argumentString.Append(" ");

				argumentString.Append(arg);
				isFirst = false;
			}

			return argumentString.ToString();
		}
	}
}