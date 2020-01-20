// <copyright file="RecipeToml.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"

namespace Soup
{
	/// <summary>
	/// The recipe Toml serialize manager
	/// </summary>
	export class RecipeToml
	{
	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static Recipe Deserialize(std::istream& stream)
		{
			// Read the entire file into a string
			std::string content(
				(std::istreambuf_iterator<char>(stream)),
				std::istreambuf_iterator<char>());

			// Read the contents of the recipe file
			auto root = toml::parse(content);
			auto& rootTable = root.as_table();
			for (auto& element : rootTable)
			{
				std::cout << element->first << std::endl;
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
	};
}
