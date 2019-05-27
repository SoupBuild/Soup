// <copyright file="RecipeManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Constants.h"
#include "Recipe.h"
#include "Path.h"

namespace Soup
{
    /// <summary>
    /// The recipe manager
    /// </summary>
    export class RecipeManager
    {
    public:
        /// <summary>
        /// Load the recipe from the root file
        /// </summary>
        static bool TryLoadFromFile(const Path& path, Recipe& recipe)
        {
            auto recipeFile = path + Path(Constants::RecipeFileName);
            auto recipeFilePath = recipeFile.ToString();
            if (std::filesystem::exists(recipeFilePath))
            {
                auto file = std::ifstream(recipeFilePath);
                recipe = LoadFromStream(file);
                return true;
            }

            recipe = {};
            return false;
        }

        /// <summary>
        /// Load from stream
        /// </summary>
        static Recipe LoadFromStream(std::istream& stream)
        {
            Recipe result = {};

            // Read the entire file into a string
            std::string content(
                (std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>());

            // Read the contents of the recipe file
            std::string error = "";
            auto jsonRoot = json11::Json::parse(content, error);
            if (jsonRoot.is_null())
            {
                auto message = "Failed to parse the recipe json: " + error;
                throw std::runtime_error(std::move(message));
            }
            else
            {
                return LoadJsonRecipe(jsonRoot);
            }
        }

        /// <summary>
        /// Save the recipe to the root file
        /// </summary>
        // static Task SaveToFile(Recipe recipe)
        // {
        //     // Serialize the contents of the recipe
        //     var content = JsonConvert.SerializeObject(
        //         recipe,
        //         Formatting.Indented,
        //         new JsonSerializerSettings() { DefaultValueHandling = DefaultValueHandling.Ignore });

        //     // Replace the contents of the file
        //     var recipePath = Path.Combine(Directory.GetCurrentDirectory(), Constants.RecipeFileName);
        //     using (var writer = new StreamWriter(File.OpenWrite(recipePath)))
        //     {
        //         await writer.WriteAsync(content);
        //     }
        // }

    private:
        static Recipe LoadJsonRecipe(const json11::Json& value)
        {
            std::string name;
            SemanticVersion version;
            std::optional<RecipeType> type;
            std::optional<std::vector<PackageReference>> dependencies;
            std::optional<std::string> publicFile;
            std::optional<std::vector<std::string>> source;

            if (!value["name"].is_null())
            {
                name = value["name"].string_value();
            }
            else
            {
                throw std::runtime_error("Missing Required field: name.");
            }

            if (!value["version"].is_null())
            {
                version = SemanticVersion::Parse(
                    value["version"].string_value());
            }
            else
            {
                throw std::runtime_error("Missing Required field: version.");
            }

            if (!value["type"].is_null())
            {
                type = Parse(
                    value["type"].string_value());
            }

            if (!value["dependencies"].is_null())
            {
                auto values = std::vector<PackageReference>();
                for (auto& value : value["dependencies"].array_items())
                {
                    auto dependency = PackageReference::Parse(value.string_value());
                    values.push_back(std::move(dependency));
                }

                dependencies = values;
            }

            if (!value["public"].is_null())
            {
                publicFile = value["public"].string_value();
            }

            if (!value["source"].is_null())
            {
                auto values = std::vector<std::string>();
                for (auto& value : value["dependencies"].array_items())
                {
                    values.push_back(value.string_value());
                }

                source = values;
            }

            return Recipe(
                std::move(name),
                version,
                type,
                std::move(dependencies),
                std::move(publicFile),
                std::move(source));
        }
    };
}
