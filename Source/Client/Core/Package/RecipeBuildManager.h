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
			_systemCompiler(systemCompiler),
			_runtimeCompiler(runtimeCompiler),
			_knownRecipes(),
			_buildSet(),
			_systemBuildSet(),
			_fileSystemState()
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

			// Load up the known file system state
			if (!arguments.ForceRebuild)
			{
				Log::Info("Loading previous file system state");
				if (!Execute::FileSystemStateManager::TryLoadState(userDataDirectory, _fileSystemState))
				{
					Log::Info("No previous file system state found");
					_fileSystemState = Execute::FileSystemState();
				}
				else
				{
					// Load the current state of the known files
					Log::Info("Loading current file system state");
					_fileSystemState.LoadCurrentFileSystemState();
				}
			}
			else
			{
				Log::Info("Skipping file system state checks for forced rebuild");
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
				projectId = BuildRecipeAndDependencies(
					projectId,
					workingDirectory,
					recipe,
					arguments,
					isSystemBuild,
					isDevBuild,
					rootParentSet,
					rootState);

				Log::EnsureListener().SetShowEventId(false);

				Log::Info("Saving updated file system state");
				Execute::FileSystemStateManager::SaveState(userDataDirectory, _fileSystemState);
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
		Evaluate::ValueTable ConvertToRootBuildState(const RecipeTable& table)
		{
			// Convert teh root table
			auto recipeState = ConvertToBuildState(table);
			
			// Initialize the Recipe state
			auto state = Evaluate::ValueTable();
			state.SetValue("Recipe", Evaluate::Value(std::move(recipeState)));
			return state;
		}

		/// <summary>
		/// Convert the recipe internal representation to initial build state
		/// </summary>
		Evaluate::ValueTable ConvertToBuildState(const RecipeTable& table)
		{
			auto result = Evaluate::ValueTable();
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
		Evaluate::ValueList ConvertToBuildState(const RecipeList& list)
		{
			auto result = Evaluate::ValueList();
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
		Evaluate::Value ConvertToBuildState(const RecipeValue& value)
		{
			switch (value.GetType())
			{
				case RecipeValueType::Empty:
					return Evaluate::Value();
				case RecipeValueType::Table:
					return Evaluate::Value(ConvertToBuildState(value.AsTable()));
				case RecipeValueType::List:
					return Evaluate::Value(ConvertToBuildState(value.AsList()));
				case RecipeValueType::String:
					return Evaluate::Value(value.AsString());
				case RecipeValueType::Integer:
					return Evaluate::Value(value.AsInteger());
				case RecipeValueType::Float:
					return Evaluate::Value(value.AsFloat());
				case RecipeValueType::Boolean:
					return Evaluate::Value(value.AsBoolean());
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
			Evaluate::ValueTable& sharedState)
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
			Evaluate::ValueTable& activeState,
			Evaluate::ValueTable& sharedState)
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

				// Add the shared build state generated from this child build into the correct
				// Table depending on if this is a normal dependency or a dev dependency
				if (isDevBuild)
				{
					// Move the parent state from active into the parents active state
					auto& devDependenciesTable = sharedState.EnsureValue("DevDependencies").EnsureTable();
					devDependenciesTable.SetValue(
						recipe.GetName(),
						Evaluate::Value(findBuildState->second));
				}
				else
				{
					// Move the parent state from active into the parents active state
					auto& dependenciesTable = sharedState.EnsureValue("Dependencies").EnsureTable();
					dependenciesTable.SetValue(
						recipe.GetName(),
						Evaluate::Value(findBuildState->second));
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

		Evaluate::ValueTable RunInProcessBuild(
			int projectId,
			const Path& packageRoot,
			Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			Evaluate::ValueTable& activeState)
		{
			// Ensure the external build extension libraries outlive all usage in the build system
			auto activeExtensionLibraries = std::vector<System::WindowsLibrary>();

			{
				// Create a new build system for the requested build
				auto buildSystem = Evaluate::BuildSystem();
				auto buildState = Evaluate::BuildState(activeState);
				auto activeStateWrapper = Utilities::ValueTableWrapper(buildState.GetActiveState());

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

				auto outputDirectory = RecipeExtensions::GetOutputDirectory(
					_systemCompiler,
					arguments.Flavor,
					arguments.System,
					arguments.Architecture);
				auto binaryDirectory = RecipeExtensions::GetBinaryDirectory(
					_systemCompiler,
					arguments.Flavor,
					arguments.System,
					arguments.Architecture);
				auto objectDirectory = RecipeExtensions::GetObjectDirectory(
					_systemCompiler,
					arguments.Flavor,
					arguments.System,
					arguments.Architecture);

				// Set the input properties
				activeStateWrapper.EnsureValue("PackageRoot").SetValueString(packageRoot.ToString());
				activeStateWrapper.EnsureValue("BuildFlavor").SetValueString(arguments.Flavor);
				activeStateWrapper.EnsureValue("BuildSystem").SetValueString(arguments.System);
				activeStateWrapper.EnsureValue("BuildArchitecture").SetValueString(arguments.Architecture);
				activeStateWrapper.EnsureValue("CompilerName").SetValueString(activeCompiler);
				activeStateWrapper.EnsureValue("OutputDirectory").SetValueString(outputDirectory.ToString());
				activeStateWrapper.EnsureValue("BinaryDirectory").SetValueString(binaryDirectory.ToString());
				activeStateWrapper.EnsureValue("ObjectDirectory").SetValueString(objectDirectory.ToString());

				// Run all build extensions
				// Note: Keep the extension libraries open while running the build system
				// to ensure their memory is kept alive

				// Run the RecipeBuild extension to inject core build tasks
				auto recipeBuildExtensionPath = Path("Soup.RecipeBuild.dll");
				auto recipeBuildLibrary = RunBuildExtension(recipeBuildExtensionPath, buildSystem);
				activeExtensionLibraries.push_back(std::move(recipeBuildLibrary));

				// Run all build extensions
				if (recipe.HasDevDependencies())
				{
					for (auto dependency : recipe.GetDevDependencies())
					{
						auto packagePath = GetPackageReferencePath(packageRoot, dependency);
						auto libraryPath = RecipeExtensions::GetRecipeOutputPath(
							packagePath,
							binaryDirectory,
							std::string("dll"));

						if (System::IFileSystem::Current().Exists(libraryPath))
						{
							auto library = RunBuildExtension(libraryPath, buildSystem);
							activeExtensionLibraries.push_back(std::move(library));
						}
					}
				}

				// Run the build
				buildSystem.Execute(buildState);

				// Find the output object directory so we can use it in the runner
				auto buildTable = activeStateWrapper.GetValue("Build").AsTable();

				if (!arguments.SkipRun)
				{
					// Execute the build operations
					auto runner = Execute::BuildRunner(packageRoot, _fileSystemState);
					auto buildOperations = Utilities::BuildOperationListWrapper(buildState.GetBuildOperations());
					runner.Execute(
						buildOperations,
						outputDirectory,
						arguments.ForceRebuild);
				}

				// Return only the build state that is to be passed to the downstream builds
				// This allows the extension dlls to be released and the operations deleted
				return buildState.RetrieveSharedState();
			}
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

		System::WindowsLibrary RunBuildExtension(
			Path& libraryPath,
			IBuildSystem& buildSystem)
		{
			try
			{
				Log::Diag("Running Build Extension: " + libraryPath.ToString());
				auto library = System::WindowsDynamicLibraryManager::LoadDynamicLibrary(
					libraryPath.ToString().c_str());
				auto function = (int(*)(IBuildSystem&))library.GetFunction(
					"RegisterBuildExtension");
				auto result = function(buildSystem);
				if (result != 0)
				{
					Log::Error("Build Extension Failed: " + std::to_string(result));
				}
				else
				{
					Log::Info("Build Extension Done");
				}

				// Keep the library open to ensure the registered tasks are not lost
				return library;
			}
			catch (...)
			{
				Log::Error("Build Extension Failed!");
				throw;
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
				if (RecipeExtensions::TryLoadFromFile(recipeFile, loadRecipe))
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
		std::map<std::string, Evaluate::ValueTable> _buildSet;
		std::map<std::string, Evaluate::ValueTable> _systemBuildSet;

		Execute::FileSystemState _fileSystemState;
	};
}
