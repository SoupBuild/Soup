// <copyright file="RecipeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"
#include "RecipeJson.h"

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
			if (!IFileSystem::Current().Exists(recipeFile))
			{
				Log::Info("Recipe file does not exist.");
				return false;
			}

			// Open the file to read from
			auto file = IFileSystem::Current().OpenRead(recipeFile);

			// Read the contents of the recipe file
			try
			{
				result = RecipeJson::Deserialize(*file);
				return true;
			}
			catch (std::exception& ex)
			{
				Log::Diag(std::string("Deserialze Threw: ") + ex.what());
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
			auto file = IFileSystem::Current().OpenWrite(recipeFile);

			// Write the recipe to the file stream
			RecipeJson::Serialize(recipe, *file);
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
			auto moduleFilename = Path(dependecyRecipe.GetName() + "." + outputFileExtension);
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

		static void GenerateDependecyStaticLibraryClosure(
			const ICompiler& compiler,
			const std::string& configuration,
			const Path& workingDirectory,
			const Recipe& recipe,
			std::vector<Path>& closure)
		{
			if (recipe.HasDependencies())
			{
				GenerateDependecyStaticLibraryClosure(
					compiler,
					configuration,
					workingDirectory,
					recipe.GetDependencies(),
					closure);
			}
		}

		static void GenerateDependecyStaticLibraryClosure(
			const ICompiler& compiler,
			const std::string& configuration,
			const Path& workingDirectory,
			const std::vector<PackageReference>& dependencies,
			std::vector<Path>& closure)
		{
			for (auto& dependecy : dependencies)
			{
				// Load this package recipe
				auto dependencyPackagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
				auto packageRecipePath = dependencyPackagePath + Path(Constants::RecipeFileName);
				Recipe dependencyRecipe = {};
				if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependencyRecipe))
				{
					Log::Error("Failed to load the dependency package: " + packageRecipePath.ToString());
					throw std::runtime_error("GenerateDependecyStaticLibraryClosure: Failed to load dependency.");
				}

				// Add this dependency if it is a library
				if (dependencyRecipe.GetType() == RecipeType::StaticLibrary || dependencyRecipe.GetType() == RecipeType::DynamicLibrary)
				{
					auto dependencyStaticLibrary = 
						dependencyPackagePath +
						GetBinaryDirectory(compiler, configuration) +
						Path(dependencyRecipe.GetName() + "." + std::string(compiler.GetStaticLibraryFileExtension()));
					closure.push_back(std::move(dependencyStaticLibrary));

					// Add transient dependencies for a static library to ensure all symbols are discoverable
					if (dependencyRecipe.GetType() == RecipeType::StaticLibrary)
					{
						// Add all recursive dependencies
						GenerateDependecyStaticLibraryClosure(
							compiler,
							configuration,
							dependencyPackagePath,
							dependencyRecipe,
							closure);
					}
				}
				else
				{
					throw std::runtime_error("Cannot reference a non library dependency.");
				}
			}
		}

		static void GenerateDependecyDynamicLibraryClosure(
			const ICompiler& compiler,
			const std::string& configuration,
			const Path& workingDirectory,
			const Recipe& recipe,
			std::vector<Path>& closure)
		{
			if (recipe.HasDependencies())
			{
				for (auto& dependecy : recipe.GetDependencies())
				{
					// Load this package recipe
					auto dependencyPackagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
					auto packageRecipePath = dependencyPackagePath + Path(Constants::RecipeFileName);
					Recipe dependencyRecipe = {};
					if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependencyRecipe))
					{
						Log::Error("Failed to load the dependency package: " + packageRecipePath.ToString());
						throw std::runtime_error("GenerateDependecyDynamicLibraryClosure: Failed to load dependency.");
					}

					// Add this dependency if it is a dynamic library
					if (dependencyRecipe.GetType() == RecipeType::DynamicLibrary)
					{
						auto dependencyLibrary = 
							dependencyPackagePath +
							GetBinaryDirectory(compiler, configuration) +
							Path(dependencyRecipe.GetName() + "." + std::string(compiler.GetDynamicLibraryFileExtension()));
						closure.push_back(std::move(dependencyLibrary));
					}
				}
			}
		}

		static Path GetObjectDirectory(const ICompiler& compiler, const std::string& configuration)
		{
			// Setup the output directories
			auto outputDirectory = Path("out");
			auto objectDirectory = outputDirectory + Path("obj");
			return objectDirectory + Path(compiler.GetName()) + Path(configuration);
		}

		static Path GetBinaryDirectory(const ICompiler& compiler, const std::string& configuration)
		{
			// Setup the output directories
			auto outputDirectory = Path("out");
			auto binaryDirectory = outputDirectory + Path("bin");
			return binaryDirectory + Path(compiler.GetName()) + Path(configuration);
		}
	};
}
