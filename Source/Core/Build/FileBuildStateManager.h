// <copyright file="FileBuildStateManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IBuildStateManager.h"

namespace Soup
{
    /// <summary>
    /// The build state manager
    /// </summary>
    class FileBuildStateManager : public IBuildStateManager
    {
    public:
        /// <summary>
        /// Load the build state from the root file
        /// </summary>
        static async Task<BuildState> LoadFromFileAsync(string path)
        {
            BuildState result = null;
            var buildStatePath = Path.Combine(path, Constants.ProjectGenerateFolderName, Constants.BuildStateFileName);
            if (File.Exists(buildStatePath))
            {
                using (var file = File.OpenRead(buildStatePath))
                {
                    result = await LoadFromStreamAsync(file);
                }
            }

            return result;
        }

        /// <summary>
        /// Load from stream async
        /// </summary>
        public static async Task<BuildState> LoadFromStreamAsync(Stream stream)
        {
            BuildState result = null;
            using (var reader = new StreamReader(stream, Encoding.UTF8, false, 2048, true))
            {
                // Read the contents of the build state file
                var content = await reader.ReadToEndAsync();
                try
                {
                    result = JsonConvert.DeserializeObject<BuildState>(content);
                }
                catch(JsonReaderException)
                {
                    Log.Verbose("Invalid build state.");
                }
            }

            return result;
        }

        /// <summary>
        /// Save the build state to the root file
        /// </summary>
        public static async Task SaveToFileAsync(BuildState state, string path)
        {
            // Serialize the contents of the build state
            var content = JsonConvert.SerializeObject(
                state,
                Formatting.Indented,
                new JsonSerializerSettings() { DefaultValueHandling = DefaultValueHandling.Ignore });

            // Replace the contents of the file
            var projectGenFolder = Path.Combine(path, Constants.ProjectGenerateFolderName);
            if (!Directory.Exists(projectGenFolder))
            {
                Directory.CreateDirectory(projectGenFolder);
            }

            var buildStatePath = Path.Combine(projectGenFolder, Constants.BuildStateFileName);
            using (var writer = new StreamWriter(File.Create(buildStatePath)))
            {
                await writer.WriteAsync(content);
            }
        }
    }
}
