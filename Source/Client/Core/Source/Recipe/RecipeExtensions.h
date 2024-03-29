﻿// <copyright file="RecipeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"
#include "RecipeSML.h"

namespace Soup::Core
{
	/// <summary>
	/// The recipe extensions
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class RecipeExtensions
	{
	public:
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		static bool TryLoadRecipeFromFile(
			const Path& recipeFile,
			Recipe& result)
		{
			// Verify the requested file exists
			Log::Diag("Load Recipe: " + recipeFile.ToString());
			if (!System::IFileSystem::Current().Exists(recipeFile))
			{
				Log::Info("Recipe file does not exist.");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(recipeFile, true);

			// Read the contents of the recipe file
			try
			{
				result = Recipe(
					RecipeSML::Deserialize(
						recipeFile,
						file->GetInStream()));
				return true;
			}
			catch (std::exception& ex)
			{
				Log::Error(std::string("Deserialize Threw: ") + ex.what());
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
			auto file = System::IFileSystem::Current().OpenWrite(recipeFile, false);

			// Write the recipe to the file stream
			RecipeSML::Serialize(recipe.GetTable(), file->GetOutStream());
		}
	};
}
