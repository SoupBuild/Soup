// <copyright file="CompileResult.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    /// <summary>
    /// Header includes recursive container
    /// </summary>
    export class HeaderInclude
    {
    public:
        /// <summary>
        /// Gets or sets the filename
        /// </summary>
        std::string Filename;

        /// <summary>
        /// Gets or sets the list of includes
        /// </summary>
        std::vector<HeaderInclude> Includes;
    };

    /// <summary>
    /// The results of a compilation
    /// </summary>
    export class CompileResult
    {
    public:
        /// <summary>
        /// Gets or sets the list of include files
        /// </summary>
        std::vector<HeaderInclude> HeaderIncludeFiles;
    };
}
