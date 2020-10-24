// <copyright file="RecipeBuildManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeBuildArguments.h"
#include "RecipeExtensions.h"

namespace Soup::Build
{
	/// <summary>
	/// The recipe build manager that knows how to perform the correct build for a recipe 
	/// and all of its development and runtime dependencies
	/// </summary>
	export class RecipeBuildManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildManager"/> class.
		/// </summary>
		RecipeBuildManager(
			std::string systemCompiler,
			std::string runtimeCompiler) :
			_systemCompiler(std::move(systemCompiler)),
			_runtimeCompiler(std::move(runtimeCompiler)),
			_knownRecipes(),
			_knownRootRecipes(),
			_buildSet(),
			_systemBuildSet(),
			_systemBuildPaths(),
			_fileSystemState(0)
		{
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(
			const Path& workingDirectory,
			Recipe& recipe,
			const RecipeBuildArguments& arguments)
		{
			// Clear the build set so we check all dependencies
			_knownRecipes.clear();
			_buildSet.clear();
			_systemBuildSet.clear();

			auto userDataDirectory = GetSoupUserDataPath();

			// Load up the file system state
			Log::Info("Loading previous file system state");
			if (Runtime::FileSystemStateManager::TryLoadState(userDataDirectory, _fileSystemState))
			{
				// Load the current state of the known files
				Log::Info("Loading current file system state");
			}
			else
			{
				Log::Info("No previous file system state found");

				// Create a unique id using the lower range of the milliseconds since the epoch
				auto currentTime = std::chrono::system_clock::now();
				auto currentTimeMilliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);
				auto offsetFromEpoch = currentTimeMilliseconds.time_since_epoch();
				auto uniqueId = static_cast<uint32_t>(offsetFromEpoch.count());
				_fileSystemState = Runtime::FileSystemState(uniqueId);
			}

			// Enable log event ids to track individual builds
			int projectId = 1;
			bool isSystemBuild = false;
			bool isDevBuild = false;
			Log::EnsureListener().SetShowEventId(true);

			// TODO: A scoped listener cleanup would be nice
			try
			{
				auto rootParentSet = std::set<std::string>();
				auto rootState = ConvertToBuildState(recipe.GetTable());

				try
				{
					projectId = BuildRecipeAndDependencies(
						projectId,
						workingDirectory,
						recipe,
						arguments,
						isSystemBuild,
						isDevBuild,
						rootParentSet,
						rootState);
				}
				catch(const Runtime::BuildFailedException&)
				{
					Log::EnsureListener().SetShowEventId(false);

					Log::Info("Saving partial file system state");
					Runtime::FileSystemStateManager::SaveState(userDataDirectory, _fileSystemState);
					throw;
				}

				Log::EnsureListener().SetShowEventId(false);

				Log::Info("Saving updated file system state");
				Runtime::FileSystemStateManager::SaveState(userDataDirectory, _fileSystemState);
			}
			catch(...)
			{
				Log::EnsureListener().SetShowEventId(false);
				throw;
			}
		}

	private:
		/// <summary>
		/// Convert the root recipe table to a build Value Table entry
		/// </summary>
		Runtime::ValueTable ConvertToRootBuildState(const RecipeTable& table)
		{
			// Convert teh root table
			auto recipeState = ConvertToBuildState(table);
			
			// Initialize the Recipe state
			auto state = Runtime::ValueTable();
			state.SetValue("Recipe", Runtime::Value(std::move(recipeState)));
			return state;
		}

		/// <summary>
		/// Convert the recipe internal representation to initial build state
		/// </summary>
		Runtime::ValueTable ConvertToBuildState(const RecipeTable& table)
		{
			auto result = Runtime::ValueTable();
			for (auto& value : table)
			{
				auto buildValue = ConvertToBuildState(value.second);
				result.SetValue(value.first, std::move(buildValue));
			}

			return result;
		}

		/// <summary>
		/// Convert the recipe internal representation to initial build state
		/// </summary>
		Runtime::ValueList ConvertToBuildState(const RecipeList& list)
		{
			auto result = Runtime::ValueList();
			for (auto& value : list)
			{
				auto buildValue = ConvertToBuildState(value);
				result.GetValues().push_back(std::move(buildValue));
			}

			return result;
		}

