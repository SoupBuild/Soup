// <copyright file="OperationGraphManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The operation state manager
	/// </summary>
	internal static class PathListManager
	{
		/// <summary>
		/// Load the path list from the provided directory
		/// </summary>
		public static async Task<bool> TryLoadFileAsync(
			Path pathListFile,
			IList<Path> pathList)
		{
			// Verify the requested file exists
			if (!System.IO.File.Exists(pathListFile.ToString()))
			{
				Log.Info("Path list file does not exist");
				return false;
			}

			// Open the file to read from
			using (var fileStream = System.IO.File.OpenRead(pathListFile.ToString()))
			using (var reader = new System.IO.StreamReader(fileStream))
			{
				// Read the contents of the build state file
				try
				{
					while (reader.Peek() >= 0)
					{
						var line = await reader.ReadLineAsync();
						if (line is null)
							throw new InvalidOperationException("Reading line returned null string");
						var path = new Path(line);
						pathList.Add(path);
					}

					return true;
				}
				catch
				{
					Log.Error("Failed to parse operation graph");
					return false;
				}
			}
		}
	}
}
