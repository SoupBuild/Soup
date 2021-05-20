// <copyright file="ResolveToolsTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Text;

namespace Soup.Build.CSharp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	public class ResolveToolsTask : IBuildTask
	{
		private IBuildState _buildState;

		/// <summary>
		/// Get the run before list
		/// </summary>
		public static IReadOnlyList<string> RunBeforeList => new List<string>()
		{
		};

		/// <summary>
		/// Get the run after list
		/// </summary>
		public static IReadOnlyList<string> RunAfterList => new List<string>()
		{
		};

		public ResolveToolsTask(IBuildState buildState)
		{
			_buildState = buildState;

			if (!LifetimeManager.Has<IProcessManager>())
			{
				LifetimeManager.RegisterSingleton<IProcessManager, RuntimeProcessManager>();
			}
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var state = _buildState.ActiveState;
			var parameters = state["Parameters"].AsTable();

			var systemName = parameters["System"].AsString();
			var architectureName = parameters["Architecture"].AsString();

			if (systemName != "win32")
				throw new InvalidOperationException("Win32 is the only supported system... so far.");

			// Check if skip platform includes was specified
			bool skipPlatform = false;
			if (state.TryGetValue("SkipPlatform", out var skipPlatformValue))
			{
				skipPlatform = skipPlatformValue.AsBoolean();
			}

			// Find the location of the Windows SDK
			var visualStudioInstallRoot = FindVSInstallRoot();
			_buildState.LogTrace(TraceLevel.Information, "Using VS Installation: " + visualStudioInstallRoot.ToString());

			// Calculate the final Roslyn binaries folder
			var roslynFolder =
				visualStudioInstallRoot + new Path("/MSBuild/Current/Bin/Roslyn/");

			var cscToolPath = roslynFolder + new Path("csc.exe");

			// Save the build properties
			state["MSVS.InstallRoot"] = new Value(visualStudioInstallRoot.ToString());
			state["Roslyn.BinRoot"] = new Value(roslynFolder.ToString());
			state["Roslyn.CscToolPath"] = new Value(cscToolPath.ToString());
		}

		private Path FindVSInstallRoot()
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
				"Microsoft.VisualStudio.Component.Roslyn.Compiler",
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
			_buildState.LogTrace(TraceLevel.Debug, executablePath.ToString() + " " + arguments);
			var process = LifetimeManager.Get<IProcessManager>().CreateProcess(
				executablePath,
				arguments,
				workingDirectory);
			process.Start();
			process.WaitForExit();

			var stdOut = process.GetStandardOutput();
			var stdErr = process.GetStandardError();
			var exitCode = process.GetExitCode();

			if (!string.IsNullOrEmpty(stdErr))
			{
				_buildState.LogTrace(TraceLevel.Error, stdErr);
				throw new InvalidOperationException("VSWhere failed.");
			}

			if (exitCode != 0)
			{
				// TODO: Return error code
				_buildState.LogTrace(TraceLevel.Error, "FAILED");
				throw new InvalidOperationException("VSWhere failed.");
			}

			// The first line is the path
			using (var reader = new System.IO.StringReader(stdOut))
			{
				var path = reader.ReadLine();
				if (path is null)
				{
					_buildState.LogTrace(TraceLevel.Error, "Failed to parse vswhere output.");
					throw new InvalidOperationException("Failed to parse vswhere output.");
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
