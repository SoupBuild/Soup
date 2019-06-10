// <copyright file="BuildArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Path.h"

namespace Soup
{
    /// <summary>
    /// The enumeration of target types
    /// </summary>
    export enum class TargetType
    {
        /// <summary>
        /// Executable
        /// </summary>
        Executable,

        /// <summary>
        /// Library
        /// </summary>
        Library,
    };

    /// <summary>
    /// The source file definition
    /// </summary>
    export struct SourceFile
    {
    public:
        Path File;
        bool IsModule;
    };

    /// <summary>
    /// The set of build arguments
    /// </summary>
    export struct BuildArguments
    {
        /// <summary>
        /// Gets or sets the target type
        /// </summary>
        TargetType Target;

        /// <summary>
        /// Gets or sets the working directory
        /// </summary>
        Path WorkingDirectory;

        /// <summary>
        /// Gets or sets the output object directory
        /// </summary>
        Path ObjectDirectory;

        /// <summary>
        /// Gets or sets the output binary directory
        /// </summary>
        Path BinaryDirectory;

        /// <summary>
        /// Gets or sets the list of source files
        /// </summary>
        std::vector<Path> SourceFiles;

        /// <summary>
        /// Gets or sets the list of include directories
        /// </summary>
        std::vector<Path> IncludeDirectories;

        /// <summary>
        /// Gets or sets the list of modules
        /// </summary>
        std::vector<Path> ModuleFiles;

        /// <summary>
        /// Gets or sets a value indicating whether the build is incremental or not
        /// </summary>
        bool IsIncremental;
    };
}
