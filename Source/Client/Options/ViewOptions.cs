// <copyright file="ViewOptions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using CommandLine;

    /// <summary>
    /// View Command Options
    /// </summary>
    [Verb("version")]
    internal class ViewOptions
    {
        /// <summary>
        /// Gets or sets the package name
        /// </summary>
        public string Package { get; set; }

        /// <summary>
        /// Show the usage details for this command
        /// </summary>
        private static void ShowUsage()
        {
            Log.Info(string.Empty);
            Log.Info("Usage: soup view <package>");
            Log.Info("\tpackage: The unique package name.");
        }
    }
}
