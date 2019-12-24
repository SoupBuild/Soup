// <copyright file="RecipeBuildManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "Build/System/BuildSystem.h"
#include "Build/Tasks/RecipeBuildTask.h"

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
			std::shared_ptr<ICompiler> systemCompiler,
			std::shared_ptr<ICompiler> runtimeCompiler) :
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
				projectId = BuildAllDependenciesRecursively(
					projectId,
					workingDirectory,
					recipe,
					arguments,
					isSystemBuild,
					rootParentSet);
				BuildRecipe(
					projectId,
					workingDirectory,
					recipe,
					arguments,
					isSystemBuild);

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
		int BuildAllDependenciesRecursively(
			int projectId,
			const Path& workingDirectory,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild,
			const std::set<std::string>& parentSet)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(recipe.GetName());

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
						throw std::runtime_error("BuildAllDependenciesRecursively: Failed to load dependency.");
					}

					// Ensure we do not have any circular dependencies
					if (activeParentSet.contains(dependecyRecipe.GetName()))
					{
						Log::Error("Found circular dependency: " + recipe.GetName() + " -> " + dependecyRecipe.GetName());
						throw std::runtime_error("BuildAllDependenciesRecursively: Circular dependency.");
					}

					// Build all recursive dependencies
					projectId = BuildAllDependenciesRecursively(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments,
						isSystemBuild,
						activeParentSet);

					// Build this dependecy
					projectId = BuildRecipe(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments,
						isSystemBuild);
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
						throw std::runtime_error("BuildAllDependenciesRecursively: Failed to load dependency.");
					}

					// Ensure we do not have any circular dependencies
					if (activeParentSet.contains(dependecyRecipe.GetName()))
					{
						Log::Error("Found circular dev dependency: " + recipe.GetName() + " -> " + dependecyRecipe.GetName());
						throw std::runtime_error("BuildAllDependenciesRecursively: Circular dev dependency.");
					}

					// Build all recursive dependencies
					projectId = BuildAllDependenciesRecursively(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments,
						true,
						activeParentSet);

					// Build this dependecy
					projectId = BuildRecipe(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments,
						true);
				}
			}

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
			bool isSystemBuild)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(projectId);
				Log::Diag("Running InProcess Build");

				if (_buildSet.contains(recipe.GetName()))
				{
					Log::Diag("Recipe already built: " + recipe.GetName());
				}
				else
				{
					// Run the required builds in process
					// This will break the circular requirments for the core build libraries
					RunInProcessBuild(projectId, workingDirectory, recipe, arguments, isSystemBuild);

					// Keep track of the packages we have already built
					// TODO: Verify unique names
					_buildSet.insert(recipe.GetName());

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
			bool isSystemBuild)
		{
			// Create a new build system for the requested build
			auto buildSystem = BuildSystem();

			// Select the correct compiler to use
			std::shared_ptr<ICompiler> activeCompiler = nullptr;
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

			// Register the recipe build task
			auto recipeBuildTask = std::make_shared<RecipeBuildTask>(
				_systemCompiler,
				activeCompiler,
				packageRoot,
				recipe,
				arguments);
			buildSystem.RegisterTask(recipeBuildTask);

			// Register the compile task
			auto buildTask = std::make_shared<BuildTask>(activeCompiler);
			buildSystem.RegisterTask(buildTask);

			// Run the build
			buildSystem.Execute();
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

	private:
		std::shared_ptr<ICompiler> _systemCompiler;
		std::shared_ptr<ICompiler> _runtimeCompiler;
		std::set<std::string> _buildSet;
	};
}
