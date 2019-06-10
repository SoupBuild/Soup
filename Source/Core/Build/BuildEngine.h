// <copyright file="BuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IBuildEngine.h"
#include "ICompiler.h"
#include "BuildStateManager.h"

namespace Soup
{
    /// <summary>
    /// The build engine
    /// </summary>
    export class BuildEngine : public IBuildEngine
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="BuildEngine"/> class.
        /// </summary>
        BuildEngine(std::shared_ptr<ICompiler> compiler) :
            _compiler(std::move(compiler))
        {
            if (_compiler == nullptr)
                throw std::runtime_error("Argument Null: compiler");
        }

        /// <summary>
        /// The Core build task
        /// </summary>
        virtual void Execute(const BuildArguments& arguments) override final
        {
            // Load the previous build state if performing an incremental build
            BuildState buildState = {};
            if (arguments.IsIncremental)
            {
                Log::Verbose("Loading previous build state.");
                if (!BuildStateManager::TryLoadState(arguments.WorkingDirectory, buildState))
                {
                    Log::Verbose("No previous state found.");
                    buildState = BuildState();
                }
            }

            bool sourceBuilt = CompileSource(arguments);

            // switch (recipe.Type)
            // {
            //     case RecipeType.Library:
            //         await CheckLinkLibraryAsync(path, recipe, objectDirectory, binaryDirectory, force);
            //         break;
            //     case RecipeType.Executable:
            //         await LinkExecutableAsync(path, recipe, objectDirectory, binaryDirectory);
            //         break;
            //     default:
            //         throw new NotSupportedException("Unknown recipe type.");
            // }

            // Save the build state
            BuildStateManager::SaveState(arguments.WorkingDirectory, buildState);
        }

    private:
        /// <summary>
        /// Compile the module file
        /// </summary>
        // bool CheckCompileModuleAsync(
        //     string path,
        //     Recipe recipe,
        //     BuildState buildState,
        //     IList<string> uniqueFolders,
        //     string objectDirectory,
        //     string binaryDirectory,
        //     bool force)
        // {
        //     var outputFilename = $"{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ModuleFileExtension}";
        //     var outputFile = Path.Combine(objectDirectory, outputFilename);
        //     bool requiresBuild = true;
        //     if (!force)
        //     {
        //         // Add all of the direct dependencies as module references
        //         var modules = new List<string>();
        //         var defines = new List<string>();
        //         await BuildDependencyModuleReferences(path, binaryDirectory, recipe, modules, defines);

        //         // The dependencies for this file are all of the direct module references
        //         var dependencies = new List<string>();
        //         dependencies.AddRange(modules);

        //         if (!BuildRequiredChecker.IsSourceFileOutdated(path, buildState, outputFile, recipe.Public, dependencies))
        //         {
        //             // TODO : This is a hack. We need to actually look through all of the imports for the module file
        //             Log.Info("Module file is up to date.");
        //             requiresBuild = false;
        //         }
        //     }

        //     if (requiresBuild)
        //     {
        //         await CompileModuleAsync(
        //             path,
        //             recipe,
        //             buildState,
        //             uniqueFolders,
        //             objectDirectory,
        //             binaryDirectory);
        //     }

        //     return requiresBuild;
        // }

        // /// <summary>
        // /// Compile the module file
        // /// </summary>
        // void CompileModule(
        //     string path,
        //     Recipe recipe,
        //     BuildState buildState,
        //     IList<string> uniqueFolders,
        //     string objectDirectory,
        //     string binaryDirectory)
        // {
        //     Log.Info("Compile Module");

        //     if (string.IsNullOrEmpty(recipe.Public))
        //     {
        //         throw new InvalidOperationException("The public file was not set.");
        //     }

        //     var modules = new List<string>();
        //     var defines = new List<string>();

        //     defines.Add("SOUP_BUILD");

        //     // Set the active version namespace
        //     defines.Add(BuildRecipeNamespaceDefine(recipe));

        //     // Add all of the direct dependencies as module references
        //     // and set their version defintions
        //     await BuildDependencyModuleReferences(path, binaryDirectory, recipe, modules, defines);

        //     var args = new CompileArguments()
        //     {
        //         Standard = Compiler.LanguageStandard.Latest,
        //         RootDirectory = path,
        //         OutputDirectory = objectDirectory,
        //         PreprocessorDefinitions = defines,
        //         SourceFiles = new List<string>() { recipe.Public },
        //         IncludeDirectories = uniqueFolders,
        //         Modules = modules,
        //         ExportModule = true,
        //         GenerateIncludeTree = true,
        //     };

