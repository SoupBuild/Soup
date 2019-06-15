// <copyright file="CompilerArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
    /// <summary>
    /// The enumeration of language standards
    /// </summary>
    export enum class LanguageStandard
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
        /// C++ 20
        /// </summary>
        CPP20,
    };

    /// <summary>
    /// The set of standard compiler arguments
    /// </summary>
    export struct CompileArguments
    {
        /// <summary>
        /// Gets or sets the language standard
        /// </summary>
        LanguageStandard Standard;

        /// <summary>
        /// Gets or sets the root directory
        /// </summary>
        Path RootDirectory;

        /// <summary>
        /// Gets or sets the output directory
        /// </summary>
        Path OutputDirectory;

        /// <summary>
        /// Gets or sets the list of preprocessor definitions
        /// </summary>
        std::vector<std::string> PreprocessorDefinitions;

        /// <summary>
        /// Gets or sets the set of source file
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
        /// Gets or sets a value indicating whether to compile as export module
        /// </summary>
        bool ExportModule;

        /// <summary>
        /// Gets or sets a value indicating whether to generate the include set for each file
        /// </summary>
        bool GenerateIncludeTree;

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const CompileArguments& rhs) const
        {
            return Standard == rhs.Standard &&
                RootDirectory == rhs.RootDirectory &&
                OutputDirectory == rhs.OutputDirectory &&
                PreprocessorDefinitions == rhs.PreprocessorDefinitions &&
                SourceFiles  == rhs.SourceFiles &&
                IncludeDirectories == rhs.IncludeDirectories &&
                IncludeModules == rhs.IncludeModules &&
                ExportModule == rhs.ExportModule &&
                GenerateIncludeTree == rhs.GenerateIncludeTree;
        }

        bool operator !=(const CompileArguments& rhs) const
        {
            return !(*this == rhs);
        }

        std::string ToString() const
        {
            auto stringBuilder = std::stringstream();
            stringBuilder << "[" <<
                static_cast<int>(Standard) << ", " <<
                RootDirectory.ToString() << ", " <<
                OutputDirectory.ToString() << ", [";

            for (auto& value : PreprocessorDefinitions)
                stringBuilder << value << ", ";

            stringBuilder << "], [";
            
            for (auto& value : SourceFiles)
                stringBuilder << value.ToString() << ", ";

            stringBuilder << "], [";

            for (auto& value : IncludeDirectories)
                stringBuilder << value.ToString() << ", ";

            stringBuilder << "], [";

            for (auto& value : IncludeModules)
                stringBuilder << value.ToString() << ", ";

            stringBuilder << "], " <<
                ExportModule << ", " <<
                GenerateIncludeTree << "]";

            return stringBuilder.str();
        }
    };
}
