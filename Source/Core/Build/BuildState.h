// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    export class FileInfo
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="FileInfo"/> class.
        /// </summary>
        FileInfo() :
            Name(),
            Includes()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FileInfo"/> class.
        /// </summary>
        FileInfo(std::string name, std::vector<std::string> includes) :
            Name(std::move(name)),
            Includes(std::move(includes))
        {
        }

        std::string Name;
        std::vector<std::string> Includes;

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const FileInfo& rhs) const
        {
            return Name == rhs.Name &&
                Includes == rhs.Includes;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        bool operator !=(const FileInfo& rhs) const
        {
            return !(*this == rhs);
        }
    };

    export class BuildState
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="BuildState"/> class.
        /// </summary>
        BuildState() :
            KnownFiles()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="BuildState"/> class.
        /// </summary>
        BuildState(std::vector<FileInfo> knownFiles) :
            KnownFiles(std::move(knownFiles))
        {
        }

        std::vector<FileInfo> KnownFiles;

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const BuildState& rhs) const
        {
            return KnownFiles == rhs.KnownFiles;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        bool operator !=(const BuildState& rhs) const
        {
            return !(*this == rhs);
        }
    };
}