		/// <summary>
		/// Convert the recipe internal representation to initial build state
		/// </summary>
		Runtime::Value ConvertToBuildState(const RecipeValue& value)
		{
			switch (value.GetType())
			{
				case RecipeValueType::Empty:
					return Runtime::Value();
				case RecipeValueType::Table:
					return Runtime::Value(ConvertToBuildState(value.AsTable()));
				case RecipeValueType::List:
					return Runtime::Value(ConvertToBuildState(value.AsList()));
				case RecipeValueType::String:
					return Runtime::Value(value.AsString());
				case RecipeValueType::Integer:
					return Runtime::Value(value.AsInteger());
				case RecipeValueType::Float:
					return Runtime::Value(value.AsFloat());
				case RecipeValueType::Boolean:
					return Runtime::Value(value.AsBoolean());
				default:
					throw std::runtime_error("Unknown value type.");
			}
		}

		/// <summary>
		/// Build the dependencies for the provided recipe recursively
		/// </summary>
		int BuildRecipeAndDependencies(
			int projectId,
			const Path& workingDirectory,
			Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			bool isDevBuild,
			const std::set<std::string>& parentSet,
			Runtime::ValueTable& sharedState)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(std::string(recipe.GetName()));

			// Start a new active state that is initialized to the recipe itself
			auto activeState = ConvertToRootBuildState(recipe.GetTable());

			if (recipe.HasDependencies())
			{
				for (auto dependency : recipe.GetDependencies())
				{
					// Load this package recipe
					auto packagePath = GetPackageReferencePath(workingDirectory, dependency);
					auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
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
							Log::Error("The Recipe version has not been installed: " + dependency.ToString());
							Log::HighPriority("Run `install` and try again");
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

					// Build all recursive dependencies
					projectId = BuildRecipeAndDependencies(
						projectId,
						packagePath,
						dependencyRecipe,
						arguments,
						isSystemBuild,
						false,
						activeParentSet,
						activeState);
				}
			}

			if (recipe.HasDevDependencies())
			{
				for (auto dependency : recipe.GetDevDependencies())
				{
					// Load this package recipe
					auto packagePath = GetPackageReferencePath(workingDirectory, dependency);
					auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
					Recipe dependencyRecipe = {};
					if (!TryGetRecipe(packageRecipePath, dependencyRecipe))
					{
						Log::Error("Failed to load the extension package: " + packageRecipePath.ToString());
						throw std::runtime_error("BuildRecipeAndDependencies: Failed to load dependency.");
					}

					// Ensure we do not have any circular dependencies
					if (activeParentSet.contains(dependencyRecipe.GetName()))
					{
						Log::Error("Found circular dev dependency: " + recipe.GetName() + " -> " + dependencyRecipe.GetName());
						throw std::runtime_error("BuildRecipeAndDependencies: Circular dev dependency.");
					}

					// Build all recursive dependencies
					projectId = BuildRecipeAndDependencies(
						projectId,
						packagePath,
						dependencyRecipe,
						arguments,
						true,
						true,
						activeParentSet,
						activeState);
				}
			}

			// Build the root recipe
			projectId = BuildRecipe(
				projectId,
				workingDirectory,
				recipe,
				arguments,
				isSystemBuild,
				isDevBuild,
				activeState,
				sharedState);

