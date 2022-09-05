// <copyright file="RecipeBuildLocationManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe/Recipe.h"
#include "ValueTable/ValueTable.h"
#include "ValueTable/ValueTableWriter.h"
#include "ProjectManager.h"
#include "RecipeBuildCacheState.h"

namespace Soup::Core
{
	/// <summary>
	/// The recipe build location manager that knows how to generate the unique folder for building a 
	/// Recipe with a given set of parameters
	/// </summary>
	export class RecipeBuildLocationManager
	{
	public:
		static Path GetOutputDirectory(
			const Path& packageRoot,
			Recipe& recipe,
			const ValueTable& globalParameters,
			ProjectManager& projectManager)
		{
			// Set the default output directory to be relative to the package
			auto rootOutput = packageRoot + Path("out/");

			// Check for root recipe file with overrides
			Path rootRecipeFile;
			if (RootRecipeExtensions::TryFindRootRecipeFile(packageRoot, rootRecipeFile))
			{
				Log::Info("Found Root Recipe: '" + rootRecipeFile.ToString() + "'");
				RootRecipe rootRecipe;
				if (!projectManager.TryGetRootRecipe(rootRecipeFile, rootRecipe))
				{
					// Nothing we can do, exit
					Log::Error("Failed to load the root recipe file: " + rootRecipeFile.ToString());
					throw HandledException(222);
				}

				// Check if there was a root output set
				if (rootRecipe.HasOutputRoot())
				{
					// Relative to the root recipe file itself
					rootOutput = rootRecipe.GetOutputRoot();

					// Add the language sub folder
					rootOutput = rootOutput + Path(recipe.GetLanguage().GetName() + "/");

					// Add the unique recipe name/version
					rootOutput = rootOutput + Path(recipe.GetName() + "/") + Path(recipe.GetVersion().ToString() + "/");

					// Ensure there is a root relative to the file itself
					if (!rootOutput.HasRoot())
					{
						rootOutput = rootRecipeFile.GetParent() + rootOutput;
					}

					Log::Info("Override root output: " + rootOutput.ToString());
				}
			}

			// Add unique folder name for parameters
			auto parametersStream = std::stringstream();
			ValueTableWriter::Serialize(globalParameters, parametersStream);
			auto hashParameters = CryptoPP::Sha1::HashBase64(parametersStream.str());
			auto uniqueParametersFolder = Path(hashParameters + "/");
			rootOutput = rootOutput + uniqueParametersFolder;

			return rootOutput;
		}
	};
}
