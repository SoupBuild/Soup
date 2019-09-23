// <copyright file="BuildStateManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildState.h"
#include "BuildStateJson.h"
#include "Constants.h"
#include "IFileSystem.h"

namespace Soup
{
    /// <summary>
    /// The build state manager
    /// </summary>
    export class BuildStateManager
    {
    private:
        static constexpr std::string_view BuildStateFileName = "BuildState.json";

    public:
        /// <summary>
        /// Load the build state from the provided directory
        /// </summary>
        static bool TryLoadState(const Path& directory, BuildState& result)
        {
            // Verify the requested file exists
            auto buildStateFile = directory +
                Path(Constants::ProjectGenerateFolderName) +
                Path(BuildStateFileName);
            if (!IFileSystem::Current().Exists(buildStateFile))
            {
                Log::Verbose("BuildState file does not exist");
                return false;
            }

            // Open the file to read from
            auto file = IFileSystem::Current().OpenRead(buildStateFile);

            // Read the contents of the build state file
            try
            {
                result = BuildStateJson::Deserialize(*file);
                return true;
            }
            catch(std::runtime_error& ex)
            {
                Log::Info(ex.what());
                return false;
            }
            catch(...)
            {
                Log::Info("Failed to parse BuildState.");
                return false;
            }
        }

        /// <summary>
        /// Save the build state for the provided directory
        /// </summary>
        static void SaveState(const Path& directory, const BuildState& state)
        {
            auto buildProjectGenerateFolder = directory +
                Path(Constants::ProjectGenerateFolderName);
            auto buildStateFile = buildProjectGenerateFolder +
                Path(BuildStateFileName);

            // Ensure the target directories exists
            if (!IFileSystem::Current().Exists(buildProjectGenerateFolder))
            {
                Log::Verbose("Create Directory: " + Path(Constants::ProjectGenerateFolderName).ToString());
                IFileSystem::Current().CreateDirectory(buildProjectGenerateFolder);
            }

            // Open the file to write to
            auto file = IFileSystem::Current().OpenWrite(buildStateFile);

            // Write the build state to the file stream
            BuildStateJson::Serialize(state, *file);
        }
    };
}
