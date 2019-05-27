// <copyright file="RecipeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
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
        /// Load from stream
        /// </summary>
        static bool TryLoadFromDirectory(
            const Path& directory,
            Recipe& result)
        {
            auto recipeFile = directory + Path(RecipeFileName);
            auto recipeFilePath = recipeFile.ToString();
            if (!std::filesystem::exists(recipeFilePath))
            {
                Log::Info("Recipe file does not exist.");
                return false;
            }

            // Read the entire file into a string
            auto file = std::fstream(recipeFilePath);

            // Read the contents of the recipe file
            try
            {
                result = RecipeJson::Deserialize(file);
                return true;
            }
            catch(...)
            {
                Log::Info("Failed to parse Recipe.");
                return false;
            }
        }
    };
}
