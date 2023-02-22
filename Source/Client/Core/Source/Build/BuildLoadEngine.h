// <copyright file="PackageProvider.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageLock/PackageLockExtensions.h"
#include "RecipeBuildArguments.h"
#include "Recipe/RecipeCache.h"
#include "Utils/HandledException.h"
#include "BuildConstants.h"
#include "BuiltInLanguagePackage.h"

namespace Soup::Core
{
	struct PackageLockState
	{
		bool HasPackageLock;
		Path RootDirectory;
		PackageClosures Closures;
	};

	using KnownPackageMap = std::map<std::string, std::pair<PackageId, Path>>;

	/// <summary>
	/// The build load engine that loads the package build graph.
	/// It is responsible for loading up the entire closure and validating build state to
	/// create a package graph that can be evaluated by the build runner.
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class BuildLoadEngine
	{
	private:
		const int _packageLockVersion = 4;
		const Path _builtInExtensionPath = Path("BuiltIn/");
		const Path _builtInExtensionOutPath = Path("out/");
		const std::string _builtInWrenLanguage = "Wren";
		const std::string _dependencyTypeBuild = "Build";
		const std::string _dependencyTypeTool = "Tool";
		const std::string _rootClosureName = "Root";

		// Built in languages
		const std::map<std::string, BuiltInLanguagePackage>& _builtInLanguageLookup;

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

		// Mapping from sub graph dependency to Package path to graph id
		std::map<Path, std::pair<PackageGraphId, std::vector<PackageChildInfo>>> _knownSubGraphSet;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildLoadEngine"/> class.
		/// </summary>
		BuildLoadEngine(
			const std::map<std::string, BuiltInLanguagePackage>& builtInLanguageLookup,
			const RecipeBuildArguments& arguments,
			const ValueTable& hostBuildGlobalParameters,
			RecipeCache& recipeCache) :
			_builtInLanguageLookup(builtInLanguageLookup),
			_arguments(arguments),
			_hostBuildGlobalParameters(hostBuildGlobalParameters),
			_recipeCache(recipeCache),
			_packageGraphLookup(),
			_packageLookup(),
			_knownSubGraphSet()
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
			auto toolDependencyProjects = std::vector<PackageChildInfo>();
			LoadClosure(
				*recipe,
				projectRoot,
				languagePackageName,
				rootPackageId,
				parentSet,
				knownPackageSet,
				packageLockState,
				toolDependencyProjects);

			for (auto& toolDependency : toolDependencyProjects)
				Log::Warning("Top Level Tool Dependency discarded: " + toolDependency.OriginalReference.ToString());

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
				if (packageLock.GetVersion() == _packageLockVersion)
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
				throw std::runtime_error("Closure [" + _rootClosureName + "] not found in lock [" + packageLockState.RootDirectory.ToString() + "]");

			// Find the package version in the lock
			auto findPackageLock = findClosure->second.find(packageLanguage);
			if (findPackageLock == findClosure->second.end())
				throw std::runtime_error("Language [" + _rootClosureName + "] [" + packageLanguage + "] not found in lock [" + packageLockState.RootDirectory.ToString() + "]");
			auto packageVersion = findPackageLock->second.find(packageName);
			if (packageVersion == findPackageLock->second.end())
				throw std::runtime_error("Package [" + _rootClosureName + "] [" + packageLanguage + "] [" + packageName + "] not found in lock [" + packageLockState.RootDirectory.ToString() + "]");

			auto& packageBuild = packageVersion->second.BuildValue;
			if (!packageBuild.has_value())
				throw std::runtime_error("Package [" + _rootClosureName + "] [" + packageLanguage + "] [" + packageName + "] does not have build closure [" + packageLockState.RootDirectory.ToString() + "]");

			return packageBuild.value();
		}

		Path GetRuntimePackageReferencePath(
			const Path& workingDirectory,
			const PackageReference& reference,
			const std::string& implicitLanguage,
			const PackageLockState& packageLockState) const
		{
			return GetPackageReferencePath(
				workingDirectory,
				reference,
				implicitLanguage,
				_rootClosureName,
				packageLockState);
		}

