// <copyright file="ExecutableUtilities .cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Text;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class ExecutableUtilities
{
	public static async Task<string> RunExecutableAsync(Path executable, IList<string> arguments)
	{
		var workingDirectory = new Path("./");

		// Execute the requested target
		var argumentsValue = CombineArguments(arguments);
		Log.Info($"{executable} {argumentsValue}");

		var process = LifetimeManager.Get<IProcessManager>().CreateProcess(
			executable,
			argumentsValue,
			workingDirectory);
		process.Start();
		await process.WaitForExitAsync();

		var stdOut = process.GetStandardOutput();
		var stdErr = process.GetStandardError();
		var exitCode = process.GetExitCode();

		if (!string.IsNullOrEmpty(stdErr))
		{
			Log.Error("Executable wrote error message.");
			Log.Error(stdErr);
			throw new HandledException();
		}

		if (exitCode != 0)
		{
			Log.Error($"Where failed: {exitCode}");
			throw new HandledException();
		}

		return stdOut;
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