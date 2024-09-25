// <copyright file="VSWhereUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class WhereIsUtilities
{
	public static async Task<Path> FindExecutableAsync(OSPlatform platform, string name)
	{
		Path executablePath;
		string separator;
		int skipCount;
		int newLineLength;
		switch (platform)
		{
			case OSPlatform.Windows:
				executablePath = new Path("C:/Windows/System32/where.exe");
				separator = "\r\n";
				skipCount = 0;
				newLineLength = 2;
				break;
			case OSPlatform.Linux:
				executablePath = new Path("/usr/bin/whereis");
				separator = " ";

				// Whereis sets the name as the first entry
				skipCount = 1;
				newLineLength = 1;
				break;
			default:
				throw new InvalidOperationException("Unsupported operating system");
		}

		var arguments = new List<string>()
		{
			name,
		};

		var stdOut = await ExecutableUtilities.RunExecutableAsync(executablePath, arguments);

		// The first line is the path
		var values = stdOut[..^newLineLength]
			.Split(separator)
			.Skip(skipCount)
			.ToList();
		if (values.Count == 0)
		{
			Log.Error("Failed to parse where output.");
			throw new HandledException();
		}

		return Path.Parse(values.First());
	}
}