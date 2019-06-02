// <copyright file="LinkerArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    /// <summary>
    /// The shared linker arguments
    /// </summary>
    class LinkerArguments
    {
    public:
        /// <summary>
        /// Gets or sets the root directory
        /// </summary>
        std::string RootDirectory;

        /// <summary>
        /// Gets or sets the output directory
        /// </summary>
        std::string OutputDirectory;

        /// <summary>
        /// Gets or sets the target name
        /// </summary>
        std::string Name;

        /// <summary>
        /// Gets or sets the list of source files
        /// </summary>
        std::vector<std::string> SourceFiles;

        /// <summary>
        /// Gets or sets the list of library files
        /// </summary>
        std::vector<std::string> LibraryFiles;
    };
}
