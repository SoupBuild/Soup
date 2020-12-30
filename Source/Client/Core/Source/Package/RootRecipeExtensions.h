// <copyright file="RootRecipeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RootRecipe.h"

namespace Soup::Build
{
	/// <summary>
	/// The root recipe extensions
	/// </summary>
	export class RootRecipeExtensions
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
					Runtime::RecipeToml::Deserialize(
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
	};
}
