// <copyright file="PathExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System.IO;

    /// <summary>
    /// Path extensions
    /// </summary>
    public static class PathExtensions
    {
        /// <summary>
        /// Check if there is a trailing slash and append one if not
        /// </summary>
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

        /// <summary>
        /// Remove trailing slash if it exists
        /// </summary>
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
