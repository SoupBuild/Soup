// <copyright file="RecipeBuildManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeBuildArguments.h"
#include "RecipeExtensions.h"
#include "Build/Runner/BuildRunner.h"
#include "Build/System/BuildSystem.h"

namespace Soup
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
			_runtimeCompiler(runtimeCompiler)
		{
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(
			const Path& workingDirectory,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments)
		{
			// Clear the build set so we check all dependencies
			_buildSet.clear();

			// Enable log event ids to track individual builds
			int projectId = 1;
			bool isSystemBuild = false;
			Log::EnsureListener().SetShowEventId(true);

			// TODO: A scoped listener cleanup would be nice
			try
			{
				auto rootParentSet = std::set<std::string>();
				auto rootState = Build::BuildState();
				projectId = BuildRecipeAndDependencies(
					projectId,
					workingDirectory,
					recipe,
					arguments,
					isSystemBuild,
					rootParentSet,
					rootState);

				Log::EnsureListener().SetShowEventId(false);
			}
			catch(...)
			{
				Log::EnsureListener().SetShowEventId(false);
				throw;
			}
		}

	private:
		/// <summary>
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		int BuildRecipeAndDependencies(
			int projectId,
			const Path& workingDirectory,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			const std::set<std::string>& parentSet,
			Build::BuildState& parentState)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(recipe.GetName());

			// Start a new active state
			auto activeState = Build::BuildState();

			if (recipe.HasDependencies())
			{
				for (auto dependecy : recipe.GetDependencies())
				{
					// Load this package recipe
					auto packagePath = GetPackageReferencePath(workingDirectory, dependecy);
					auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
					Recipe dependecyRecipe = {};
					if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependecyRecipe))
					{
						Log::Error("Failed to load the dependency package: " + packageRecipePath.ToString());
						throw std::runtime_error("BuildRecipeAndDependencies: Failed to load dependency.");
					}

					// Ensure we do not have any circular dependencies
					if (activeParentSet.contains(dependecyRecipe.GetName()))
					{
						Log::Error("Found circular dependency: " + recipe.GetName() + " -> " + dependecyRecipe.GetName());
						throw std::runtime_error("BuildRecipeAndDependencies: Circular dependency.");
					}

					// Build all recursive dependencies
					projectId = BuildRecipeAndDependencies(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments,
						isSystemBuild,
						activeParentSet,
						activeState);
				}
			}

			if (recipe.HasDevDependencies())
			{
				for (auto dependecy : recipe.GetDevDependencies())
				{
					// Load this package recipe
					auto packagePath = GetPackageReferencePath(workingDirectory, dependecy);
					auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
					Recipe dependecyRecipe = {};
					if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependecyRecipe))
					{
						Log::Error("Failed to load the dependency package: " + packageRecipePath.ToString());
						throw std::runtime_error("BuildRecipeAndDependencies: Failed to load dependency.");
					}

					// Ensure we do not have any circular dependencies
					if (activeParentSet.contains(dependecyRecipe.GetName()))
					{
						Log::Error("Found circular dev dependency: " + recipe.GetName() + " -> " + dependecyRecipe.GetName());
						throw std::runtime_error("BuildRecipeAndDependencies: Circular dev dependency.");
					}

					// Build all recursive dependencies
					// Note: Ignore all shared dependencies. They are not exposed past dev dependencies.
					auto ignoredBuildState = Build::BuildState();
					projectId = BuildRecipeAndDependencies(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments,
						true,
						activeParentSet,
						ignoredBuildState);
				}
			}

			// Build the root recipe
			projectId = BuildRecipe(
				projectId,
				workingDirectory,
				recipe,
				arguments,
				isSystemBuild,
				activeState,
				parentState);

			// Return the updated project id after building all dependencies
			return projectId;
		}

		/// <summary>
		/// The core build that will either invoke the recipe builder directly
		/// or compile it into an executable and invoke it.
		/// </summary>
		int BuildRecipe(
			int projectId,
			const Path& workingDirectory,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			Build::BuildState& activeState,
			Build::BuildState& parentState)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(projectId);
				Log::Diag("Running InProcess Build");

				auto findBuildState = _buildSet.find(recipe.GetName());
				if (findBuildState != _buildSet.end())
				{
					Log::Diag("Recipe already built: " + recipe.GetName());

					// Move the parent state from active into the parents active state :)
					parentState.CombineChildState(findBuildState->second);
				}
				else
				{
					// Run the required builds in process
					// This will break the circular requirments for the core build libraries
					RunInProcessBuild(
						projectId,
						workingDirectory,
						recipe,
						arguments,
						isSystemBuild,
						activeState);

					// Move the parent state from active into the parents active state :)
					parentState.CombineChildState(activeState);

					// Keep track of the packages we have already built
					// TODO: Verify unique names
					_buildSet.emplace(recipe.GetName(), std::move(activeState));

					// Move to the next build project id
					projectId++;
				}


				Log::SetActiveId(-1);
			}
			catch(...)
			{
				Log::SetActiveId(-1);
				throw;
			}

			return projectId;
		}

		void RunInProcessBuild(
			int projectId,
			const Path& packageRoot,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			Build::BuildState& state)
		{
			// Ensure the external build extension libraries outlive all usage in the build system
			auto activeExtensionLibraries = std::vector<System::Library>();

			{
				// Create a new build system for the requested build
				auto buildSystem = Build::BuildSystem();
				auto activeState = Build::PropertyBagWrapper(state.GetActiveState());

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

				// Set the input properties
				activeState.SetPropertyStringValue("PackageRoot", packageRoot.ToString());
				activeState.SetPropertyBooleanValue("ForceRebuild", arguments.ForceRebuild); // TOOD: Remove?
				activeState.SetPropertyStringValue("BuildFlavor", arguments.Flavor);
				activeState.SetPropertyStringValue("CompilerName", activeCompiler);
				activeState.SetPropertyStringList("PlatformLibraries", arguments.PlatformLibraries);
				activeState.SetPropertyStringList("PlatformIncludePaths", arguments.PlatformIncludePaths);
				activeState.SetPropertyStringList("PlatformLibraryPaths", arguments.PlatformLibraryPaths);
				activeState.SetPropertyStringList("PlatformPreprocessorDefinitions", arguments.PlatformPreprocessorDefinitions);

				// Run all build extensions
				// Note: Keep the extension libraries open while running the build system
				// to ensure their memory is kept alive

				// Run the RecipeBuild extension to inject core build tasks
				auto recipeBuildExtensionPath = Path("RecipeBuildExtension.dll");
				auto recipeBuildLibrary = RunBuildExtension(recipeBuildExtensionPath, buildSystem);
				activeExtensionLibraries.push_back(std::move(recipeBuildLibrary));

				// Run all dev dependency extensions
				if (recipe.HasDevDependencies())
				{
					for (auto dependecy : recipe.GetDevDependencies())
					{
						auto packagePath = RecipeExtensions::GetPackageReferencePath(packageRoot, dependecy);
						auto libraryPath = RecipeExtensions::GetRecipeOutputPath(
							packagePath,
							RecipeExtensions::GetBinaryDirectory(_systemCompiler, arguments.Flavor),
							std::string(".dll"));
						
						auto library = RunBuildExtension(libraryPath, buildSystem);
						activeExtensionLibraries.push_back(std::move(library));
					}
				}

				// Run the build
				buildSystem.Execute(state);

				// Execute the build nodes
				auto runner = Build::BuildRunner(packageRoot);
				runner.Execute(state.GetBuildNodes(), arguments.ForceRebuild);
			}
		}

		Path GetPackageReferencePath(const Path& workingDirectory, const PackageReference& reference) const
		{
			// If the path is relative then combine with the working directory
			auto packagePath = reference.GetPath();
			if (!packagePath.HasRoot())
			{
				packagePath = workingDirectory + packagePath;
			}

			return packagePath;
		}

		System::Library RunBuildExtension(
			Path& libraryPath,
			Build::IBuildSystem& buildSystem)
		{
			try
			{
				Log::Diag("Running Build Extension: " + libraryPath.ToString());
				auto library = System::DynamicLibraryManager::LoadDynamicLibrary(
					libraryPath.ToString().c_str());
				auto function = (int(*)(Build::IBuildSystem&))library.GetFunction(
					"RegisterBuildExtension");
				auto result = function(buildSystem);
				if (result != 0)
				{
					Log::Error("Build Extension Failed: " + std::to_string(result));
				}
				else
				{
					Log::Info("Build Extension Done: " + std::to_string(result));
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

	private:
		std::string _systemCompiler;
		std::string _runtimeCompiler;
		std::map<std::string, Build::BuildState> _buildSet;
	};
}
