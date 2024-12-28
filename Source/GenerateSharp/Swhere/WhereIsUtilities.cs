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
		string executable;
		string separator;
		int skipCount;
		int newLineLength;
		switch (platform)
		{
			case OSPlatform.Windows:
				executable = "C:/Windows/System32/where.exe";
				separator = "\r\n";
				skipCount = 0;
				newLineLength = 2;
				break;
			case OSPlatform.Linux:
				executable = "which";
				separator = " ";

				// which sets the name as the first entry
				skipCount = 0;
				newLineLength = 1;
				break;
			default:
				throw new InvalidOperationException("Unsupported operating system");
		}

		var arguments = new List<string>()
		{
			name,
		};

		var stdOut = await ExecutableUtilities.RunExecutableAsync(executable, arguments);

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