// <copyright file="FileBuildStateManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IBuildStateManager.h"
#include "Constants.h"

namespace Soup
{
    /// <summary>
    /// The build state manager
    /// </summary>
    export class FileBuildStateManager : public IBuildStateManager
    {
    private:
        static constexpr const char* BuildStateFileName = "BuildState.json";

    public:
        /// <summary>
        /// Load the build state from the provided directory
        /// </summary>
        virtual bool TryLoadState(const Path& directory, BuildState& result) override final
        {
            // Verify the requested file exists
            auto buildStateFile = directory +
                Path(Constants::ProjectGenerateFolderName) +
                Path(BuildStateFileName);
            auto buildStateFilePath = buildStateFile.ToString();
            if (!std::filesystem::exists(buildStateFilePath))
            {
                Log::Verbose("BuildState file does not exist.");
                return false;
            }

            // Open the file to read from
            auto file = std::fstream(buildStateFilePath);

            // Read the contents of the build state file
            try
            {
                result = BuildStateJson::Deserialize(file);
                return true;
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
        virtual void SaveState(const Path& directory, const BuildState& state) override final
        {
            // Open the file to write to
            auto buildStateFile = directory +
                Path(Constants::ProjectGenerateFolderName) +
                Path(BuildStateFileName);
            auto buildStateFilePath = buildStateFile.ToString();
            auto file = std::fstream(buildStateFilePath);

            // Write the build state to the file stream
            BuildStateJson::Serialize(state, file);
        }
    };
}
