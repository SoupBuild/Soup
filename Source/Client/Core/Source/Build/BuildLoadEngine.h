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
	struct PackageLockState
	{
		bool HasPackageLock;
		Path RootDirectory;
		PackageClosures Closures;
	};

	/// <summary>
	/// The build load engine that loads the package build graph.
	/// It is responsible for loading up the entire closure and validating build state to
	/// create a package graph that can be evaluated by the build runner.
	/// </summary>
	using KnownPackageMap = std::map<std::string, std::pair<PackageId, Path>>;
	export class BuildLoadEngine
	{
	private:
		const Path _builtInExtensionPath = Path("Extensions/");
		const std::string _buildDependencyType = "Build";
		const std::string _builtInCppLanguage = "C++";
		const std::string _builtInCppExtensionVersion = "0.4.0";
		const Path _builtInCppExtensionPath = Path("Soup.Cpp/");
		const Path _builtInCppExtensionFilename = Path("Soup.Cpp.dll");
		const std::string _builtInCSharpLanguage = "C#";
		const std::string _builtInCSharpExtensionVersion = "0.7.0";
		const Path _builtInCSharpExtensionPath = Path("Soup.CSharp/");
		const Path _builtInCSharpExtensionFilename = Path("Soup.CSharp.dll");
		const std::string _rootClosureName = "Root";

		// Arguments
		const RecipeBuildArguments& _arguments;

		// System Parameters
		const ValueTable& _hostBuildGlobalParameters;

		// Shared Runtime State
		RecipeCache& _recipeCache;

		int _uniquePackageId;
		int _uniqueGraphId;

		// The package build graph results
		PackageGraphLookupMap _packageGraphLookup;
		PackageLookupMap _packageLookup;

		// Mapping from build dependency to Package path to graph id
		std::map<Path, PackageGraphId> _knownGraphSet;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildLoadEngine"/> class.
		/// </summary>
		BuildLoadEngine(
			const RecipeBuildArguments& arguments,
			const ValueTable& hostBuildGlobalParameters,
			RecipeCache& recipeCache) :
			_arguments(arguments),
			_hostBuildGlobalParameters(hostBuildGlobalParameters),
			_recipeCache(recipeCache),
			_packageGraphLookup(),
			_packageLookup(),
			_knownGraphSet()
		{
		}

		/// <summary>
		/// Load the package lock and using it recursively load up all packages that are a part of the build closure
		/// Validates that there are no circular dependencies and all required packages are available
		/// </summary>
		PackageProvider Load()
		{
			const Path& projectRoot = _arguments.WorkingDirectory;

			// Load the package lock if present from project folder
			auto packageLockState = LoadPackageLockIfPresent(projectRoot);

			auto recipePath = projectRoot + BuildConstants::RecipeFileName();
			const Recipe* recipe;
			if (!_recipeCache.TryGetOrLoadRecipe(recipePath, recipe))
			{
				Log::Error("The target Recipe does not exist: " + recipePath.ToString());
				Log::HighPriority("Make sure the path is correct and try again");

				// Nothing we can do, exit
				throw HandledException(1123124);
			}

			// Create the root build graph
			_uniqueGraphId = 0;
			_uniquePackageId = 0;
			PackageGraphId rootGraphId = ++_uniqueGraphId;
			PackageId rootPackageId = ++_uniquePackageId;

			// Save the package graph
			_packageGraphLookup.emplace(
				rootGraphId,
				PackageGraph(rootGraphId, rootPackageId, _arguments.GlobalParameters));

			auto languagePackageName = recipe->GetLanguage().GetName() + "|" + recipe->GetName();
			auto parentSet = std::set<std::string>();
			auto knownPackageSet = KnownPackageMap();
			LoadClosure(
				*recipe,
				projectRoot,
				languagePackageName,
				rootPackageId,
				parentSet,
				knownPackageSet,
				packageLockState);

			return PackageProvider(rootGraphId, std::move(_packageGraphLookup), std::move(_packageLookup));
		}

	private:
		PackageLockState LoadPackageLockIfPresent(const Path& projectRoot)
		{
			// Load the package lock if present
			auto packageLockPath = projectRoot + BuildConstants::PackageLockFileName();
			auto packageLockState = PackageLockState();
			packageLockState.HasPackageLock = false;
			PackageLock packageLock = {};
			if (PackageLockExtensions::TryLoadFromFile(packageLockPath, packageLock))
			{
				Log::Info("Package lock loaded");
				if (packageLock.GetVersion() == 3)
				{
					packageLockState.RootDirectory = projectRoot;
					packageLockState.Closures = packageLock.GetClosures();
					packageLockState.HasPackageLock = true;
				}
				else
				{
					Log::Warning("Unknown package lock version.");
				}
			}

			return packageLockState;
		}

		std::string GetPackageBuildClosure(
			const std::string& packageLanguage,
			const std::string& packageName,
			const PackageLockState& packageLockState) const
		{
			if (!packageLockState.HasPackageLock)
			{
				return "";
			}

			// Find the required closure
			auto findClosure = packageLockState.Closures.find(_rootClosureName);
			if (findClosure == packageLockState.Closures.end())
				throw std::runtime_error("Closure [" + _rootClosureName + "] not found in lock");

			// Find the package version in the lock
			auto findPackageLock = findClosure->second.find(packageLanguage);
			if (findPackageLock == findClosure->second.end())
				throw std::runtime_error("Language [" + _rootClosureName + "] [" + packageLanguage + "] not found in lock");
			auto packageVersion = findPackageLock->second.find(packageName);
			if (packageVersion == findPackageLock->second.end())
				throw std::runtime_error("Package [" + _rootClosureName + "] [" + packageLanguage + "] [" + packageName + "] not found in lock");

			auto& packageBuild = packageVersion->second.second;
			if (!packageBuild.has_value())
				throw std::runtime_error("Package [" + _rootClosureName + "] [" + packageLanguage + "] [" + packageName + "] does not have build closure");

			return packageBuild.value();
		}

		Path GetRuntimePackageReferencePath(
			const Path& workingDirectory,
			const PackageReference& reference,
			const std::string& packageLanguage,
			const PackageLockState& packageLockState) const
		{
			return GetPackageReferencePath(
				workingDirectory,
				reference,
				packageLanguage,
				_rootClosureName,
				packageLockState);
		}

		Path GetPackageReferencePath(
			const Path& workingDirectory,
			const PackageReference& reference,
			const std::string& packageLanguage,
			const std::string& closureName,
			const PackageLockState& packageLockState) const
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
				auto referenceLanguage = reference.HasLanguage() ? reference.GetLanguage() : packageLanguage;
				auto packageStore = GetSoupUserDataPath() + Path("packages/");
				if (packageLockState.HasPackageLock)
				{
					// Find the required closure
					auto findClosure = packageLockState.Closures.find(closureName);
					if (findClosure == packageLockState.Closures.end())
						throw std::runtime_error("Closure [" + closureName + "] not found in lock");

					// Find the package version in the lock
					auto findPackageLock = findClosure->second.find(referenceLanguage);
					if (findPackageLock == findClosure->second.end())
						throw std::runtime_error("Language [" + closureName + "] [" + referenceLanguage + "] not found in lock");
					auto packageVersion = findPackageLock->second.find(reference.GetName());
					if (packageVersion == findPackageLock->second.end())
						throw std::runtime_error("Package [" + closureName + "] [" + referenceLanguage + "] [" + reference.GetName() + "] not found in lock");

					auto& packageReference = packageVersion->second.first;
					if (packageReference.IsLocal())
					{
						// Allow overload to local version
						packagePath = packageReference.GetPath();
						if (!packagePath.HasRoot())
						{
							packagePath = packageLockState.RootDirectory + packagePath;
						}
					}
					else
					{
						// Use the package version in the lock
						packagePath = packageStore +
							Path(referenceLanguage) +
							Path(reference.GetName()) +
							Path(packageReference.GetVersion().ToString());
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

		Path GetPackageLockPath(
			const Path& workingDirectory,
			const PackageReference& reference,
			const std::string& packageLanguage,
			const std::string& closureName,
			const PackageLockState& packageLockState) const
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
				auto referenceLanguage = reference.HasLanguage() ? reference.GetLanguage() : packageLanguage;
				auto packageStore = GetSoupUserDataPath() + Path("locks/");
				if (packageLockState.HasPackageLock)
				{
					// Find the required closure
					auto findClosure = packageLockState.Closures.find(closureName);
					if (findClosure == packageLockState.Closures.end())
						throw std::runtime_error("Closure [" + closureName + "] not found in lock");

					// Find the package version in the lock
					auto findPackageLock = findClosure->second.find(referenceLanguage);
					if (findPackageLock == findClosure->second.end())
						throw std::runtime_error("Language [" + closureName + "] [" + referenceLanguage + "] not found in lock");
					auto packageVersion = findPackageLock->second.find(reference.GetName());
					if (packageVersion == findPackageLock->second.end())
						throw std::runtime_error("Package [" + closureName + "] [" + referenceLanguage + "] [" + reference.GetName() + "] not found in lock");

					auto& packageReference = packageVersion->second.first;
					if (packageReference.IsLocal())
					{
						// Allow overload to local version
						packagePath = packageReference.GetPath();
						if (!packagePath.HasRoot())
						{
							packagePath = packageLockState.RootDirectory + packagePath;
						}
					}
					else
					{
						// Use the package version in the lock
						packagePath = packageStore +
							Path(referenceLanguage) +
							Path(reference.GetName()) +
							Path(packageReference.GetVersion().ToString());
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

		void LoadClosure(
			const Recipe& recipe,
			const Path& projectRoot,
			const std::string& languagePackageName,
			PackageId packageId,
			const std::set<std::string>& parentSet,
			KnownPackageMap& knownPackageSet,
			const PackageLockState& packageLockState)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(std::string(recipe.GetName()));

			// Keep track of the packages we have already seen
			auto insertKnown = knownPackageSet.emplace(
				languagePackageName,
				std::make_pair(packageId, projectRoot));

			// Same language as parent is implied
			if (!recipe.HasLanguage())
				throw std::runtime_error("Recipe does not have a language reference.");

			auto dependencyProjects = std::map<std::string, std::vector<PackageChildInfo>>();
			for (auto dependencyType : recipe.GetDependencyTypes())
			{
				bool isBuildDependency = dependencyType == _buildDependencyType;
				if (isBuildDependency)
				{
					auto dependencyTypeProjects = LoadBuildDependencies(
						recipe,
						projectRoot,
						packageLockState);
					dependencyProjects.emplace(dependencyType, std::move(dependencyTypeProjects));
				}
				else
				{
					auto dependencyTypeProjects = LoadRuntimeDependencies(
						recipe,
						projectRoot,
						dependencyType,
						activeParentSet,
						knownPackageSet,
						packageLockState);
					dependencyProjects.emplace(dependencyType, std::move(dependencyTypeProjects));
				}
			}

			// Add the language as a build dependency
			auto languageExtension = LoadLanguageBuildDependency(recipe);
			// TODO: dependencyProjects[_buildDependencyType].push_back(std::move(languageDependency));

			// Save the package info
			_packageLookup.emplace(
				packageId,
				PackageInfo(packageId, projectRoot, recipe, std::move(languageExtension), std::move(dependencyProjects)));
		}

		std::vector<PackageChildInfo> LoadRuntimeDependencies(
			const Recipe& recipe,
			const Path& projectRoot,
			const std::string& dependencyType,
			const std::set<std::string>& activeParentSet,
			KnownPackageMap& knownPackageSet,
			const PackageLockState& packageLockState)
		{
			auto implicitLanguage = recipe.GetLanguage().GetName();

			auto dependencyTypeProjects = std::vector<PackageChildInfo>();
			for (auto dependency : recipe.GetNamedDependencies(dependencyType))
			{
				// Load this package recipe
				auto dependencyProjectRoot = GetRuntimePackageReferencePath(
					projectRoot,
					dependency,
					implicitLanguage,
					packageLockState);
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
				auto findKnownPackage = knownPackageSet.find(languagePackageName);
				if (findKnownPackage != knownPackageSet.end())
				{
					// Verify the project name is unique
					if (findKnownPackage->second.second != dependencyProjectRoot)
					{
						Log::Error("Recipe with this name already exists: " + languagePackageName + " [" + projectRoot.ToString() + "] [" + findKnownPackage->second.second.ToString() + "]");
						throw std::runtime_error("Recipe name not unique");
					}
					else
					{
						Log::Diag("Recipe closure already loaded: " + languagePackageName);
						dependencyTypeProjects.push_back(PackageChildInfo(dependency, false, findKnownPackage->second.first, -1));
					}
				}
				else
				{
					// Discover all recursive dependencies
					auto childPackageId = ++_uniquePackageId;
					LoadClosure(
						*dependencyRecipe,
						dependencyProjectRoot,
						languagePackageName,
						childPackageId,
						activeParentSet,
						knownPackageSet,
						packageLockState);
						
					// Update the child project id
					dependencyTypeProjects.push_back(PackageChildInfo(dependency, false, childPackageId, -1));
				}
			}

			return dependencyTypeProjects;
		}

		std::vector<PackageChildInfo> LoadBuildDependencies(
			const Recipe& recipe,
			const Path& projectRoot,
			const PackageLockState& packageLockState)
		{
			// Build dependencies do not inherit the parent language
			// Instead, they default to C#
			auto implicitLanguage = _builtInCSharpLanguage;

			// Get the current package build closure
			auto buildClosureName = GetPackageBuildClosure(
				recipe.GetLanguage().GetName(),
				recipe.GetName(),
				packageLockState);

			auto dependencyTypeProjects = std::vector<PackageChildInfo>();
			for (auto dependency : recipe.GetNamedDependencies(_buildDependencyType))
			{
				// Load this package recipe
				auto dependencyProjectRoot = GetPackageReferencePath(
					projectRoot,
					dependency,
					implicitLanguage,
					buildClosureName,
					packageLockState);
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

				// Check if the package has already been processed from another graph
				auto findKnownGraph = _knownGraphSet.find(dependencyProjectRoot);
				if (findKnownGraph != _knownGraphSet.end())
				{
					// Verify the project name is unique
					Log::Diag("Graph closure already loaded: " + dependencyProjectRoot.ToString());
					dependencyTypeProjects.push_back(PackageChildInfo(dependency, true, -1, findKnownGraph->second));
				}
				else
				{
					// Reset parent set to allow uniqueness within sub graph
					auto parentSet = std::set<std::string>();
					auto knownPackageSet = KnownPackageMap();

					// Load the package lock if present for the build dependency
					auto packageLockRoot = GetPackageLockPath(
						projectRoot,
						dependency,
						implicitLanguage,
						buildClosureName,
						packageLockState);
					auto dependencyPackageLockState = LoadPackageLockIfPresent(packageLockRoot);

					// Discover all recursive dependencies
					auto languagePackageName = dependencyRecipe->GetLanguage().GetName() + "|" + dependencyRecipe->GetName();
					auto childPackageId = ++_uniquePackageId;
					LoadClosure(
						*dependencyRecipe,
						dependencyProjectRoot,
						languagePackageName,
						childPackageId,
						parentSet,
						knownPackageSet,
						dependencyPackageLockState);

					// Create the build graph
					auto graphId = ++_uniqueGraphId;

					// Save the package graph
					_packageGraphLookup.emplace(
						graphId,
						PackageGraph(graphId, childPackageId, _hostBuildGlobalParameters));

					// Update the child project id
					dependencyTypeProjects.push_back(PackageChildInfo(dependency, true, -1, graphId));
				}
			}

			return dependencyTypeProjects;
		}

		Path LoadLanguageBuildDependency(const Recipe& recipe)
		{
			auto name = recipe.GetLanguage().GetName();
			if (name == _builtInCSharpLanguage)
			{
				auto processFilename = System::IProcessManager::Current().GetCurrentProcessFileName();
				auto processDirectory = processFilename.GetParent();
				return processDirectory +
					_builtInExtensionPath +
					_builtInCSharpExtensionPath +
					Path(_builtInCSharpExtensionVersion) +
					_builtInCSharpExtensionFilename;
			}
			else if (name == _builtInCppLanguage)
			{
				auto processFilename = System::IProcessManager::Current().GetCurrentProcessFileName();
				auto processDirectory = processFilename.GetParent();
				return processDirectory +
					_builtInExtensionPath +
					_builtInCppExtensionPath +
					Path(_builtInCppExtensionVersion) +
					_builtInCppExtensionFilename;
			}
			else
			{
				throw std::runtime_error("Unknown language extension path");
			}
		}

		Path GetSoupUserDataPath() const
		{
			auto result = System::IFileSystem::Current().GetUserProfileDirectory() +
				BuildConstants::GetSoupLocalStoreDirectory();
			return result;
		}
	};
}