        //     // Ensure the object directory exists
        //     var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
        //     if (!Directory.Exists(objectDirectry))
        //     {
        //         Directory.CreateDirectory(objectDirectry);
        //     }

        //     // Compile each file
        //     var result = await _compiler.CompileAsync(args);

        //     // Save the build state
        //     if (result.HeaderIncludeFiles != null)
        //         buildState.UpdateIncludeTree(result.HeaderIncludeFiles);
        // }

        /// <summary>
        /// Compile the supporting source files
        /// </summary>
        bool CompileSource(const BuildArguments& arguments)
        {
            // // Check if each source file is out of date and requires a rebuild
            // for (var sourceFile in recipe.Source)
            // {
            //     var outputFile = Path.Combine(objectDirectory, $"{Path.GetFileNameWithoutExtension(sourceFile)}.{_compiler.ObjectFileExtension}");
            //     if (force || BuildRequiredChecker.IsSourceFileOutdated(path, buildState, outputFile, sourceFile, sharedDependecies))
            //     {
            //         source.Add(sourceFile);
            //     }
            // }

            // if (source.Count == 0)
            // {
            //     Log.Info("All source is up to date.");
            //     return false;
            // }
            // else
            // {
            //     Log.Info("Compile Source");
            //     var args = new CompileArguments()
            //     {
            //         Standard = Compiler.LanguageStandard.Latest,
            //         RootDirectory = path,
            //         OutputDirectory = objectDirectory,
            //         PreprocessorDefinitions = defines,
            //         SourceFiles = source,
            //         IncludeDirectories = uniqueFolders,
            //         Modules = modules,
            //         GenerateIncludeTree = true,
            //     };

            //     // Ensure the object directory exists
            //     var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
            //     if (!Directory.Exists(objectDirectry))
            //     {
            //         Directory.CreateDirectory(objectDirectry);
            //     }


            //     // Compile each file
            //     var result = await _compiler.CompileAsync(args);

            //     // Save the build state
            //     if (result.HeaderIncludeFiles != null)
            //         buildState.UpdateIncludeTree(result.HeaderIncludeFiles);

                 return true;
            // }
        }

        // private async Task BuildDependencyModuleReferences(
        //     string path,
        //     string binaryDirectory,
        //     Recipe recipe,
        //     IList<string> modules,
        //     IList<string> defines)
        // {
        //     foreach (var dependecy in recipe.Dependencies)
        //     {
        //         // Load this package recipe
        //         var packagePath = VerifyDependencyPath(path, dependecy);
        //         var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

        //         modules.Add(Path.Combine(packagePath, binaryDirectory, BuildRecipeModuleFilename(dependecyRecipe)));
        //         defines.Add(BuildRecipeNamespaceDefine(dependecyRecipe));
        //     }
        // }

        // /// <summary>
        // /// Link the resulting object files
        // /// </summary>
        // private async Task CheckLinkLibraryAsync(
        //     string path,
        //     Recipe recipe,
        //     string objectDirectory,
        //     string binaryDirectory,
        //     bool force)
        // {
        //     var allFiles = new List<string>(recipe.Source);
        //     if (recipe.Type == RecipeType.Library)
        //     {
        //         allFiles.Add(recipe.Public);
        //     }

        //     // Convert all source files into objects
        //     var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.{_compiler.ObjectFileExtension}").ToList();

        //     // Add the modules object too
        //     objectFiles.Add($"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ObjectFileExtension}");

        //     var targetLibraryFile = Path.Combine(binaryDirectory, $"{recipe.Name}.{_compiler.StaticLibraryFileExtension}");
        //     if (force || BuildRequiredChecker.IsOutdated(path, targetLibraryFile, objectFiles))
        //     {
        //         Log.Info("Link library");
        //         var args = new LinkerArguments()
        //         {
        //             Name = recipe.Name,
        //             RootDirectory = path,
        //             OutputDirectory = binaryDirectory,
        //             SourceFiles = objectFiles,
        //         };

        //         // Ensure the object directory exists
        //         var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
        //         if (!Directory.Exists(objectDirectry))
        //         {
        //             Directory.CreateDirectory(objectDirectry);
        //         }

        //         // Link
        //         await _compiler.LinkLibraryAsync(args);
        //     }
        //     else
        //     {
        //         Log.Info("Static library up to date.");
        //     }
        // }

