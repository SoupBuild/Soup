// <copyright file="LinkArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    /// <summary>
    /// The enumeration of link targets
    /// </summary>
    export enum class LinkTarget
    {
        /// <summary>
        /// Static Library
        /// </summary>
        StaticLibrary,

        /// <summary>
        /// v
        /// </summary>
        Executable,
    };

    /// <summary>
    /// The shared link arguments
    /// </summary>
    export class LinkArguments
    {
    public:
        /// <summary>
        /// Gets or sets the target type
        /// </summary>
        LinkTarget Target;

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

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const LinkArguments& rhs) const
        {
            return Target == rhs.Target &&
                RootDirectory == rhs.RootDirectory &&
                OutputDirectory == rhs.OutputDirectory &&
                Name == rhs.Name &&
                SourceFiles == rhs.SourceFiles &&
                LibraryFiles == rhs.LibraryFiles;
        }

        bool operator !=(const LinkArguments& rhs) const
        {
            return !(*this == rhs);
        }
    };
}
