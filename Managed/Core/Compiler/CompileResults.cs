// <copyright file="CompilerArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Compiler
{
    using System.Collections.Generic;

    /// <summary>
    /// Header includes recursive container
    /// </summary>
    public class HeaderInclude
    {
        /// <summary>
        /// Gets or sets the filename
        /// </summary>
        public string Filename { get; set; }

        /// <summary>
        /// Gets or sets the list of includes
        /// </summary>
        public IList<HeaderInclude> Includes { get; set; } = new List<HeaderInclude>();
    }

    /// <summary>
    /// The results of a compilation
    /// </summary>
    public class CompileResults
    {
        /// <summary>
        /// Gets or sets the list of include files
        /// </summary>
        public IList<HeaderInclude> HeaderIncludeFiles { get; set; } = null;
    }
}
