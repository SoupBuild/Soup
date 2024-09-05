// <copyright file="VSWhereUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Text;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class VSWhereUtilities
{
	/// <summary>
	/// Attempt to find MSVC compiler installation 
	/// </summary>
	public static async Task<(string Version, Path Path)> FindMSVCInstallAsync(bool includePrerelease)
	{
		// Find the location of the Windows SDK
		var visualStudioInstallRoot = await FindVSInstallRootAsync(
			"Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
			includePrerelease);
		Log.HighPriority("Using VS Installation: " + visualStudioInstallRoot.ToString());

		// Use the default version
		var visualCompilerVersion = await FindDefaultVCToolsVersionAsync(visualStudioInstallRoot);
		Log.HighPriority("Using VC Version: " + visualCompilerVersion);

		// Calculate the final VC tools folder
		var visualCompilerVersionFolder =
			visualStudioInstallRoot + new Path($"./VC/Tools/MSVC/{visualCompilerVersion}/");

		return (visualCompilerVersion, visualCompilerVersionFolder);
	}

	private static async Task<Path> FindVSInstallRootAsync(string requires, bool includePrerelease)
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
		if (includePrerelease)
		{
			argumentList.Add("-prerelease");
		}

		// Execute the requested target
		var arguments = CombineArguments(argumentList);
		Log.Info(executablePath.ToString() + " " + arguments);
		if (!LifetimeManager.Get<IFileSystem>().Exists(executablePath))
		{
			Log.Error("VSWhere is not installed on the host machine");
			throw new HandledException();
		}

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
		using var reader = new System.IO.StringReader(stdOut);
		var path = await reader.ReadLineAsync();
		if (path is null)
		{
			Log.Error("Failed to parse vswhere output.");
			throw new HandledException();
		}

		return new Path(path);
	}

	private static async Task<string> FindDefaultVCToolsVersionAsync(
		Path visualStudioInstallRoot)
	{
		// Check the default tools version
		var visualCompilerToolsDefaultVersionFile =
			visualStudioInstallRoot + new Path("./VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt");
		if (!LifetimeManager.Get<IFileSystem>().Exists(visualCompilerToolsDefaultVersionFile))
		{
			Log.Error("VisualCompilerToolsDefaultVersionFile file does not exist: " + visualCompilerToolsDefaultVersionFile.ToString());
			throw new HandledException();
		}

		// Read the entire file into a string
		using var file = LifetimeManager.Get<IFileSystem>().OpenRead(visualCompilerToolsDefaultVersionFile);
		using var reader = new System.IO.StreamReader(file.GetInStream(), null, true, -1, true);
		// The first line is the version
		var version = await reader.ReadLineAsync();
		if (version is null)
		{
			Log.Error("Failed to parse version from file.");
			throw new HandledException();
		}

		return version;
	}

	private static string CombineArguments(IList<string> args)
	{
		var argumentString = new StringBuilder();
		bool isFirst = true;
		foreach (var arg in args)
		{
			if (!isFirst)
				_ = argumentString.Append(' ');

			_ = argumentString.Append(arg);
			isFirst = false;
		}

		return argumentString.ToString();
	}
}