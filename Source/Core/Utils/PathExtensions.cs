// <copyright company="Soup" file="PathExtensions.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;
	using System.IO;

	/// <summary>
	/// Path extensions
	/// </summary>
	public static class PathExtensions
	{
		public static string EnsureTrailingSlash(this string path)
        {
            if (!path.EndsWith(Path.DirectorySeparatorChar))
            {
                return path + Path.DirectorySeparatorChar;
            }
            else
            {
                return path;
            }
        }

		public static string RemoveTrailingSlash(this string path)
        {
            if (path.EndsWith(Path.DirectorySeparatorChar))
            {
                return path.Substring(0, path.Length - 1);
            }
            else
            {
                return path;
            }
        }
	}
}
