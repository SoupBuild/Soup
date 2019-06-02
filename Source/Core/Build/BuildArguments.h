// <copyright file="BuildArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    /// <summary>
    /// The set of build arguments
    /// </summary>
    export struct BuildArguments
    {
        /// <summary>
        /// Gets or sets the output directory
        /// </summary>
        std::string OutputDirectory;

        /// <summary>
        /// Gets or sets the list of source files
        /// </summary>
        std::vector<std::string> SourceFiles;

        /// <summary>
        /// Gets or sets the list of include directories
        /// </summary>
        std::vector<std::string> IncludeDirectories;

        /// <summary>
        /// Gets or sets the list of modules
        /// </summary>
        std::vector<std::string> Modules;
    };
}