		Path GetPackageReferencePath(
			const Path& workingDirectory,
			const PackageReference& reference,
			const std::optional<std::string>& implicitLanguage,
			const std::string& closureName,
			const PackageLockState& packageLockState) const
		{
			Path packagePath;
			if (reference.IsLocal())
			{
				// Use local reference relative to package directory
				packagePath = reference.GetPath();
				if (!packagePath.HasRoot())
				{
					packagePath = workingDirectory + packagePath;
				}
			}
			else
			{
				std::string language;
				if (reference.HasLanguage())
				{
					language = reference.GetLanguage();
				}
				else
				{
					if (implicitLanguage.has_value())
					{
						language = implicitLanguage.value();
					}
					else
					{
						throw std::runtime_error("Reference [" + reference.ToString() + "] must have explicit language");
					}
				}

				auto& activeReference = GetActivePackageReference(
					reference,
					language,
					closureName,
					packageLockState);
				if (activeReference.IsLocal())
				{
					// Use local reference relative to lock directory
					packagePath = activeReference.GetPath();
					if (!packagePath.HasRoot())
					{
						packagePath = packageLockState.RootDirectory + packagePath;
					}
				}
				else
				{
					// Build the global store location path
					auto packageStore = GetSoupUserDataPath() + Path("packages/");
					auto& languageSafeName = GetLanguageSafeName(language);
					packagePath = packageStore +
						Path(languageSafeName) +
						Path(activeReference.GetName()) +
						Path(activeReference.GetVersion().ToString() + "/");
				}
			}

			return packagePath;
		}

		Path GetPackageLockPath(
			const Path& workingDirectory,
			const PackageReference& reference,
			const std::optional<std::string>& implicitLanguage,
			const std::string& closureName,
			const PackageLockState& packageLockState) const
		{
			Path packagePath;
			if (reference.IsLocal())
			{
				// Use local reference relative to package directory
				packagePath = reference.GetPath();
				if (!packagePath.HasRoot())
				{
					packagePath = workingDirectory + packagePath;
				}
			}
			else
			{
				std::string language;
				if (reference.HasLanguage())
				{
					language = reference.GetLanguage();
				}
				else
				{
					if (implicitLanguage.has_value())
					{
						language = implicitLanguage.value();
					}
					else
					{
						throw std::runtime_error("Reference [" + reference.ToString() + "] must have explicit language");
					}
				}

				auto& activeReference = GetActivePackageReference(
					reference,
					language,
					closureName,
					packageLockState);
				if (activeReference.IsLocal())
				{
					// Use local reference relative to lock directory
					packagePath = activeReference.GetPath();
					if (!packagePath.HasRoot())
					{
						packagePath = packageLockState.RootDirectory + packagePath;
					}
				}
				else
				{
					// Build the global store location path
					auto packageStore = GetSoupUserDataPath() + Path("locks/");
					auto& languageSafeName = GetLanguageSafeName(language);
					packagePath = packageStore +
						Path(languageSafeName) +
						Path(activeReference.GetName()) +
						Path(activeReference.GetVersion().ToString() + "/");
				}
			}

			return packagePath;
		}

		const PackageReference& GetActivePackageReference(
			const PackageReference& reference,
			const std::string& language,
			const std::string& closureName,
			const PackageLockState& packageLockState) const
		{
			if (packageLockState.HasPackageLock)
			{
				// Find the required closure
				auto findClosure = packageLockState.Closures.find(closureName);
				if (findClosure == packageLockState.Closures.end())
					throw std::runtime_error("Closure [" + closureName + "] not found in lock [" + packageLockState.RootDirectory.ToString() + "]");

				// Find the package version in the lock
				auto findPackageLock = findClosure->second.find(language);
				if (findPackageLock == findClosure->second.end())
					throw std::runtime_error("Language [" + closureName + "] [" + language + "] not found in lock [" + packageLockState.RootDirectory.ToString() + "]");
				auto findPackageVersion = findPackageLock->second.find(reference.GetName());
				if (findPackageVersion == findPackageLock->second.end())
					throw std::runtime_error("Package [" + closureName + "] [" + language + "] [" + reference.GetName() + "] not found in lock [" + packageLockState.RootDirectory.ToString() + "]");

				auto& lockReference = findPackageVersion->second.Reference;
				return lockReference;
			}
			else
			{
				// Without a package lock, use the exact version specified
				return reference;
			}
		}

