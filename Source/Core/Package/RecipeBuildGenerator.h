// <copyright file="RecipeBuildGenerator.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "RecipeBuild.h"

namespace Soup
{
    /// <summary>
    /// The recipe build generator that knows how to build a recipe 
    /// and all of its dependencies
    /// </summary>
    export class RecipeBuildGenerator
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="RecipeBuildGenerator"/> class.
        /// </summary>
        RecipeBuildGenerator(std::shared_ptr<ICompiler> compiler) :
            _build(std::move(compiler))
        {
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        void Execute(const Path& workingDirectory, const Recipe& recipe, bool forceBuild)
        {
            // Enable log event ids to track individual builds
            int projectId = 1;
            Log::EnsureListener().SetShowEventId(true);

            // TODO: A scoped listener cleanup would be nice
            try
            {
                projectId = BuildAllDependenciesRecursively(projectId, workingDirectory, recipe, forceBuild);
                _build.Execute(projectId, workingDirectory, recipe, forceBuild);

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
                _build.Execute(projectId, packagePath, dependecyRecipe, forceBuild);

                // Move to the next build project id
                projectId++;
            }

            // Return the updated project id after building all dependencies
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
        RecipeBuild _build;
    };
}