			// Return the updated project id after building all dependencies
			return projectId;
		}

		/// <summary>
		/// The core build that will either invoke the recipe builder directly
		/// or load a previous state
		/// </summary>
		int BuildRecipe(
			int projectId,
			const Path& workingDirectory,
			Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			bool isDevBuild,
			Runtime::ValueTable& activeState,
			Runtime::ValueTable& sharedState)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(projectId);
				Log::Diag("Running Build");

				// Select the correct build set to ensure that the different build properties 
				// required the same project to be build twice
				auto& buildSet = isSystemBuild ? _systemBuildSet : _buildSet;
				auto findBuildState = buildSet.find(recipe.GetName());
				if (findBuildState != buildSet.end())
				{
					Log::Diag("Recipe already built: " + recipe.GetName());
				}
				else
				{
					// Run the required builds in process
					// This will break the circular requirements for the core build libraries
					auto resultSharedState = RunInProcessBuild(
						projectId,
						workingDirectory,
						recipe,
						arguments,
						isSystemBuild,
						activeState);

					// Keep track of the packages we have already built
					// TODO: Verify unique names
					auto insertBuildState = buildSet.emplace(
						recipe.GetName(),
						std::move(resultSharedState));

					// Replace the find iterator so it can be used to update the shared table state
					findBuildState = insertBuildState.first;

					// Move to the next build project id
					projectId++;
				}

				// Add the shared build state Runtimed from this child build into the correct
				// Table depending on if this is a normal dependency or a dev dependency
				if (isDevBuild)
				{
					// Move the parent state from active into the parents active state
					auto& devDependenciesTable = sharedState.EnsureValue("DevDependencies").EnsureTable();
					devDependenciesTable.SetValue(
						recipe.GetName(),
						Runtime::Value(findBuildState->second));
				}
				else
				{
					// Move the parent state from active into the parents active state
					auto& dependenciesTable = sharedState.EnsureValue("Dependencies").EnsureTable();
					dependenciesTable.SetValue(
						recipe.GetName(),
						Runtime::Value(findBuildState->second));
				}

				Log::SetActiveId(0);
			}
			catch(...)
			{
				Log::SetActiveId(0);
				throw;
			}

			return projectId;
		}

		Runtime::ValueTable RunInProcessBuild(
			int projectId,
			const Path& packageRoot,
			Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			Runtime::ValueTable& activeState)
		{
			// Select the correct compiler to use
			std::string activeCompiler = "";
			if (isSystemBuild)
			{
				Log::HighPriority("System Build '" + recipe.GetName() + "'");
				activeCompiler = _systemCompiler;
			}
			else
			{
				Log::HighPriority("Build '" + recipe.GetName() + "'");
				activeCompiler = _runtimeCompiler;
			}

			// Set the default output directory to be relative to the package
			auto rootOutput = packageRoot + Path("out/");

			// Add unique location for system builds
			if (isSystemBuild)
			{
				rootOutput = rootOutput + Path("HostBuild/");
			}

			// Check for root recipe file with overrides
			Path rootRecipeFile;
			if (RecipeExtensions::TryFindRootRecipeFile(packageRoot, rootRecipeFile))
			{
				Log::Info("Found Root Recipe: '" + rootRecipeFile.ToString() + "'");
				RootRecipe rootRecipe;
				if (!TryGetRootRecipe(rootRecipeFile, rootRecipe))
				{
					// Nothing we can do, exit
					Log::Error("Failed to load the root recipe file: " + rootRecipeFile.ToString());
					throw HandledException(222);
				}

				// Today the only unique thing it can do is set the shared output directory
				if (rootRecipe.HasOutputRoot())
				{
					// Relative to the root recipe file itself
					rootOutput = rootRecipe.GetOutputRoot();

					// Add unique location for system builds
					if (isSystemBuild)
					{
						rootOutput = rootOutput + Path("HostBuild/");
					}

					// Add the unique recipe name
					rootOutput = rootOutput + Path(recipe.GetName() + "/");

					// Ensure there is a root relative to the file itself
					if (!rootOutput.HasRoot())
					{
						rootOutput = rootRecipeFile.GetParent() + rootOutput;
					}

					Log::Info("Override root output: " + rootOutput.ToString());
				}
			}

			// Build up the expected output directory for the build to be used to cache state
			auto targetDirectory = rootOutput + Runtime::BuildGenerateEngine::GetConfigurationDirectory(
				_systemCompiler,
				arguments.Flavor,
				arguments.System,
				arguments.Architecture);

			// Cache if is system build to load build tasks
			if (isSystemBuild)
			{
				_systemBuildPaths.emplace(recipe.GetName(), targetDirectory);
			}

			auto activeBuildGraph = Runtime::OperationGraph(0);
			auto sharedState = Runtime::ValueTable();
			if (!arguments.SkipGenerate)
			{
				auto buildExtensionLibraries = std::vector<Path>();

				// Run the RecipeBuild extension to inject core build tasks
				auto recipeBuildExtensionPath = Path();
				auto language = recipe.GetLanguage();
				if (language == "C++")
				{
					recipeBuildExtensionPath = Path("Soup.Cpp.dll");
				}
				else if (language == "C#")
				{
					recipeBuildExtensionPath = Path("Soup.CSharp.dll");
				}
				else
				{
					throw std::runtime_error("Unknown language.");
				}

				buildExtensionLibraries.push_back(std::move(recipeBuildExtensionPath));

				if (recipe.HasDevDependencies())
				{
					for (auto dependency : recipe.GetDevDependencies())
					{
						auto packagePath = GetPackageReferencePath(packageRoot, dependency);
						auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
						Recipe dependecyRecipe = {};
						if (!RecipeExtensions::TryLoadRecipeFromFile(packageRecipePath, dependecyRecipe))
						{
							Log::Error("Failed to load the package: " + packageRecipePath.ToString());
							throw std::runtime_error("RunInProcessBuild: Failed to load dependency.");
						}

						// Get the recipe output directory
						auto findOutputDirectory = _systemBuildPaths.find(dependecyRecipe.GetName());
						if (findOutputDirectory == _systemBuildPaths.end())
						{
							Log::Error("Failed to find the system build path to check for a build extension: " + dependecyRecipe.GetName());
							throw std::runtime_error("RunInProcessBuild: Failed to get dev dependency output directory.");
						}

						auto binaryPath = findOutputDirectory->second + Runtime::BuildGenerateEngine::GetBinaryDirectory();
						auto moduleFilename = Path(dependecyRecipe.GetName() + ".dll");
						auto libraryPath = binaryPath + moduleFilename;

						if (System::IFileSystem::Current().Exists(libraryPath))
						{
							buildExtensionLibraries.push_back(std::move(libraryPath));
						}
					}
				}

				auto buildGenerateEngine = Runtime::BuildGenerateEngine(
					_fileSystemState);
				auto generateResult = buildGenerateEngine.Generate(
					targetDirectory,
					activeCompiler,
					arguments.Flavor,
					arguments.System,
					arguments.Architecture,
					packageRoot,
					activeState,
					buildExtensionLibraries);

				activeBuildGraph = std::move(generateResult.Graph);
				sharedState = std::move(generateResult.SharedState);
			}
			else
			{
				// Load and run the previous stored state directly
				Log::Info("Loading previous operation graph as the active graph");
				if (!Runtime::OperationGraphManager::TryLoadState(
					targetDirectory,
					activeBuildGraph,
					_fileSystemState.GetId()))
				{
					throw std::runtime_error("Missing cached operation graph when skipping Runtime phase.");
				}
			}

			if (!arguments.SkipEvaluate)
			{
				if (arguments.ForceRebuild)
				{
					Log::Diag("Purge operation graph to force build");
					for (auto& activeOperationEntry : activeBuildGraph.GetOperations())
					{
						auto& activeOperationInfo = activeOperationEntry.second;
						activeOperationInfo.WasSuccessfulRun = false;
						activeOperationInfo.ObservedInput = {};
						activeOperationInfo.ObservedOutput = {};
					}
				}

				try
				{
					// Evaluate the build
					auto runner = Runtime::BuildEvaluateEngine(
						_fileSystemState,
						activeBuildGraph);
					runner.Evaluate();
				}
				catch(const Runtime::BuildFailedException& e)
				{
					Log::Info("Saving partial build state");
					Runtime::OperationGraphManager::SaveState(targetDirectory, activeBuildGraph);
					throw;
				}

				Log::Info("Saving updated build state");
				Runtime::OperationGraphManager::SaveState(targetDirectory, activeBuildGraph);

				Log::HighPriority("Done");
			}

			return sharedState;
		}

		Path GetSoupUserDataPath() const
		{
			auto result = System::IFileSystem::Current().GetUserProfileDirectory() +
				Path(".soup/");
			return result;
		}

		Path GetPackageReferencePath(const Path& workingDirectory, const PackageReference& reference) const
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
				auto packageStore = GetSoupUserDataPath() +
					Path("packages/");
				packagePath = packageStore + Path(reference.GetName()) + Path(reference.GetVersion().ToString());
			}

			return packagePath;
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
				if (RecipeExtensions::TryLoadRootRecipeFromFile(recipeFile, loadRecipe))
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
				Recipe loadRecipe;
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

	private:
		std::string _systemCompiler;
		std::string _runtimeCompiler;

		std::map<std::string, Recipe> _knownRecipes;
		std::map<std::string, RootRecipe> _knownRootRecipes;

		std::map<std::string, Runtime::ValueTable> _buildSet;
		std::map<std::string, Runtime::ValueTable> _systemBuildSet;

		std::map<std::string, Path> _systemBuildPaths;

		Runtime::FileSystemState _fileSystemState;
	};
}