		void LoadClosure(
			const Recipe& recipe,
			const Path& projectRoot,
			const std::string& languagePackageName,
			PackageId packageId,
			const std::set<std::string>& parentSet,
			KnownPackageMap& knownPackageSet,
			const PackageLockState& packageLockState,
			std::vector<PackageChildInfo>& toolDependencies)
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

			// Get the current package build closure
			auto buildClosureName = GetPackageBuildClosure(
				recipe.GetLanguage().GetName(),
				recipe.GetName(),
				packageLockState);

			auto dependencyProjects = std::map<std::string, std::vector<PackageChildInfo>>();
			auto buildDependencyToolDependencies = std::vector<PackageChildInfo>();
			for (auto dependencyType : recipe.GetDependencyTypes())
			{
				if (dependencyType == _dependencyTypeBuild)
				{
					auto [buildDependencies, buildToolDependencies] = LoadBuildDependencies(
						recipe,
						projectRoot,
						buildClosureName,
						packageLockState);
					dependencyProjects.emplace(_dependencyTypeBuild, std::move(buildDependencies));
					buildDependencyToolDependencies = std::move(buildToolDependencies);
				}
				else if (dependencyType == _dependencyTypeTool)
				{
					toolDependencies = LoadToolDependencies(
						recipe,
						projectRoot,
						buildClosureName,
						packageLockState);
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
			auto [languageExtensionPackageChildInfo, languageExtensionToolDependencies] =
				LoadLanguageBuildDependency(
					recipe,
					projectRoot,
					buildClosureName,
					packageLockState);
			buildDependencyToolDependencies.insert(
				buildDependencyToolDependencies.end(),
				std::make_move_iterator(languageExtensionToolDependencies.begin()),
				std::make_move_iterator(languageExtensionToolDependencies.end()));

			dependencyProjects[_dependencyTypeBuild].push_back(
				std::move(languageExtensionPackageChildInfo));

			// Tool dependencies for build dependencies are implicit dependencies for the project itself
			if (!buildDependencyToolDependencies.empty())
			{
				dependencyProjects.emplace(_dependencyTypeTool, std::move(buildDependencyToolDependencies));
			}

			// Save the package info
			_packageLookup.emplace(
				packageId,
				PackageInfo(
					packageId,
					recipe.GetName(),
					false,
					projectRoot,
					Path(),
					&recipe,
					std::move(dependencyProjects)));
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
						Log::Diag("Recipe already loaded: " + languagePackageName);
						dependencyTypeProjects.push_back(
							PackageChildInfo(dependency, false, findKnownPackage->second.first, -1));
					}
				}
				else
				{
					// Discover all recursive dependencies
					auto childPackageId = ++_uniquePackageId;
					auto toolDependencyProjects = std::vector<PackageChildInfo>();
					LoadClosure(
						*dependencyRecipe,
						dependencyProjectRoot,
						languagePackageName,
						childPackageId,
						activeParentSet,
						knownPackageSet,
						packageLockState,
						toolDependencyProjects);

					for (auto& toolDependency : toolDependencyProjects)
						Log::Warning("Runtime Dependency Tool Dependency discarded: " + toolDependency.OriginalReference.ToString());

					// Update the child project id
					dependencyTypeProjects.push_back(
						PackageChildInfo(dependency, false, childPackageId, -1));
				}
			}

			return dependencyTypeProjects;
		}

		std::pair<std::vector<PackageChildInfo>, std::vector<PackageChildInfo>> LoadBuildDependencies(
			const Recipe& recipe,
			const Path& projectRoot,
			const std::string& buildClosureName,
			const PackageLockState& packageLockState)
		{
			auto buildProjects = std::vector<PackageChildInfo>();
			auto buildToolProjects = std::vector<PackageChildInfo>();
			for (auto dependency : recipe.GetNamedDependencies(_dependencyTypeBuild))
			{
				auto [buildDependency, buildToolDependencies] = LoadBuildDependency(
					dependency,
					projectRoot,
					buildClosureName,
					packageLockState);
				buildProjects.push_back(std::move(buildDependency));

				// Propagate the build tool dependencies
				buildToolProjects.insert(
					buildToolProjects.end(),
					std::make_move_iterator(buildToolDependencies.begin()),
					std::make_move_iterator(buildToolDependencies.end()));
			}

			return std::make_pair(std::move(buildProjects), std::move(buildToolProjects));
		}

		std::vector<PackageChildInfo> LoadToolDependencies(
			const Recipe& recipe,
			const Path& projectRoot,
			const std::string& buildClosureName,
			const PackageLockState& packageLockState)
		{
			auto dependencyTypeProjects = std::vector<PackageChildInfo>();
			for (auto dependency : recipe.GetNamedDependencies(_dependencyTypeTool))
			{
				dependencyTypeProjects.push_back(
					LoadToolDependency(
						dependency,
						projectRoot,
						buildClosureName,
						packageLockState));
			}

			return dependencyTypeProjects;
		}

		std::pair<PackageChildInfo, std::vector<PackageChildInfo>> LoadBuildDependency(
			const PackageReference& dependency,
			const Path& projectRoot,
			const std::string& buildClosureName,
			const PackageLockState& packageLockState)
		{
			// Build dependencies do not inherit the parent language
			// Instead, they default to Wren
			static const std::optional<std::string> implicitLanguage = _builtInWrenLanguage;

			return LoadSubGraphDependency(
				implicitLanguage,
				dependency,
				projectRoot,
				buildClosureName,
				packageLockState);
		}

		PackageChildInfo LoadToolDependency(
			const PackageReference& dependency,
			const Path& projectRoot,
			const std::string& buildClosureName,
			const PackageLockState& packageLockState)
		{
			// Tool dependencies do not inherit the parent language
			// They must be explicitly defined
			static const std::optional<std::string> implicitLanguage = std::nullopt;

			auto [toolDependency, toolToolDependencies] = LoadSubGraphDependency(
				implicitLanguage,
				dependency,
				projectRoot,
				buildClosureName,
				packageLockState);

			for (auto& toolToolDependency : toolToolDependencies)
				Log::Warning("Tool Tool Dependency discarded: " + toolToolDependency.OriginalReference.ToString());

			return toolDependency;
		}

		std::pair<PackageChildInfo, std::vector<PackageChildInfo>> LoadSubGraphDependency(
			const std::optional<std::string>& implicitLanguage,
			const PackageReference& dependency,
			const Path& projectRoot,
			const std::string& buildClosureName,
			const PackageLockState& packageLockState)
		{
			// Load this package recipe
			auto dependencyProjectRoot = GetPackageReferencePath(
				projectRoot,
				dependency,
				implicitLanguage,
				buildClosureName,
				packageLockState);

			// Check if the package has already been processed from another graph
			auto findKnownGraph = _knownSubGraphSet.find(dependencyProjectRoot);
			if (findKnownGraph != _knownSubGraphSet.end())
			{
				// Verify the project name is unique
				Log::Diag("Graph already loaded: " + dependencyProjectRoot.ToString());
				return std::make_pair(
					PackageChildInfo(dependency, true, -1, findKnownGraph->second.first),
					findKnownGraph->second.second);
			}
			else
			{
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
				auto toolDependencyProjects = std::vector<PackageChildInfo>();
				LoadClosure(
					*dependencyRecipe,
					dependencyProjectRoot,
					languagePackageName,
					childPackageId,
					parentSet,
					knownPackageSet,
					dependencyPackageLockState,
					toolDependencyProjects);

				// Create the build graph
				auto graphId = ++_uniqueGraphId;

				// Save the package graph
				_packageGraphLookup.emplace(
					graphId,
					PackageGraph(graphId, childPackageId, _hostBuildGlobalParameters));

				// Keep track of the build graphs we have already seen
				auto insertKnown = _knownSubGraphSet.emplace(
					dependencyProjectRoot,
					std::make_pair(graphId, toolDependencyProjects));

				// Update the child project id
				return std::make_pair(
					PackageChildInfo(dependency, true, -1, graphId),
					std::move(toolDependencyProjects));
			}
		}

		std::pair<PackageChildInfo, std::vector<PackageChildInfo>> LoadLanguageBuildDependency(
			const Recipe& recipe,
			const Path& projectRoot,
			const std::string& closureName,
			const PackageLockState& packageLockState)
		{
			auto name = recipe.GetLanguage().GetName();

			// Get the active version
			auto builtInLanguageResult = _builtInLanguageLookup.find(name);
			if (builtInLanguageResult == _builtInLanguageLookup.end())
				throw std::runtime_error("Unknown language: " + name);
			auto& builtInLanguagePackage = builtInLanguageResult->second;

			// Build dependencies do not inherit the parent language
			// Instead, they default to Wren
			auto implicitLanguage = _builtInWrenLanguage;

			auto builtInExtensionReference = PackageReference(
				std::nullopt,
				builtInLanguagePackage.ExtensionName,
				builtInLanguagePackage.ExtensionVersion);
			auto& activeReference = GetActivePackageReference(
				builtInExtensionReference,
				implicitLanguage,
				closureName,
				packageLockState);

			if (!activeReference.IsLocal() && activeReference.GetVersion() == builtInLanguagePackage.ExtensionVersion)
			{
				return LoadBuiltInLanguageExtension(
					activeReference,
					builtInLanguagePackage,
					closureName,
					packageLockState);
			}
			else
			{
				return LoadBuildDependency(
					activeReference,
					projectRoot,
					closureName,
					packageLockState);
			}
		}

		std::pair<PackageChildInfo, std::vector<PackageChildInfo>> LoadBuiltInLanguageExtension(
			const PackageReference& activeReference,
			const BuiltInLanguagePackage& builtInLanguagePackage,
			const std::string& buildClosureName,
			const PackageLockState& packageLockState)
		{
			// Use the prebuilt version in the install folder
			auto processFilename = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto processDirectory = processFilename.GetParent();
			auto extensionRoot = processDirectory +
				_builtInExtensionPath +
				Path(builtInLanguagePackage.ExtensionName) +
				Path(activeReference.GetVersion().ToString() + "/");

			// Check if the package has already been processed from another graph
			auto findKnownGraph = _knownSubGraphSet.find(extensionRoot);
			if (findKnownGraph != _knownSubGraphSet.end())
			{
				// Verify the project name is unique
				Log::Diag("Graph already loaded: " + extensionRoot.ToString());
				return std::make_pair(
					PackageChildInfo(activeReference, true, -1, findKnownGraph->second.first),
					findKnownGraph->second.second);
			}
			else
			{
				auto recipePath = extensionRoot + BuildConstants::RecipeFileName();
				const Recipe* recipe;
				if (!_recipeCache.TryGetOrLoadRecipe(recipePath, recipe))
				{
					Log::Error("The built in extension Recipe does not exist: " + recipePath.ToString());
					Log::HighPriority("The installation may be corrupted");

					// Nothing we can do, exit
					throw HandledException(1123124);
				}

				auto extensionToolDependencies = std::vector<PackageChildInfo>();
				if (recipe->HasNamedDependencies(_dependencyTypeTool))
				{
					extensionToolDependencies = LoadToolDependencies(
						*recipe,
						extensionRoot,
						buildClosureName,
						packageLockState);
				}

				// Create a fake child package id
				auto packageId = ++_uniquePackageId;

				// Create the build graph
				auto graphId = ++_uniqueGraphId;

				// Save the package graph
				_packageGraphLookup.emplace(
					graphId,
					PackageGraph(graphId, packageId, {}));

				// Keep track of the build graphs we have already seen
				auto insertKnown = _knownSubGraphSet.emplace(
					extensionRoot,
					std::make_pair(graphId, extensionToolDependencies));

				// The target directory is under the root
				auto targetDirectory = extensionRoot + _builtInExtensionOutPath;

				// Save the package info
				_packageLookup.emplace(
					packageId,
					PackageInfo(
						packageId,
						builtInLanguagePackage.ExtensionName,
						true,
						std::move(extensionRoot),
						std::move(targetDirectory),
						nullptr,
						{}));

				// Update the child project id
				return std::make_pair(
					PackageChildInfo(activeReference, true, -1, graphId),
					std::move(extensionToolDependencies));
			}
		}

		const std::string& GetLanguageSafeName(const std::string& language) const
		{
			// Get the active version
			auto builtInLanguageResult = _builtInLanguageLookup.find(language);
			if (builtInLanguageResult == _builtInLanguageLookup.end())
				throw std::runtime_error("Unknown language: " + language);

			return builtInLanguageResult->second.LanguageSafeName;
		}

		Path GetSoupUserDataPath() const
		{
			auto result = System::IFileSystem::Current().GetUserProfileDirectory() +
				BuildConstants::SoupLocalStoreDirectory();
			return result;
		}
	};
}
