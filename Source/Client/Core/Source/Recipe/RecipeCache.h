// <copyright file="RecipeCache.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe/RecipeExtensions.h"
#include "Recipe/RootRecipeExtensions.h"

namespace Soup::Core
{
	/// <summary>
	/// The recipe cache that maintains an in memory collection of recipes to prevent loading multiple instances from disk
	/// </summary>
	export class RecipeCache
	{
	private:
		std::map<std::string, Recipe> _knownRecipes;
		std::map<std::string, RootRecipe> _knownRootRecipes;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeCache"/> class.
		/// </summary>
		RecipeCache() :
			_knownRecipes(),
			_knownRootRecipes()
		{
		}

		bool TryGetRootRecipe(
			const Path& recipeFile,
			RootRecipe& result)
		{
			// Check if the recipe was already loaded
			auto findRecipe = _knownRootRecipes.find(recipeFile.ToString());
			if (findRecipe != _knownRootRecipes.end())
			{
				result = findRecipe->second;
				return true;
			}
			else
			{
				RootRecipe loadRecipe;
				if (RootRecipeExtensions::TryLoadRootRecipeFromFile(recipeFile, loadRecipe))
				{
					// Save the recipe for later
					auto insertRecipe = _knownRootRecipes.emplace(
						recipeFile.ToString(),
						std::move(loadRecipe));

					result = insertRecipe.first->second;
					return true;
				}
				else
				{
					// Failed to load this recipe
					return false;
				}
			}
		}

		Recipe GetRecipe(const Path& recipeFile)
		{
			// The Recipe must already be loaded
			auto findRecipe = _knownRecipes.find(recipeFile.ToString());
			if (findRecipe != _knownRecipes.end())
			{
				return findRecipe->second;
			}
			else
			{
				throw std::runtime_error("Recipe [" + recipeFile.ToString() + "] not found in closure");
			}
		}

		bool TryGetOrLoadRecipe(
			const Path& recipeFile,
			Recipe& result)
		{
			// Check if the recipe was already loaded
			auto findRecipe = _knownRecipes.find(recipeFile.ToString());
			if (findRecipe != _knownRecipes.end())
			{
				result = findRecipe->second;
				return true;
			}
			else
			{
				Recipe loadRecipe = {};
				if (RecipeExtensions::TryLoadRecipeFromFile(recipeFile, loadRecipe))
				{
					// Save the recipe for later
					auto insertRecipe = _knownRecipes.emplace(
						recipeFile.ToString(),
						std::move(loadRecipe));

					result = insertRecipe.first->second;
					return true;
				}
				else
				{
					// Failed to load this recipe
					return false;
				}
			}
		}
	};
}
