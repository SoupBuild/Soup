// <copyright file="VSWhereUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Runtime.InteropServices;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class WhereIsUtilities
{
	public static async Task<Path> FindExecutableAsync(string name)
	{
		Path executablePath;
		string separator;
		int skipCount;
		if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
		{
			executablePath = new Path("C:/Windows/System32/where.exe");
			separator = "\r\n";
			skipCount = 0;
		}
		else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
		{
			executablePath = new Path("./usr/bin/whereis");
			separator = " ";

			// Whereis sets the name as the first entry
			skipCount = 1;
		}
		else
		{
			throw new InvalidOperationException("Unsupported operating system");
		}

		var arguments = new List<string>()
			{
				name,
			};

		var stdOut = await ExecutableUtilities.RunExecutableAsync(executablePath, arguments);

		// The first line is the path
		var values = stdOut[..^Environment.NewLine.Length]
			.Split(separator)
			.Skip(skipCount)
			.ToList();
		if (values.Count == 0)
		{
			Log.Error("Failed to parse where output.");
			throw new HandledException();
		}

		return new Path(values.First());
	}
}