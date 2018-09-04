// <copyright file="LinkerArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Compiler
{
    using System.Collections.Generic;

    /// <summary>
    /// The shared linker arguments
    /// </summary>
    public class LinkerArguments
    {
        /// <summary>
        /// Gets or sets the root directory
        /// </summary>
        public string RootDirectory { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the output directory
        /// </summary>
        public string OutputDirectory { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the target name
        /// </summary>
        public string Name { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the list of source files
        /// </summary>
        public IList<string> SourceFiles { get; set; } = new List<string>();

        /// <summary>
        /// Gets or sets the list of library files
        /// </summary>
        public IList<string> LibraryFiles { get; set; } = new List<string>();
    }
}
