// <copyright file="ProjectManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageLock/PackageLockExtensions.h"
#include "RecipeBuildArguments.h"
#include "Recipe/RecipeExtensions.h"
#include "Recipe/RootRecipeExtensions.h"
#include "Utils/HandledException.h"

namespace Soup::Core
{
	/// <summary>
	/// The project manager that maintains the in memory representation of all build state
	/// </summary>
	export class ProjectManager
	{
	private:
		bool _hasPackageLock;
		Path _packageLockRoot;
		std::map<std::string, std::map<std::string, PackageReference>> _packageLanguageLock;
		std::map<std::string, Recipe> _knownRecipes;
		std::map<std::string, RootRecipe> _knownRootRecipes;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ProjectManager"/> class.
		/// </summary>
		ProjectManager() :
			_hasPackageLock(false),
			_packageLanguageLock(),
			_knownRecipes(),
			_knownRootRecipes()
		{
		}

		void LoadClosure(const Path& projectRoot)
		{
			// Load the package lock if present
			auto packageLockPath = projectRoot + BuildConstants::PackageLockFileName();
			_hasPackageLock = false;
			PackageLock packageLock = { };
			if (PackageLockExtensions::TryLoadFromFile(packageLockPath, packageLock))
			{
				Log::Info("Package lock loaded");

				_packageLockRoot = projectRoot;
				_packageLanguageLock = packageLock.GetProjects();
				_hasPackageLock = true;
			}

			int projectId = 1;
			auto parentSet = std::set<std::string>();

			auto recipePath = projectRoot + BuildConstants::RecipeFileName();
			Recipe recipe = {};
			if (!TryGetRecipe(recipePath, recipe))
			{
				Log::Error("The target Recipe does not exist: " + recipePath.ToString());
				Log::HighPriority("Make sure the path is correct and try again");

				// Nothing we can do, exit
				throw HandledException(1123124);
			}

			projectId = LoadClosure(recipe, projectRoot, projectId, parentSet);
		}

		int LoadClosure(
			const Recipe& recipe,
			const Path& projectRoot,
			int projectId,
			const std::set<std::string>& parentSet)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(std::string(recipe.GetName()));

			for (auto dependecyType : recipe.GetDependencyTypes())
			{
				// Same language as parent is implied
				auto implicitLanguage = recipe.GetLanguage();
				if (dependecyType == "Build")
				{
					// Build dependencies do not inherit the parent language
					// Instead, they default to C#
					implicitLanguage = "C#";
				}

				for (auto dependency : recipe.GetNamedDependencies(dependecyType))
				{
					// Load this package recipe
					auto dependencyProjectRoot = GetPackageReferencePath(projectRoot, dependency, implicitLanguage);
					auto packageRecipePath = dependencyProjectRoot + BuildConstants::RecipeFileName();
					Recipe dependencyRecipe = {};
					if (!TryGetRecipe(packageRecipePath, dependencyRecipe))
					{
						if (dependency.IsLocal())
						{
							Log::Error("The dependency Recipe does not exist: " + packageRecipePath.ToString());
							Log::HighPriority("Make sure the path is correct and try again");
						}
						else
						{
							Log::Error("The dependency Recipe version has not been installed: " + dependency.ToString() + " -> " + dependencyProjectRoot.ToString() + " [" + projectRoot.ToString() + "]");
							Log::HighPriority("Run `restore` and try again");
						}

						// Nothing we can do, exit
						throw HandledException(1234);
					}

					// Ensure we do not have any circular dependencies
					if (activeParentSet.contains(dependencyRecipe.GetName()))
					{
						Log::Error("Found circular dependency: " + recipe.GetName() + " -> " + dependencyRecipe.GetName());
						throw std::runtime_error("BuildRecipeAndDependencies: Circular dependency.");
					}

					// Discover all recursive dependencies
					projectId = LoadClosure(
						dependencyRecipe,
						dependencyProjectRoot,
						projectId,
						activeParentSet);
				}
			}

			return projectId;
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

		bool TryGetRecipe(
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

		Path GetPackageReferencePath(
			const Path& workingDirectory,
			const PackageReference& reference,
			const std::string& packageLangauge) const
		{
			// If the path is relative then combine with the working directory
			Path packagePath;
			if (reference.IsLocal())
			{
				packagePath = reference.GetPath();
				if (!packagePath.HasRoot())
				{
					packagePath = workingDirectory + packagePath;
				}
			}
			else
			{
				auto referenceLanguage = reference.HasLanguage() ? reference.GetLanguage() : packageLangauge;

				auto packageStore = GetSoupUserDataPath() + Path("packages/");
				if (_hasPackageLock)
				{
					// Find the package version in the lock
					auto packageLock = _packageLanguageLock.find(referenceLanguage);
					if (packageLock == _packageLanguageLock.end())
						throw std::runtime_error("Language [" + referenceLanguage + "] not found in lock");
					auto packageVersion = packageLock->second.find(reference.GetName());
					if (packageVersion == packageLock->second.end())
						throw std::runtime_error("Package [" + referenceLanguage + "] [" + reference.GetName() + "] not found in lock");

					if (packageVersion->second.IsLocal())
					{
						// Allow overload to local version
						packagePath = packageVersion->second.GetPath();
						if (!packagePath.HasRoot())
						{
							packagePath = _packageLockRoot + packagePath;
						}
					}
					else
					{
						// Use the package version in the lock
						packagePath = packageStore +
							Path(referenceLanguage) +
							Path(reference.GetName()) +
							Path(packageVersion->second.GetVersion().ToString());
					}
				}
				else
				{
					// Without a package lock, use the exact version specified
					packagePath = packageStore +
						Path(referenceLanguage) +
						Path(reference.GetName()) +
						Path(reference.GetVersion().ToString());
				}
			}

			return packagePath;
		}

	private:
		Path GetSoupUserDataPath() const
		{
			auto result = System::IFileSystem::Current().GetUserProfileDirectory() +
				Path(".soup/");
			return result;
		}
	};
}
