// <copyright file="CompilerArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Compiler
{
    using System.Collections.Generic;

    /// <summary>
    /// The enumeration of language standards
    /// </summary>
    public enum LanguageStandard
    {
        /// <summary>
        /// C++ 11
        /// </summary>
        CPP11,

        /// <summary>
        /// C++ 14
        /// </summary>
        CPP14,

        /// <summary>
        /// C++ 17
        /// </summary>
        CPP17,

        /// <summary>
        /// The latest experimental features
        /// </summary>
        Latest
    }

    /// <summary>
    /// The set of standard compiler arguments
    /// </summary>
    public class CompilerArguments
    {
        /// <summary>
        /// Gets or sets the language standard
        /// </summary>
        public LanguageStandard Standard { get; set; } = LanguageStandard.CPP14;

        /// <summary>
        /// Gets or sets the root directory
        /// </summary>
        public string RootDirectory { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the output directory
        /// </summary>
        public string OutputDirectory { get; set; } = string.Empty;

        /// <summary>
        /// Gets or sets the list of preprocessor definitions
        /// </summary>
        public IList<string> PreprocessorDefinitions { get; set; } = new List<string>();

        /// <summary>
        /// Gets or sets the list of source files
        /// </summary>
        public IList<string> SourceFiles { get; set; } = new List<string>();

        /// <summary>
        /// Gets or sets the list of include directories
        /// </summary>
        public IList<string> IncludeDirectories { get; set; } = new List<string>();

        /// <summary>
        /// Gets or sets the list of modules
        /// </summary>
        public IList<string> Modules { get; set; } = new List<string>();

        /// <summary>
        /// Gets or sets a value indicating whether to compile as export module
        /// </summary>
        public bool ExportModule { get; set; } = false;
    }
}
