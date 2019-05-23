// <copyright file="BuildOptions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using CommandLine;

    /// <summary>
    /// Build Command Options
    /// </summary>
    [Verb("build")]
    internal class BuildOptions : SharedOptions
    {
        /// <summary>
        /// Gets or sets a value indicating whether to force a build
        /// </summary>
        [Option('f', "force", Default = false, HelpText = "Force a rebuild.")]
        public bool Force { get; set; }
    }
}
