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

    std::string ToString(LanguageStandard value)
    {
        switch (value)
        {
            case LanguageStandard::CPP11:
                return "C++11";
            case LanguageStandard::CPP14:
                return "C++14";
            case LanguageStandard::CPP17:
                return "C++17";
            case LanguageStandard::CPP20:
                return "C++20";
            default:
                throw std::runtime_error("Unknown LanguageStandard");
        }
    }

    /// <summary>
    /// The enumeration of optimization levels
    /// </summary>
    export enum class OptimizationLevel
    {
        /// <summary>
        /// Disable all optimization for build speed and debugability
        /// </summary>
        Disabled,

        /// <summary>
        /// Optimize for speed
        /// </summary>
        Speed,

        /// <summary>
        /// Optimize for size
        /// </summary>
        Size,
    };

    std::string ToString(OptimizationLevel value)
    {
        switch (value)
        {
            case OptimizationLevel::Disabled:
                return "Disabled";
            case OptimizationLevel::Speed:
                return "Speed";
            case OptimizationLevel::Size:
                return "Size";
            default:
                throw std::runtime_error("Unknown OptimizationLevel");
        }
    }

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
        /// Gets or sets the optimization level
        /// </summary>
        OptimizationLevel Optimize;

        /// <summary>
        /// Gets or sets the root directory
        /// </summary>
        Path RootDirectory;

        /// <summary>
        /// Gets or sets the list of preprocessor definitions
        /// </summary>
        std::vector<std::string> PreprocessorDefinitions;

        /// <summary>
        /// Gets or sets the source file
        /// </summary>
        Path SourceFile;

        /// <summary>
        /// Gets or sets the target file
        /// </summary>
        Path TargetFile;

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
                Optimize == rhs.Optimize &&
                RootDirectory == rhs.RootDirectory &&
                PreprocessorDefinitions == rhs.PreprocessorDefinitions &&
                SourceFile  == rhs.SourceFile &&
                TargetFile  == rhs.TargetFile &&
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
                ::Soup::ToString(Standard) << ", " <<
                ::Soup::ToString(Optimize) << ", " <<
                RootDirectory.ToString() << ", [";

            for (auto& value : PreprocessorDefinitions)
                stringBuilder << value << ", ";

            stringBuilder << "], " << 
                SourceFile.ToString() << ", " <<
                TargetFile.ToString() << ", [";

            for (auto& value : IncludeDirectories)
                stringBuilder << value.ToString() << ", ";

            stringBuilder << "], [";

            for (auto& value : IncludeModules)
                stringBuilder << value.ToString() << ", ";

            stringBuilder << "], " <<
                std::to_string(ExportModule) << ", " <<
                std::to_string(GenerateIncludeTree) << "]";

            return stringBuilder.str();
        }
    };
}
