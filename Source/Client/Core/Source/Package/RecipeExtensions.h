// <copyright file="RecipeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"
#include "RootRecipe.h"
#include "RecipeToml.h"

namespace Soup
{
	/// <summary>
	/// The recipe extensions
	/// </summary>
	export class RecipeExtensions
	{
	public:
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		static bool TryLoadRootRecipeFromFile(
			const Path& recipeFile,
			RootRecipe& result)
		{
			// Verify the requested file exists
			Log::Diag("Load Root Recipe: " + recipeFile.ToString());
			if (!System::IFileSystem::Current().Exists(recipeFile))
			{
				Log::Error("Root Recipe file does not exist.");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(recipeFile, true);

			// Read the contents of the recipe file
			try
			{
				result = RootRecipe(
					RecipeToml::Deserialize(
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
					RecipeToml::Deserialize(
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
		/// Check if there is a root recipe file in any of the parent directories from the package root
		/// </summary>
		static bool TryFindRootRecipeFile(const Path& packageRoot, Path& rootRecipeFile)
		{
			auto parentDirectory = packageRoot.GetParent();
			auto done = false;
			while (!done)
			{
				auto checkRootRecipeFile = parentDirectory + Path("RootRecipe.toml");
				if (System::IFileSystem::Current().Exists(checkRootRecipeFile))
				{
					// We found one!
					rootRecipeFile = std::move(checkRootRecipeFile);
					return true;
				}

				// Get the next parent directory
				auto nextParentDirectory = parentDirectory.GetParent();
				done = nextParentDirectory.ToString().size() == parentDirectory.ToString().size();
				parentDirectory = std::move(nextParentDirectory);
			}

			// Non of the parent directories contain the known file
			return false;
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
			RecipeToml::Serialize(recipe.GetTable(), file->GetOutStream());
		}

		/// <summary>
		/// Get the package reference path
		/// </summary>
		static Path GetPackageReferencePath(
			const Path& workingDirectory,
			const PackageReference& reference)
		{
			// If the path is relative then combine with the working directory
			auto packagePath = reference.GetPath();
			if (!packagePath.HasRoot())
			{
				packagePath = workingDirectory + packagePath;
			}

			return packagePath;
		}
	};
}
