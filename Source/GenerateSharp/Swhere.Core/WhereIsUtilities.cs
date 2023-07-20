﻿// <copyright file="VSWhereUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Opal;
using Opal.System;

namespace Soup.Build.Discover
{
	public static class WhereIsUtilities
	{
		public static async Task<Path> FindExecutableAsync(string name)
		{
			var executablePath = new Path("/usr/bin/whereis");
			var arguments = new List<string>()
			{
				name,
			};

			var stdOut = await ExecutableUtilities.RunExecutableAsync(executablePath, arguments);

			// The first line is the path
			var values = stdOut.Substring(0, stdOut.Length - 1).Split(' ');
			if (values.Length < 2)
			{
				Log.Error("Failed to parse where output.");
				throw new HandledException();
			}

			return new Path(values[1]);
		}
	}
}