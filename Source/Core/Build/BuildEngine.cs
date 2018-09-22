// <copyright file="BuildEngine.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Threading.Tasks;
    using Soup.Compiler;

    /// <summary>
    /// The build engine
    /// </summary>
    public class BuildEngine
    {
        private LocalUserConfig _config;
        private ICompiler _compiler;

        /// <summary>
        /// Initializes a new instance of the <see cref="BuildEngine"/> class.
        /// </summary>
        public BuildEngine(LocalUserConfig config, ICompiler compiler)
        {
            _config = config;
            _compiler = compiler;
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        public async Task ExecuteAsync(string path, Recipe recipe)
        {
            try
            {
                Log.Info("Build Recursive Dependencies.");
                await BuildAllDependenciesRecursivelyAsync(path, recipe);

                Log.Info("Build Toplevel Recipe.");
                await CoreBuildAsync(path, recipe);
            }
            catch (InvalidOperationException)
            {
                Log.Error("BUILD FAILED!");
            }
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        private async Task CoreBuildAsync(string path, Recipe recipe)
        {
            var objectDirectory = "obj";
            var binaryDirectory = "bin";

            // Determine the include paths
            var folderSet = new HashSet<string>();
            foreach (var file in recipe.Source)
            {
                var fileFolder = Path.GetDirectoryName(file);
                if (!string.IsNullOrWhiteSpace(fileFolder))
                {
                    folderSet.Add(fileFolder);
                }
            }

            var uniqueFolders = folderSet.ToList();

            Log.Info($"Building {recipe.Name}.");
            if (recipe.Type == RecipeType.Library)
            {
                await CompileModuleAsync(path, recipe, uniqueFolders, objectDirectory);
            }

            await CompileSourceAsync(path, recipe, uniqueFolders, objectDirectory);
            switch (recipe.Type)
            {
                case RecipeType.Library:
                    await LinkLibraryAsync(path, recipe, objectDirectory, binaryDirectory);
                    break;
                case RecipeType.Executable:
                    await LinkExecutableAsync(path, recipe, objectDirectory, binaryDirectory);
                    break;
                default:
                    throw new NotSupportedException("Unknown recipe type.");
            }

            if (recipe.Type == RecipeType.Library)
            {
                CloneModuleInterface(path, recipe, objectDirectory, binaryDirectory);
            }
        }

        /// <summary>
        /// Build the dependecies for the provided recipe recursively
        /// </summary>
        private async Task BuildAllDependenciesRecursivelyAsync(
            string path,
            Recipe recipe)
        {
            Log.Info($"Seaching Dependencies: {recipe.Name}.");
            foreach (var dependecy in recipe.Dependencies)
            {
                // Load this package recipe
                string packagePath;
                if (dependecy.Path != null)
                {
                    packagePath = dependecy.Path;
                }
                else
                {
                    packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
                }

                var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

                // Build all recursive dependencies
                await BuildAllDependenciesRecursivelyAsync(packagePath, dependecyRecipe);

                // Build this dependecy
                await CoreBuildAsync(packagePath, dependecyRecipe);
            }
        }

        /// <summary>
        /// Compile the module file
        /// </summary>
        private async Task CompileModuleAsync(string path, Recipe recipe, IList<string> uniqueFolders, string objectDirectory)
        {
            Log.Info("Compile Module");

            var defines = new List<string>();
            defines.Add("SOUP_BUILD");

            // Set the active version namespace
            defines.Add($"{recipe.Name}_VersionNamespace={recipe.Name}::{GetNamespace(recipe.Version)}");

            // Add all of the direct dependencies version defintions
            foreach (var dependecy in recipe.Dependencies)
            {
                // Load this package recipe
                string packagePath;
                if (dependecy.Path != null)
                {
                    packagePath = dependecy.Path;
                }
                else
                {
                    packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
                }

                var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

                defines.Add($"{dependecyRecipe.Name}_VersionNamespace={dependecyRecipe.Name}::{GetNamespace(dependecyRecipe.Version)}");
            }

            var args = new CompilerArguments()
            {
                Standard = Compiler.LanguageStandard.Latest,
                RootDirectory = path,
                OutputDirectory = objectDirectory,
                PreprocessorDefinitions = defines,
                SourceFiles = new List<string>() { recipe.Public },
                IncludeDirectories = uniqueFolders,
                ExportModule = true,
            };

            // Ensure the object directory exists
            var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
            if (!Directory.Exists(objectDirectry))
            {
                Directory.CreateDirectory(objectDirectry);
            }

            // Compile each file
            await _compiler.CompileAsync(args);
        }

        /// <summary>
        /// Compile the supporting source files
        /// </summary>
        private async Task CompileSourceAsync(string path, Recipe recipe, IList<string> uniqueFolders, string objectDirectory)
        {
            Log.Info("Compile Source");
            var modules = new List<string>();
            var defines = new List<string>();

            defines.Add("SOUP_BUILD");
            if (recipe.Type == RecipeType.Library)
            {
                // Add a reference to our own modules interface definition
                modules.Add(Path.Combine(objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.ifc"));
                defines.Add($"{recipe.Name}_VersionNamespace={recipe.Name}::{GetNamespace(recipe.Version)}");
            }

            // Add all of the direct dependencies as module references
            // and set their version defintions
            foreach (var dependecy in recipe.Dependencies)
            {
                // Load this package recipe
                string packagePath;
                if (dependecy.Path != null)
                {
                    packagePath = dependecy.Path;
                }
                else
                {
                    packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
                }

                var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

                modules.Add(Path.Combine(packagePath, "bin", $"{dependecyRecipe.Name}.ifc"));
                defines.Add($"{dependecyRecipe.Name}_VersionNamespace={dependecyRecipe.Name}::{GetNamespace(dependecyRecipe.Version)}");
            }

            var source = new List<string>(recipe.Source);
            if (recipe.Type == RecipeType.Library)
            {
                // Add the precompile module to the list of source files
                source.Add(Path.Combine(objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.pcm"));
            }

            var args = new CompilerArguments()
            {
                Standard = Compiler.LanguageStandard.Latest,
                RootDirectory = path,
                OutputDirectory = objectDirectory,
                PreprocessorDefinitions = defines,
                SourceFiles = source,
                IncludeDirectories = uniqueFolders,
                Modules = modules,
            };

            // Ensure the object directory exists
            var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
            if (!Directory.Exists(objectDirectry))
            {
                Directory.CreateDirectory(objectDirectry);
            }

            // Compile each file
            await _compiler.CompileAsync(args);
        }

        /// <summary>
        /// Link the resulting object files
        /// </summary>
        private async Task LinkLibraryAsync(string path, Recipe recipe, string objectDirectory, string binaryDirectory)
        {
            Log.Info("Link");
            var allFiles = new List<string>(recipe.Source);
            if (recipe.Type == RecipeType.Library)
            {
                allFiles.Add(recipe.Public);
            }

            // Convert all source files into objects
            var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.obj").ToList();

            // Add the modules object too
            objectFiles.Add($"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(recipe.Public)}.obj");

            var args = new LinkerArguments()
            {
                Name = recipe.Name,
                RootDirectory = path,
                OutputDirectory = binaryDirectory,
                SourceFiles = objectFiles,
            };

            // Ensure the object directory exists
            var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
            if (!Directory.Exists(objectDirectry))
            {
                Directory.CreateDirectory(objectDirectry);
            }

            // Link
            await _compiler.LinkLibraryAsync(args);
        }

        /// <summary>
        /// Link the executable
        /// </summary>
        private async Task LinkExecutableAsync(
            string path,
            Recipe recipe,
            string objectDirectory,
            string binaryDirectory)
        {
            Log.Info("Link");
            var allFiles = new List<string>(recipe.Source);
            if (recipe.Type == RecipeType.Library)
            {
                allFiles.Add(recipe.Public);
            }

            // Add all of the dependencies as module references
            var librarySet = new HashSet<string>();
            await GenerateDependencyLibrarySetAsync(recipe, librarySet);

            var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.obj").ToList();
            var libraryFiles = librarySet.ToList();
            var args = new LinkerArguments()
            {
                Name = recipe.Name,
                RootDirectory = path,
                OutputDirectory = binaryDirectory,
                SourceFiles = objectFiles,
                LibraryFiles = libraryFiles,
            };

            // Ensure the object directory exists
            var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
            if (!Directory.Exists(objectDirectry))
            {
                Directory.CreateDirectory(objectDirectry);
            }

            // Link
            await _compiler.LinkExecutableAsync(args);
        }

        /// <summary>
        /// Clone the module interface definition from the object directoy to the binary directory
        /// </summary>
        private void CloneModuleInterface(string path, Recipe recipe, string objectDirectory, string binaryDirectory)
        {
            Log.Verbose("Clone Module Interface");
            var sourceModuleFile = Path.Combine(path, objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.pcm");
            var targetModuleFile = Path.Combine(path, binaryDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Name)}.pcm");

            // Ensure the object directory exists
            if (!File.Exists(sourceModuleFile))
            {
                throw new InvalidOperationException("The resulting module interface definition was missing.");
            }

            Log.Verbose($"Clone PCM: {sourceModuleFile} -> {targetModuleFile}");
            File.Copy(sourceModuleFile, targetModuleFile, true);
        }

        private async Task GenerateDependencyLibrarySetAsync(Recipe recipe, HashSet<string> set)
        {
            foreach (var dependecy in recipe.Dependencies)
            {
                // Load this package recipe
                string packagePath;
                if (dependecy.Path != null)
                {
                    packagePath = dependecy.Path;
                }
                else
                {
                    packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
                }

                // Get recursive dependencies
                var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);
                await GenerateDependencyLibrarySetAsync(dependecyRecipe, set);

                set.Add(Path.Combine(packagePath, "bin", $"{dependecyRecipe.Name}.a").ToLower());
            }
        }

        private string GetNamespace(SemanticVersion version)
        {
            return $"v{version.Major}_{version.Minor}_{version.Patch}";
        }
    }
}
