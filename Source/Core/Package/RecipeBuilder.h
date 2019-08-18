// <copyright file="RecipeBuild.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"

namespace Soup
{
    /// <summary>
    /// The recipe builder that knows how to build a single recipe
    /// Used either directly inside Soup builds or in the generated build executable
    /// </summary>
    export class RecipeBuilder
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="RecipeBuilder"/> class.
        /// </summary>
        RecipeBuilder(std::shared_ptr<ICompiler> compiler) :
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
        void Execute(
            int projectId,
            const Path& workingDirectory,
            const Recipe& recipe,
            bool forceBuild)
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

                // Add all dependency packages modules references
                auto includeModules = std::vector<Path>();
                for (auto dependecy : recipe.GetDependencies())
                {
                    auto packagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
                    auto modulePath = RecipeExtensions::GetRecipeModulePath(packagePath, GetBinaryDirectory(), std::string(_compiler->GetModuleFileExtension()));
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
                arguments.ModuleInterfaceSourceFile = 
                    recipe.HasPublic() ? recipe.GetPublicAsPath() : Path();
                arguments.SourceFiles = recipe.GetSourceAsPath();
                arguments.IncludeModules = std::move(includeModules);
                arguments.LinkLibraries = std::move(linkLibraries);
                arguments.IsIncremental = !forceBuild;
                arguments.PreprocessorDefinitions = std::vector<std::string>({
                    "SOUP_BUILD",
                });

                // Strip out the working directory from the include paths
                for (auto& entry : includePaths)
                {
                    auto entryPath = Path(entry);
                    auto directory = entryPath.GetRelativeTo(workingDirectory);
                    arguments.IncludeDirectories.push_back(directory);
                }

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
                auto wasBuilt = buildEngine.Execute(arguments);

                if (wasBuilt)
                    Log::Info("Done");
                else
                    Log::Info("Up to date");
            }
            catch (std::exception& ex)
            {
                // Log the exception and convert to handled
                Log::Error(std::string("Build Failed: ") + ex.what());
                throw HandledException();
            }
        }

    private:
        void GenerateDependecyStaticLibraryClosure(
            const Path& workingDirectory,
            const Recipe& recipe,
            std::vector<Path>& closure) const
        {
            for (auto& dependecy : recipe.GetDependencies())
            {
                // Load this package recipe
                auto dependencyPackagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
                auto packageRecipePath = dependencyPackagePath + Path(Constants::RecipeFileName);
                Recipe dependecyRecipe = {};
                if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, dependecyRecipe))
                {
                    Log::Error("Failed to load the dependency package: " + packageRecipePath.ToString());
                    throw std::runtime_error("GenerateDependecyStaticLibraryClosure: Failed to load dependency.");
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

    private:
        std::shared_ptr<ICompiler> _compiler;
        Path _objectDirectory;
        Path _binaryDirectory;
    };
}
