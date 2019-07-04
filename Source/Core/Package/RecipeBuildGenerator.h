// <copyright file="RecipeBuildGenerator.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"

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
            _compiler(std::move(compiler))
        {
            if (_compiler == nullptr)
                throw std::runtime_error("Argument null: compiler");
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        void Execute(const Path& workingDirectory, const Recipe& recipe, bool force)
        {
            // Enable log event ids to track individual builds
            int projectId = 1;
            Log::EnsureListener().SetShowEventId(true);

            // TODO: A scoped listener cleanup would be nice
            try
            {
                projectId = BuildAllDependenciesRecursively(projectId, workingDirectory, recipe, force);
                CoreBuild(projectId, workingDirectory, recipe, force);

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
            bool force)
        {
            for (auto dependecy : recipe.GetDependencies())
            {
                // Load this package recipe
                auto packagePath = dependecy.GetPath();
                auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
                Recipe dependecyRecipe = {};
                if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependecyRecipe))
                {
                    Log::Error("Failed to load the dependency package: {packagePath}");
                    throw std::runtime_error("Failed to load dependency.");
                }

                // Build all recursive dependencies
                projectId = BuildAllDependenciesRecursively(projectId, packagePath, dependecyRecipe, force);

                // Build this dependecy
                CoreBuild(projectId, packagePath, dependecyRecipe, force);

                // Move to the next build project id
                projectId++;
            }

            // Return the updated project id after building all dependencies
            return projectId;
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        void CoreBuild(int projectId, const Path& workingDirectory, const Recipe& recipe, bool force)
        {
            Log::SetActiveId(projectId);
            Log::Info("Building '" + recipe.GetName() + "'");

            try
            {
                // Initialize the required target paths
                auto compilerFolder = Path(_compiler->GetName());
                auto buildPath =
                    Path(Constants::ProjectGenerateFolderName) +
                    Path(Constants::StoreBuildFolderName);
                auto outputDirectory = Path("out");
                auto objectDirectory = outputDirectory + Path("obj") + compilerFolder;
                auto binaryDirectory = outputDirectory + Path("bin") + compilerFolder;

                // Determine the include paths
                // var folderWithHeadersSet = Directory.EnumerateFiles(path, "*.h", SearchOption.AllDirectories).Select(file => Path.GetDirectoryName(file)).ToHashSet();
                // var uniqueFolders = folderWithHeadersSet.ToList();

                // Build up arguments to build this individual recipe
                auto arguments = BuildArguments();
                arguments.Target = BuildTargetType::Executable;
                arguments.WorkingDirectory = workingDirectory;
                arguments.ObjectDirectory = objectDirectory;
                arguments.BinaryDirectory = binaryDirectory;
                arguments.ModuleSourceFile = 
                    recipe.HasPublic() ? recipe.GetPublicAsPath() : Path();
                arguments.SourceFiles = recipe.GetSourceAsPath();
                arguments.IncludeDirectories = std::vector<Path>({});
                arguments.IncludeModules = std::vector<Path>({});
                arguments.IsIncremental = true;

                // Perform the build
                auto buildEngine = BuildEngine(_compiler);
                buildEngine.Execute(arguments);
            }
            catch (std::exception& ex)
            {
                // Log the exception and convert to handled
                Log::Error(std::string("Build Failed: ") + ex.what());
                throw HandledException();
            }
        }

    private:
        std::shared_ptr<ICompiler> _compiler;
    };
}