        // /// <summary>
        // /// Link the executable
        // /// </summary>
        // private async Task LinkExecutableAsync(
        //     string path,
        //     Recipe recipe,
        //     string objectDirectory,
        //     string binaryDirectory)
        // {
        //     Log.Info("Link Executable");
        //     var allFiles = new List<string>(recipe.Source);
        //     if (recipe.Type == RecipeType.Library)
        //     {
        //         allFiles.Add(recipe.Public);
        //     }

        //     // Add all of the dependencies as module references
        //     var librarySet = new HashSet<string>();
        //     await GenerateDependencyLibrarySetAsync(path, binaryDirectory, recipe, librarySet);

        //     var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.{_compiler.ObjectFileExtension}").ToList();
        //     var libraryFiles = librarySet.ToList();
        //     var args = new LinkerArguments()
        //     {
        //         Name = recipe.Name,
        //         RootDirectory = path,
        //         OutputDirectory = binaryDirectory,
        //         SourceFiles = objectFiles,
        //         LibraryFiles = libraryFiles,
        //     };

        //     // Ensure the object directory exists
        //     var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
        //     if (!Directory.Exists(objectDirectry))
        //     {
        //         Directory.CreateDirectory(objectDirectry);
        //     }

        //     // Link
        //     await _compiler.LinkExecutableAsync(args);
        // }

        // /// <summary>
        // /// Clone the module interface definition from the object directoy to the binary directory
        // /// </summary>
        // private void CloneModuleInterface(string path, Recipe recipe, string objectDirectory, string binaryDirectory)
        // {
        //     Log.Verbose("Clone Module Interface");
        //     var sourceModuleFile = Path.Combine(path, objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ModuleFileExtension}");
        //     var targetModuleFile = Path.Combine(path, binaryDirectory, BuildRecipeModuleFilename(recipe));

        //     // Ensure the object directory exists
        //     if (!File.Exists(sourceModuleFile))
        //     {
        //         throw new InvalidOperationException("The resulting module interface definition was missing.");
        //     }

        //     Log.Verbose($"Clone Module: {sourceModuleFile} -> {targetModuleFile}");
        //     File.Copy(sourceModuleFile, targetModuleFile, true);
        // }

        // private async Task GenerateDependencyLibrarySetAsync(
        //     string path,
        //     string binaryDirectory,
        //     Recipe recipe,
        //     HashSet<string> set)
        // {
        //     foreach (var dependecy in recipe.Dependencies)
        //     {
        //         // Load this package recipe
        //         var packagePath = VerifyDependencyPath(path, dependecy);
        //         var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

        //         // Get recursive dependencies
        //         await GenerateDependencyLibrarySetAsync(path, binaryDirectory, dependecyRecipe, set);

        //         set.Add(Path.Combine(packagePath, binaryDirectory, $"{dependecyRecipe.Name}.a").ToLower());
        //     }
        // }

        // private string VerifyDependencyPath(string path, PackageReference dependecy)
        // {
        //     string packagePath;
        //     if (dependecy.Path != null)
        //     {
        //         // Build the relative path
        //         Log.Verbose($"Local Dependecy: {dependecy.Path}");
        //         if (!Path.IsPathRooted(dependecy.Path))
        //         {
        //             packagePath = Path.GetFullPath(Path.Combine(path, dependecy.Path));
        //         }
        //         else
        //         {
        //             packagePath = dependecy.Path;
        //         }

        //         Log.Verbose($"Path: {packagePath}");

        //         // Verify the package exists
        //         if (!Directory.Exists(packagePath))
        //         {
        //             Log.Error($"The local package reference folder does not exist: {packagePath}");
        //             throw new InvalidOperationException();
        //         }
        //     }
        //     else
        //     {
        //         packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
        //     }

        //     return packagePath;
        // }

        // private string BuildRecipeNamespaceDefine(Recipe recipe)
        // {
        //     return $"{recipe.Name.Replace(".", "_")}_VersionNamespace={recipe.Name}::{GetNamespace(recipe.Version)}";
        // }

        // private string BuildRecipeModuleFilename(Recipe recipe)
        // {
        //     return $"{recipe.Name.Replace(".", "_")}.{_compiler.ModuleFileExtension}";
        // }

        // private string GetNamespace(SemanticVersion version)
        // {
        //     return $"v{version.Major}_{version.Minor}_{version.Patch}";
        // }

    private:
        std::shared_ptr<ICompiler> _compiler;
    };
}
