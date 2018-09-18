// <copyright file="InstallOptions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using CommandLine;

    /// <summary>
    /// Install Command
    /// </summary>
    [Verb("install")]
    internal class InstallOptions : SharedOptions
    {
        /// <summary>
        /// Gets or sets the package name
        /// </summary>
        public string Package { get; set; }

        /// <summary>
        /// Show the usage details for this command
        /// </summary>
        private void ShowUsage()
        {
            Log.Info(string.Empty);
            Log.Info("Usage: soup install <package_file>");
            Log.Info("\tpackage_file: Must be a zip file.");
        }
    }
}
