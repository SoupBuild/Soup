// <copyright file="ViewOptions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using CommandLine;

    /// <summary>
    /// Shared Options for all Commands
    /// </summary>
    internal class SharedOptions
    {
        /// <summary>
        /// Gets or sets the package name
        /// </summary>
        [Option('v', "verbose", Default = false, HelpText = "Enable verbose logging.")]
        public bool EnableVerbose { get; set; }
    }
}
