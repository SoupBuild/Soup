// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
    export class FileInfo
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="FileInfo"/> class.
        /// </summary>
        FileInfo() :
            File(),
            Includes()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="FileInfo"/> class.
        /// </summary>
        FileInfo(Path file, std::vector<Path> includes) :
            File(std::move(file)),
            Includes(std::move(includes))
        {
        }

        Path File;
        std::vector<Path> Includes;

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const FileInfo& rhs) const
        {
            return File == rhs.File &&
                Includes == rhs.Includes;
        }

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
        /// Recursively build up the closure of all included files
        /// from the build state
        /// </summary>
        bool TryBuildIncludeClosure(
            const Path& sourceFile,
            std::vector<Path>& closure)
        {
            closure.clear();
            if (!TryBuildIncludeClosure(
                std::vector<Path>({ sourceFile }),
                closure))
            {
                closure.clear();
                return false;
            }
            else
            {
                return true;
            }
        }

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

    private:
        /// <summary>
        /// Internal implentation
        /// </summary>
        bool TryBuildIncludeClosure(
            const std::vector<Path>& sourceFiles,
            std::vector<Path>& closure)
        {
            for (auto& file : sourceFiles)
            {
                auto fileInfoResult = std::find_if(
                    KnownFiles.begin(),
                    KnownFiles.end(),
                    [&file](const FileInfo& fileInfo) { return fileInfo.File == file; });
                if (fileInfoResult != KnownFiles.end())
                {
                    // Copy all of the includes from the file info into the closure
                    auto& includes =  fileInfoResult->Includes;
                    std::copy(
                        includes.begin(),
                        includes.end(),
                        std::back_inserter(closure));

                    // Build up the child includes
                    if (!TryBuildIncludeClosure(includes, closure))
                    {
                        // Propagate the failed result
                        return false;
                    }
                }
                else
                {
                    Log::Verbose("Missing file info: " + file.ToString());
                    return false;
                }
            }

            return true;
        }
    };
}
