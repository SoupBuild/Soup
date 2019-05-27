// <copyright file="RecipeJson.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"
#include "Path.h"

namespace Soup
{
    /// <summary>
    /// The recipe json serialize manager
    /// </summary>
    export class RecipeJson
    {
    private:
        static constexpr const char* Property_Name = "name";
        static constexpr const char* Property_Version = "version";
        static constexpr const char* Property_Type = "type";
        static constexpr const char* Property_Dependencies = "dependencies";
        static constexpr const char* Property_Public = "public";
        static constexpr const char* Property_Source = "source";

    public:
        /// <summary>
        /// Load from stream
        /// </summary>
        static Recipe Deserialize(std::istream& stream)
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
        static void Serialize(const Recipe& recipe, std::ostream& stream)
        {
            // Serialize the contents of the recipe
            json11::Json json = BuildJson(recipe);

            stream << json.dump();
        }

    private:
        static Recipe LoadJsonRecipe(const json11::Json& value)
        {
            std::string name;
            SemanticVersion version;
            std::optional<RecipeType> type;
            std::optional<std::vector<PackageReference>> dependencies;
            std::optional<std::string> publicFile;
            std::optional<std::vector<std::string>> source;

            if (!value[Property_Name].is_null())
            {
                name = value[Property_Name].string_value();
            }
            else
            {
                throw std::runtime_error("Missing Required field: name.");
            }

            if (!value[Property_Version].is_null())
            {
                version = SemanticVersion::Parse(
                    value[Property_Version].string_value());
            }
            else
            {
                throw std::runtime_error("Missing Required field: version.");
            }

            if (!value[Property_Type].is_null())
            {
                type = Parse(
                    value[Property_Type].string_value());
            }

            if (!value[Property_Dependencies].is_null())
            {
                auto values = std::vector<PackageReference>();
                for (auto& value : value[Property_Dependencies].array_items())
                {
                    auto dependency = PackageReference::Parse(value.string_value());
                    values.push_back(std::move(dependency));
                }

                dependencies = values;
            }

            if (!value[Property_Public].is_null())
            {
                publicFile = value[Property_Public].string_value();
            }

            if (!value[Property_Source].is_null())
            {
                auto values = std::vector<std::string>();
                for (auto& value : value[Property_Source].array_items())
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

        static json11::Json BuildJson(const Recipe& recipe)
        {
            json11::Json::object result = {};

            // Add required fields
            result[Property_Name] = recipe.GetName();
            result[Property_Version] = recipe.GetVersion().ToString();

            if (recipe.HasType())
            {
                result[Property_Type] = ToString(recipe.GetType());
            }

            if (recipe.HasDependencies())
            {
                json11::Json::array dependencies;
                for (auto& value : recipe.GetDependencies())
                {
                    dependencies.push_back(value.ToString());
                }

                result[Property_Dependencies] = std::move(dependencies);
            }

            if (recipe.HasPublic())
            {
                result[Property_Public] = recipe.GetPublic();
            }

            if (recipe.HasSource())
            {
                json11::Json::array source;
                for (auto& value : recipe.GetSource())
                {
                    source.push_back(value);
                }

                result[Property_Source] = std::move(source);
            }

            return result;
        }
    };
}
