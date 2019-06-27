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
    export enum class BuildTargetType
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

    std::string ToString(BuildTargetType value)
    {
        switch (value)
        {
            case BuildTargetType::Executable:
                return "Executable";
            case BuildTargetType::Library:
                return "Library";
            default:
                throw std::runtime_error("Unknown BuildTargetType");
        }
    }

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
        BuildTargetType Target;

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
        /// Gets or sets the module source files
        /// </summary>
        Path ModuleSourceFile;

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
        std::vector<Path> IncludeModules;

        /// <summary>
        /// Gets or sets a value indicating whether the build is incremental or not
        /// </summary>
        bool IsIncremental;

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const BuildArguments& rhs) const
        {
            return Target == rhs.Target &&
                WorkingDirectory == rhs.WorkingDirectory &&
                ObjectDirectory == rhs.ObjectDirectory &&
                BinaryDirectory == rhs.BinaryDirectory &&
                ModuleSourceFile  == rhs.ModuleSourceFile &&
                SourceFiles == rhs.SourceFiles &&
                IncludeDirectories == rhs.IncludeDirectories &&
                IncludeModules == rhs.IncludeModules &&
                IsIncremental == rhs.IsIncremental;
        }

        bool operator !=(const BuildArguments& rhs) const
        {
            return !(*this == rhs);
        }
    };
}
