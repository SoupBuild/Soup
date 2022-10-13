// <copyright file="PackageProvider.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageLock/PackageLockExtensions.h"
#include "RecipeBuildArguments.h"
#include "Recipe/RecipeCache.h"
#include "Utils/HandledException.h"
#include "BuildConstants.h"

namespace Soup::Core
{
	/// <summary>
	/// The build load engine that loads the package build graph.
	/// It is responsible for loading up the entire closure and validating build state to 
	/// create a package graph that can be evaluated by the build runner.
	/// </summary>
	export class BuildLoadEngine
	{
	private:
		const std::string _buildDependencyType = "Build";
		const std::string _builtInCSharpLanguage = "C#";

		// Shared Runtime State
		RecipeCache& _recipeCache;

		bool _hasPackageLock;
		Path _packageLockRoot;
		std::map<std::string, std::map<std::string, PackageReference>> _packageLanguageLock;
		std::map<int, PackageInfo> _packageLookup;
		std::map<std::string, std::pair<int, Path>> _knownSet;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildLoadEngine"/> class.
		/// </summary>
		BuildLoadEngine(RecipeCache& recipeCache) :
			_recipeCache(recipeCache),
			_hasPackageLock(false),
			_packageLanguageLock(),
			_packageLookup(),
			_knownSet()
		{
		}

		/// <summary>
		/// Load the package lock and using it recursively load up all packages that are a part of the build closure
		/// Validates that there are no circular dependencies and all required packages are available
		/// </summary>
		PackageProvider Load(const Path& projectRoot)
		{
			// Load the package lock if present
			auto packageLockPath = projectRoot + BuildConstants::PackageLockFileName();
			_hasPackageLock = false;
			PackageLock packageLock = { };
			if (PackageLockExtensions::TryLoadFromFile(packageLockPath, packageLock))
			{
				Log::Info("Package lock loaded");
				if (packageLock.GetVersion() == 2)
				{
					_packageLockRoot = projectRoot;
					auto closures = packageLock.GetClosures();
					_packageLanguageLock = closures["Root"];
					_hasPackageLock = true;
				}
				else
				{
					Log::Warning("Unknown package lock version.");
				}
			}

			int projectId = 1;
			auto parentSet = std::set<std::string>();

			auto recipePath = projectRoot + BuildConstants::RecipeFileName();
			const Recipe* recipe;
			if (!_recipeCache.TryGetOrLoadRecipe(recipePath, recipe))
			{
				Log::Error("The target Recipe does not exist: " + recipePath.ToString());
				Log::HighPriority("Make sure the path is correct and try again");

				// Nothing we can do, exit
				throw HandledException(1123124);
			}

			auto finalProjectId = LoadClosure(*recipe, projectRoot, projectId, parentSet);
			(finalProjectId);

			return PackageProvider(std::move(_packageLookup), projectId);
		}

	private:
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

		int LoadClosure(
			const Recipe& recipe,
			const Path& projectRoot,
			int projectId,
			const std::set<std::string>& parentSet)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(std::string(recipe.GetName()));

			// Same language as parent is implied
			if (!recipe.HasLanguage())
				throw std::runtime_error("Recipe does not have a language reference.");

			auto childProjectId = projectId;
			auto dependencyProjects = std::map<std::string, std::vector<std::pair<PackageReference, int>>>();
			for (auto dependencyType : recipe.GetDependencyTypes())
			{
				bool isBuildDependency = dependencyType == _buildDependencyType;
				auto implicitLanguage = recipe.GetLanguage().GetName();
				if (isBuildDependency)
				{
					// Build dependencies do not inherit the parent language
					// Instead, they default to C#
					implicitLanguage = _builtInCSharpLanguage;
				}

				auto dependencyTypeProjects = std::vector<std::pair<PackageReference, int>>();
				for (auto dependency : recipe.GetNamedDependencies(dependencyType))
				{
					// Load this package recipe
					auto dependencyProjectRoot = GetPackageReferencePath(projectRoot, dependency, implicitLanguage);
					auto packageRecipePath = dependencyProjectRoot + BuildConstants::RecipeFileName();
					const Recipe* dependencyRecipe;
					if (!_recipeCache.TryGetOrLoadRecipe(packageRecipePath, dependencyRecipe))
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
					if (activeParentSet.contains(dependencyRecipe->GetName()))
					{
						Log::Error("Found circular dependency: " + recipe.GetName() + " -> " + dependencyRecipe->GetName());
						throw std::runtime_error("BuildRecipeAndDependencies: Circular dependency.");
					}

					// Check if the package has already been processed from another reference
					auto languagePackageName = dependencyRecipe->GetLanguage().GetName() + "|" + dependencyRecipe->GetName();
					auto findKnown = _knownSet.find(languagePackageName);
					if (findKnown != _knownSet.end())
					{
						// Verify the project name is unique
						if (findKnown->second.second != dependencyProjectRoot)
						{
							Log::Error("Recipe with this name already exists: " + languagePackageName + " [" + projectRoot.ToString() + "] [" + findKnown->second.second.ToString() + "]");
							throw std::runtime_error("Recipe name not unique");
						}
						else
						{
							Log::Diag("Recipe closure already loaded: " + languagePackageName);
							dependencyTypeProjects.push_back(std::make_pair(dependency, findKnown->second.first));
						}
					}
					else
					{
						// Load the closure for real
						// Update the child project id
						childProjectId++;
						dependencyTypeProjects.push_back(std::make_pair(dependency, childProjectId));

						// Keep track of the packages we have already built
						auto insertKnown = _knownSet.emplace(
							languagePackageName,
							std::make_pair(childProjectId, dependencyProjectRoot));

						// Discover all recursive dependencies
						childProjectId = LoadClosure(
							*dependencyRecipe,
							dependencyProjectRoot,
							childProjectId,
							activeParentSet);
					}

				}

				dependencyProjects.emplace(dependencyType, std::move(dependencyTypeProjects));
			}

			// Save the package info
			_packageLookup.emplace(
				projectId,
				 PackageInfo(projectId, projectRoot, recipe, std::move(dependencyProjects)));

			return childProjectId;
		}

		Path GetSoupUserDataPath() const
		{
			auto result = System::IFileSystem::Current().GetUserProfileDirectory() +
				BuildConstants::GetSoupLocalStoreDirectory();
			return result;
		}
	};
}
