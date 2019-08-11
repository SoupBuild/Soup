// <copyright file="RecipeBuildManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "RecipeBuilder.h"
#include "RecipeBuildGenerator.h"

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
            _builder(std::move(compiler))
        {
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        void Execute(const Path& workingDirectory, const Recipe& recipe, bool forceBuild)
        {
            // Clear the build set so we check all dependencies
            _buildSet.clear();

            // Enable log event ids to track individual builds
            int projectId = 1;
            Log::EnsureListener().SetShowEventId(true);

            // TODO: A scoped listener cleanup would be nice
            try
            {
                projectId = BuildAllDependenciesRecursively(projectId, workingDirectory, recipe, forceBuild);
                BuildRecipe(projectId, workingDirectory, recipe, forceBuild);

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
            bool forceBuild)
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
                projectId = BuildAllDependenciesRecursively(projectId, packagePath, dependecyRecipe, forceBuild);

                // Build this dependecy
                projectId = BuildRecipe(projectId, packagePath, dependecyRecipe, forceBuild);
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
            bool forceBuild)
        {
            if (_buildSet.contains(recipe.GetName()))
            {
                Log::Verbose("Recipe already built: " + recipe.GetName());
            }
            else
            {
                // Gen the build
                auto generator = RecipeBuildGenerator();
                generator.Execute(workingDirectory, recipe);

                // Run the build in process
                _builder.Execute(projectId, workingDirectory, recipe, forceBuild);

                // Keep track of the packages we have already built
                // TODO: Verify unique names
                _buildSet.insert(recipe.GetName());

                // Move to the next build project id
                projectId++;
            }

            return projectId;
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
        std::set<std::string> _buildSet;
    };
}
