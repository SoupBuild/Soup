// <copyright file="RecipeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"
#include "RecipeJson.h"

namespace Soup
{
    /// <summary>
    /// The recipe extensions
    /// </summary>
    export class RecipeExtensions
    {
    private:
        static constexpr const char* RecipeFileName = "Recipe.json";

    public:
        /// <summary>
        /// Attempt to load from file
        /// </summary>
        static bool TryLoadFromFile(
            const Path& recipeFile,
            Recipe& result)
        {
            // Verify the requested file exists
            if (!IFileSystem::Current().Exists(recipeFile))
            {
                Log::Info("Recipe file does not exist.");
                return false;
            }

            // Open the file to read from
            auto file = IFileSystem::Current().OpenRead(recipeFile);

            // Read the contents of the recipe file
            try
            {
                result = RecipeJson::Deserialize(*file);
                return true;
            }
            catch(...)
            {
                Log::Info("Failed to parse Recipe.");
                return false;
            }
        }

        /// <summary>
        /// Build up the recipe file location from the package reference
        /// </summary>
        static Path GetPackageRecipeFile(
            const Path& workingDirectory,
            const PackageReference& package)
        {
            if (package.IsLocal())
            {
                auto recipeFile = workingDirectory + package.GetPath();
                return recipeFile;
            }
            else
            {
                throw std::runtime_error("Non-local packages not supported.");
            }
        }

        /// <summary>
        /// Save the recipe to file
        /// </summary>
        static void SaveToFile(
            const Path& recipeFile,
            Recipe& recipe)
        {
            // Open the file to write to
            auto file = IFileSystem::Current().OpenWrite(recipeFile);

            // Write the recipe to the file stream
            RecipeJson::Serialize(recipe, *file);
        }
    };
}
