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

            // Setup the output directories
            auto outputDirectory = Path("out");
            _objectDirectory = outputDirectory + Path("obj");
            _binaryDirectory = outputDirectory + Path("bin");
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
        void CoreBuild(
            int projectId,
            const Path& workingDirectory,
            const Recipe& recipe,
            bool force)
        {
            Log::SetActiveId(projectId);
            Log::Info("Building '" + recipe.GetName() + "'");

            try
            {
                // Determine the include paths
                std::unordered_set<std::string> includePaths;
                for (auto& entry : std::filesystem::recursive_directory_iterator(workingDirectory.ToString()))
                {
                    if (entry.path().extension() == ".h")
                    {
                        includePaths.insert(
                            entry.path().parent_path().string());
                    }
                }

                // var folderWithHeadersSet = Directory.EnumerateFiles(path, "*.h", SearchOption.AllDirectories).Select(file => Path.GetDirectoryName(file)).ToHashSet();
                // var uniqueFolders = folderWithHeadersSet.ToList();

                // Add all dependency packages modules references
                auto includeModules = std::vector<Path>();
                for (auto dependecy : recipe.GetDependencies())
                {
                    auto modulePath = GetRecipeModulePath(dependecy.GetPath());
                    includeModules.push_back(std::move(modulePath));
                }

                // Add the dependency static library closure to link if targeting an executable
                std::vector<Path> linkLibraries;
                if (recipe.GetType() == RecipeType::Executable)
                {
                    GenerateDependecyStaticLibraryClosure(
                        workingDirectory,
                        recipe,
                        linkLibraries);
                }

                // Build up arguments to build this individual recipe
                auto arguments = BuildArguments();
                arguments.TargetName = recipe.GetName();
                arguments.WorkingDirectory = workingDirectory;
                arguments.ObjectDirectory = GetObjectDirectory();
                arguments.BinaryDirectory = GetBinaryDirectory();
                arguments.ModuleSourceFile = 
                    recipe.HasPublic() ? recipe.GetPublicAsPath() : Path();
                arguments.SourceFiles = recipe.GetSourceAsPath();
                arguments.IncludeDirectories = std::vector<Path>(includePaths.begin(), includePaths.end());
                arguments.IncludeModules = std::move(includeModules);
                arguments.LinkLibraries = std::move(linkLibraries);
                arguments.IsIncremental = true;

                // Convert the recipe type to the required build type
                switch (recipe.GetType())
                {
                    case RecipeType::Library:
                        arguments.TargetType = BuildTargetType::Library;
                        break;
                    case RecipeType::Executable:
                        arguments.TargetType = BuildTargetType::Executable;
                        break;
                    default:
                        throw std::runtime_error("Unknown build target type.");
                }

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

        void GenerateDependecyStaticLibraryClosure(
            const Path& workingDirectory,
            const Recipe& recipe,
            std::vector<Path>& closure) const
        {
            for (auto dependecy : recipe.GetDependencies())
            {
                // Load this package recipe
                auto dependencyPackagePath = dependecy.GetPath();
                auto packageRecipePath = dependencyPackagePath + Path(Constants::RecipeFileName);
                Recipe dependecyRecipe = {};
                if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependecyRecipe))
                {
                    Log::Error("Failed to load the dependency package: {packagePath}");
                    throw std::runtime_error("Failed to load dependency.");
                }

                // Add this dependency
                auto dependencyStaticLibrary = 
                    dependencyPackagePath +
                    GetBinaryDirectory() +
                    Path(dependecyRecipe.GetName() + "." + std::string(_compiler->GetStaticLibraryFileExtension()));
                closure.push_back(std::move(dependencyStaticLibrary));

                // Add all recursive dependencies
                GenerateDependecyStaticLibraryClosure(dependencyPackagePath, dependecyRecipe, closure);
            }
        }

        Path GetObjectDirectory() const
        {
            return _objectDirectory + Path(_compiler->GetName());
        }

        Path GetBinaryDirectory() const
        {
            return _binaryDirectory + Path(_compiler->GetName());
        }

        Path GetRecipeModulePath(const Path& packagePath) const
        {
            auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
            Recipe dependecyRecipe = {};
            if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependecyRecipe))
            {
                Log::Error("Failed to load the dependency package: {packagePath}");
                throw std::runtime_error("Failed to load dependency.");
            }

            auto packageBinaryPath = packagePath + GetBinaryDirectory();
            auto moduleFilename = Path(dependecyRecipe.GetName() + "." + std::string(_compiler->GetModuleFileExtension()));
            auto modulePath = packageBinaryPath + moduleFilename;

            return modulePath;
        }

    private:
        std::shared_ptr<ICompiler> _compiler;
        Path _objectDirectory;
        Path _binaryDirectory;
    };
}
