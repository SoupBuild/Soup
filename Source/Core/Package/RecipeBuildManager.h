// <copyright file="RecipeBuildManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "RecipeBuilder.h"
#include "RecipeBuildGenerator.h"
#include "IProcessManager.h"

namespace Soup
{
	/// <summary>
	/// The recipe build manager that knows how to perform the correct build for a recipe 
	/// and all of its developer and runtime dependencies
	/// </summary>
	export class RecipeBuildManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildManager"/> class.
		/// </summary>
		RecipeBuildManager(std::shared_ptr<ICompiler> compiler) :
			_builder(compiler),
			_generator(compiler)
		{
			// Setup the core set of recipes that are required to break
			// the circular build dependency from within the core command line executable
			// TODO: Normalize uppercase?
			_knownInProcessRecipes = std::set<std::string>({
				"std.core",
				"Soup.Core",
			});
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
			Log::EnsureListener().SetShowEventId(true);

			// TODO: A scoped listener cleanup would be nice
			try
			{
				projectId = BuildAllDependenciesRecursively(
					projectId,
					workingDirectory,
					recipe,
					arguments);
				BuildRecipe(
					projectId,
					workingDirectory,
					recipe,
					arguments);

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
			const RecipeBuildArguments& arguments)
		{
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

					// Build all recursive dependencies
					projectId = BuildAllDependenciesRecursively(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments);

					// Build this dependecy
					projectId = BuildRecipe(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments);
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

					// Build all recursive dependencies
					projectId = BuildAllDependenciesRecursively(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments);

					// Build this dependecy
					projectId = BuildRecipe(
						projectId,
						packagePath,
						dependecyRecipe,
						arguments);
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
			const RecipeBuildArguments& arguments)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(projectId);
				Log::Verbose("Running InProcess Build");

				if (_buildSet.contains(recipe.GetName()))
				{
					Log::Verbose("Recipe already built: " + recipe.GetName());
				}
				else
				{
					// if (_knownInProcessRecipes.contains(recipe.GetName()))
					// {
						// Run the required builds in process
						// This will break the circular requirments for the core build libraries
						RunInProcessBuild(projectId, workingDirectory, recipe, arguments);
					// }
					// else
					// {
					// 	// Default to using a generated build executable
					// 	RunGenerateBuild(projectId, workingDirectory, recipe);
					// }

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
			const RecipeBuildArguments& arguments)
		{
			_builder.Execute(packageRoot, recipe, arguments);
		}

		void RunGenerateBuild(
			int projectId,
			const Path& packageRoot,
			const Recipe& recipe)
		{
			Log::Verbose("Running Generate Build");

			// Gen the build
			auto executablePath = _generator.EnsureExecutableBuilt(packageRoot, recipe);

			// Invoke the build
			Log::Verbose("Invoke Compiler: " + executablePath.ToString());
			auto arguments = std::vector<std::string>();
			auto result = IProcessManager::Current().Execute(
				executablePath,
				arguments,
				packageRoot);

			// TODO: Directly pipe to output and make sure there is no extra newline
			if (!result.StdOut.empty())
			{
				Log::Info(result.StdOut);
			}

			if (!result.StdErr.empty())
			{
				Log::Error(result.StdErr);
			}

			if (result.ExitCode != 0)
			{
				// TODO: Return error code
				Log::Verbose("Invoke Build Failed: " + std::to_string(result.ExitCode));
				throw std::runtime_error("Invoke Build Failed!");
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

	private:
		RecipeBuilder _builder;
		RecipeBuildGenerator _generator;
		std::set<std::string> _buildSet;
		std::set<std::string> _knownInProcessRecipes;
	};
}
