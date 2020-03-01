// <copyright file="RecipeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
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
		static bool TryLoadFromFile(
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
				result = RecipeToml::Deserialize(*file);
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
			RecipeToml::Serialize(recipe, file->GetStream());
		}

		/// <summary>
		/// Get the recipe output path
		/// </summary>
		static Path GetRecipeOutputPath(const Path& packagePath, const Path& binaryDirectory, const std::string& outputFileExtension)
		{
			auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
			Recipe dependecyRecipe = {};
			if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependecyRecipe))
			{
				Log::Error("Failed to load the dependency package: " + packageRecipePath.ToString());
				throw std::runtime_error("GetRecipeOutputPath: Failed to load dependency.");
			}

			auto packageBinaryPath = packagePath + binaryDirectory;
			auto moduleFilename = Path(std::string(dependecyRecipe.GetName()) + "." + outputFileExtension);
			auto modulePath = packageBinaryPath + moduleFilename;

			return modulePath;
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

		static Path GetObjectDirectory(const std::string& compiler, const std::string& configuration)
		{
			// Setup the output directories
			auto outputDirectory = Path("out");
			auto objectDirectory = outputDirectory + Path("obj");
			return objectDirectory + Path(compiler) + Path(configuration);
		}

		static Path GetBinaryDirectory(const std::string& compiler, const std::string& configuration)
		{
			// Setup the output directories
			auto outputDirectory = Path("out");
			auto binaryDirectory = outputDirectory + Path("bin");
			return binaryDirectory + Path(compiler) + Path(configuration);
		}
	};
}
