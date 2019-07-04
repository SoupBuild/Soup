// <copyright file="BuildStateChecker.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildState.h"
#include "IFileSystem.h"
#include "Path.h"

namespace Soup
{
    export class BuildStateChecker
    {
    public:
        /// <summary>
        /// Perform a check if the requested target is outdated with
        /// respect to the input files
        /// </summary>
        static bool IsOutdated(
            const Path& targetFile,
            const std::vector<Path>& inputFiles,
            const Path& rootPath)
        {
            // Verify the output file exists
            auto relativeOutputFile = rootPath + targetFile;
            if (!IFileSystem::Current().Exists(relativeOutputFile))
            {
                Log::Verbose("Output target does not exist: " + targetFile.ToString());
                return true;
            }

            auto outputFileLastWriteTime = 
                IFileSystem::Current().GetLastWriteTime(relativeOutputFile);
            for (auto& inputFile : inputFiles)
            {
                // if the file is relative then combine it with the root path
                Path relativeInputFile = inputFile.HasRoot() ? inputFile : rootPath + inputFile;
                if (!IFileSystem::Current().Exists(relativeInputFile))
                {
                    Log::Verbose("Input file missing, rebuild required.");
                    Log::Verbose("[" + inputFile.ToString() + "] -> [" + targetFile.ToString() + "].");
                    return true;
                }

                auto dependencyLastWriteTime = 
                    IFileSystem::Current().GetLastWriteTime(relativeInputFile);
                if (dependencyLastWriteTime > outputFileLastWriteTime)
                {
                    Log::Verbose("Input file altered after target, rebuild required.");
                    Log::Verbose("[" + inputFile.ToString() + "] -> [" + targetFile.ToString() + "].");
                    return true;
                }
            }

            return false;
        }
    };
}
