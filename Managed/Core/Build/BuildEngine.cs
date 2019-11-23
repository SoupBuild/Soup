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
        public async Task ExecuteAsync(string path, Recipe recipe, bool force)
        {
            if (!Path.IsPathFullyQualified(path))
            {
                throw new InvalidOperationException("Build must provide qualified path.");
            }

            Log.Info("Build Recursive Dependencies.");
            await BuildAllDependenciesRecursivelyAsync(path, recipe, force);

            Log.Info("Build Toplevel Recipe.");
            await CoreBuildAsync(path, recipe, force);
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        private async Task CoreBuildAsync(string path, Recipe recipe, bool force)
        {
            var configuration = "release";
            var objectDirectory = Path.Combine("out", "obj", _compiler.Name, configuration);
            var binaryDirectory = Path.Combine("out", "bin", _compiler.Name, configuration);

            Log.Info($"Building {recipe.Name}.");
            Log.Verbose($"Loading previous build state.");
            var buildState = await BuildStateManager.LoadFromFileAsync(path);
            if (buildState == null)
            {
                Log.Verbose($"No previous state found.");
                buildState = new BuildState();
            }

            // Determine the include paths
            var includePaths = recipe.IncludePaths ?? new List<string>();

            bool moduleBuilt = false;
            if (recipe.Type == RecipeType.Library)
            {
                moduleBuilt = await CheckCompileModuleAsync(path, recipe, buildState, includePaths, objectDirectory, binaryDirectory, force);
            }

            bool sourceBuilt = await CheckCompileSourceAsync(
                path,
                recipe,
                buildState,
                includePaths,
                objectDirectory,
                binaryDirectory,
                force);

            switch (recipe.Type)
            {
                case RecipeType.Library:
                    await CheckLinkLibraryAsync(path, recipe, objectDirectory, binaryDirectory, force);
                    break;
                case RecipeType.Executable:
                    await LinkExecutableAsync(path, recipe, objectDirectory, binaryDirectory);
                    break;
                default:
                    throw new NotSupportedException("Unknown recipe type.");
            }

            if (moduleBuilt)
            {
                CloneModuleInterface(path, recipe, objectDirectory, binaryDirectory);
            }

            // Save the build state
            await BuildStateManager.SaveToFileAsync(buildState, path);
        }

        /// <summary>
        /// Build the dependecies for the provided recipe recursively
        /// </summary>
        private async Task BuildAllDependenciesRecursivelyAsync(
            string path,
            Recipe recipe,
            bool force)
        {
            Log.Info($"Searching Dependencies: {recipe.Name}.");
            foreach (var dependecy in recipe.Dependencies)
            {
                // Load this package recipe
                var packagePath = VerifyDependencyPath(path, dependecy);
                var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);
                if (dependecyRecipe == null)
                {
                    Log.Error($"Failed to load the dependency package: {packagePath}");
                    throw new InvalidOperationException();
                }

                // Build all recursive dependencies
                await BuildAllDependenciesRecursivelyAsync(packagePath, dependecyRecipe, force);

                // Build this dependecy
                await CoreBuildAsync(packagePath, dependecyRecipe, force);
            }
        }

        /// <summary>
        /// Compile the module file
        /// </summary>
        private async Task<bool> CheckCompileModuleAsync(
            string path,
            Recipe recipe,
            BuildState buildState,
            IList<string> includePaths,
            string objectDirectory,
            string binaryDirectory,
            bool force)
        {
            var outputFilename = $"{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ModuleFileExtension}";
            var outputFile = Path.Combine(objectDirectory, outputFilename);
            bool requiresBuild = true;
            if (!force)
            {
                // Add all of the direct dependencies as module references
                var modules = new List<string>();
                var defines = new List<string>();
                bool isRecursive = false;
                await BuildDependencyModuleReferences(path, binaryDirectory, recipe, modules, defines, isRecursive);

                // The dependencies for this file are all of the direct module references
                var dependencies = new List<string>();
                dependencies.AddRange(modules);

                if (!BuildRequiredChecker.IsSourceFileOutdated(path, buildState, outputFile, recipe.Public, dependencies))
                {
                    // TODO : This is a hack. We need to actually look through all of the imports for the module file
                    Log.Info("Module file is up to date.");
                    requiresBuild = false;
                }
            }

            if (requiresBuild)
            {
                await CompileModuleAsync(
                    path,
                    recipe,
                    buildState,
                    includePaths,
                    objectDirectory,
                    binaryDirectory);
            }

            return requiresBuild;
        }

        /// <summary>
        /// Compile the module file
        /// </summary>
        private async Task CompileModuleAsync(
            string path,
            Recipe recipe,
            BuildState buildState,
            IList<string> includePaths,
            string objectDirectory,
            string binaryDirectory)
        {
            Log.Info("Compile Module");

            if (string.IsNullOrEmpty(recipe.Public))
            {
                throw new InvalidOperationException("The public file was not set.");
            }

            var modules = new List<string>();
            var defines = new List<string>();

            defines.Add("SOUP_BUILD");

            // Set the active version namespace
            defines.Add(BuildRecipeNamespaceDefine(recipe));

            // Add all of the direct dependencies as module references
            // and set their version defintions

            // TODO: MSVC requires all transient modules also
            bool isRecursive = _compiler.Name == "MSVC";
            await BuildDependencyModuleReferences(path, binaryDirectory, recipe, modules, defines, isRecursive);

            // TODO: Clang wants modules to be cppm
            var publicFile = recipe.Public;
            if (_compiler.Name == "Clang")
            {
                publicFile = Path.GetFileNameWithoutExtension(publicFile) + ".cppm";
            }

            var args = new CompileArguments()
            {
                Standard = Compiler.LanguageStandard.Latest,
                RootDirectory = path,
                OutputDirectory = objectDirectory,
                PreprocessorDefinitions = defines,
                SourceFiles = new List<string>() { publicFile },
                IncludeDirectories = includePaths,
                Modules = modules,
                ExportModule = true,
                GenerateIncludeTree = true,
            };

            // Ensure the object directory exists
            var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
            if (!Directory.Exists(objectDirectry))
            {
                Directory.CreateDirectory(objectDirectry);
            }

            // Compile each file
            var result = await _compiler.CompileAsync(args);

            // Save the build state
            if (result.HeaderIncludeFiles != null)
                buildState.UpdateIncludeTree(result.HeaderIncludeFiles);
        }

        /// <summary>
        /// Compile the supporting source files
        /// </summary>
        private async Task<bool> CheckCompileSourceAsync(
            string path,
            Recipe recipe,
            BuildState buildState,
            IList<string> includePaths,
            string objectDirectory,
            string binaryDirectory,
            bool force)
        {
            var modules = new List<string>();
            var defines = new List<string>();

            defines.Add("SOUP_BUILD");
            if (recipe.Type == RecipeType.Library)
            {
                // Add a reference to our own modules interface definition
                var modulePath = Path.Combine(
                    objectDirectory,
                    $"{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ModuleFileExtension}");
                modules.Add(modulePath);
                defines.Add(BuildRecipeNamespaceDefine(recipe));
            }

            // Add all of the direct dependencies as module references
            // and set their version defintions
            // TODO: MSVC requires all transient modules also
            bool isRecursive = _compiler.Name == "MSVC";
            await BuildDependencyModuleReferences(path, binaryDirectory, recipe, modules, defines, isRecursive);

            var source = new List<string>();

            // All files are dependent on the parent module and all referenced modules
            var sharedDependecies = new List<string>();
            sharedDependecies.AddRange(modules);

            // Check if the precompiled module should be compiled
            if (recipe.Type == RecipeType.Library)
            {
                // Add the precompile module to the list of shared dependencies
                // TODO: Could optimize this to not do file datetime checks over again
                var moduleFile = Path.Combine(path, objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ModuleFileExtension}");
                sharedDependecies.Add(moduleFile);

                var moduleOutputFile = Path.Combine(path, objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ObjectFileExtension}");
                if (force || BuildRequiredChecker.IsOutdated(path, moduleOutputFile, sharedDependecies))
                {
                    // HACK 
                    if (_compiler.Name == "Clang")
                    {
                        source.Add(moduleFile);
                    }
                }
            }

            // Check if each source file is out of date and requires a rebuild
            foreach (var sourceFile in recipe.Source)
            {
                var outputFile = Path.Combine(objectDirectory, $"{Path.GetFileNameWithoutExtension(sourceFile)}.{_compiler.ObjectFileExtension}");
                if (force || BuildRequiredChecker.IsSourceFileOutdated(path, buildState, outputFile, sourceFile, sharedDependecies))
                {
                    source.Add(sourceFile);
                }
            }

            if (source.Count == 0)
            {
                Log.Info("All source is up to date.");
                return false;
            }
            else
            {
                Log.Info("Compile Source");
                var args = new CompileArguments()
                {
                    Standard = Compiler.LanguageStandard.Latest,
                    RootDirectory = path,
                    OutputDirectory = objectDirectory,
                    PreprocessorDefinitions = defines,
                    SourceFiles = source,
                    IncludeDirectories = includePaths,
                    Modules = modules,
                    GenerateIncludeTree = true,
                };

                // Ensure the object directory exists
                var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
                if (!Directory.Exists(objectDirectry))
                {
                    Directory.CreateDirectory(objectDirectry);
                }


                // Compile each file
                var result = await _compiler.CompileAsync(args);

                // Save the build state
                if (result.HeaderIncludeFiles != null)
                    buildState.UpdateIncludeTree(result.HeaderIncludeFiles);

                return true;
            }
        }

        private async Task BuildDependencyModuleReferences(
            string path,
            string binaryDirectory,
            Recipe recipe,
            IList<string> modules,
            IList<string> defines,
            bool isRecursive)
        {
            foreach (var dependecy in recipe.Dependencies)
            {
                // Load this package recipe
                var packagePath = VerifyDependencyPath(path, dependecy);
                var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

                modules.Add(Path.Combine(packagePath, binaryDirectory, BuildRecipeModuleFilename(dependecyRecipe)));
                defines.Add(BuildRecipeNamespaceDefine(dependecyRecipe));

                if (isRecursive)
                {
                    await BuildDependencyModuleReferences(packagePath, binaryDirectory, dependecyRecipe, modules, defines, isRecursive);
                }
            }
        }

        /// <summary>
        /// Link the resulting object files
        /// </summary>
        private async Task CheckLinkLibraryAsync(
            string path,
            Recipe recipe,
            string objectDirectory,
            string binaryDirectory,
            bool force)
        {
            var allFiles = new List<string>(recipe.Source);
            if (recipe.Type == RecipeType.Library)
            {
                allFiles.Add(recipe.Public);
            }

            // Convert all source files into objects
            var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.{_compiler.ObjectFileExtension}").ToList();

            // Add the modules object too
            objectFiles.Add($"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ObjectFileExtension}");

            var targetLibraryFile = Path.Combine(binaryDirectory, $"{recipe.Name}.{_compiler.StaticLibraryFileExtension}");
            if (force || BuildRequiredChecker.IsOutdated(path, targetLibraryFile, objectFiles))
            {
                Log.Info("Link library");
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
            else
            {
                Log.Info("Static library up to date.");
            }
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
            Log.Info("Link Executable");
            var allFiles = new List<string>(recipe.Source);
            if (recipe.Type == RecipeType.Library)
            {
                allFiles.Add(recipe.Public);
            }

            // Add all of the dependencies as module references
            var librarySet = new HashSet<string>();
            await GenerateDependencyLibrarySetAsync(path, binaryDirectory, recipe, librarySet);

            var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.{_compiler.ObjectFileExtension}").ToList();
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

            // HACK since GCC places stuff in a different folder
            string sourceModuleFile;
            if (_compiler.Name == "GCC")
            {
                sourceModuleFile = Path.Combine(path, objectDirectory,"gcm.cache", BuildRecipeModuleFilename(recipe));
            }
            else
            {
                sourceModuleFile = Path.Combine(path, objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.{_compiler.ModuleFileExtension}");
            }

            var targetModuleFile = Path.Combine(path, binaryDirectory, BuildRecipeModuleFilename(recipe));


            // Ensure the object directory exists
            if (!File.Exists(sourceModuleFile))
            {
                throw new InvalidOperationException($"The resulting module interface definition was missing: {sourceModuleFile}");
            }

            Log.Verbose($"Clone Module: {sourceModuleFile} -> {targetModuleFile}");
            File.Copy(sourceModuleFile, targetModuleFile, true);
        }

        private async Task GenerateDependencyLibrarySetAsync(
            string path,
            string binaryDirectory,
            Recipe recipe,
            HashSet<string> set)
        {
            foreach (var dependecy in recipe.Dependencies)
            {
                // Load this package recipe
                var packagePath = VerifyDependencyPath(path, dependecy);
                var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

                // Get recursive dependencies
                await GenerateDependencyLibrarySetAsync(packagePath, binaryDirectory, dependecyRecipe, set);

                set.Add(Path.Combine(packagePath, binaryDirectory, $"{dependecyRecipe.Name}.{_compiler.StaticLibraryFileExtension}").ToLower());
            }
        }

        private string VerifyDependencyPath(string path, PackageReference dependecy)
        {
            string packagePath;
            if (dependecy.Path != null)
            {
                // Build the relative path
                Log.Verbose($"Local Dependecy: {dependecy.Path}");
                if (!Path.IsPathRooted(dependecy.Path))
                {
                    packagePath = Path.GetFullPath(Path.Combine(path, dependecy.Path));
                }
                else
                {
                    packagePath = dependecy.Path;
                }

                Log.Verbose($"Path: {packagePath}");

                // Verify the package exists
                if (!Directory.Exists(packagePath))
                {
                    Log.Error($"The local package reference folder does not exist: {packagePath}");
                    throw new InvalidOperationException();
                }
            }
            else
            {
                packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
            }

            return packagePath;
        }

        private string BuildRecipeNamespaceDefine(Recipe recipe)
        {
            return $"{recipe.Name.Replace(".", "_")}_VersionNamespace={recipe.Name}::{GetNamespace(recipe.Version)}";
        }

        private string BuildRecipeModuleFilename(Recipe recipe)
        {
            return $"{recipe.Name}.{_compiler.ModuleFileExtension}";
        }

        private string GetNamespace(SemanticVersion version)
        {
            return $"v{version.Major}_{version.Minor}_{version.Patch}";
        }
    }
}
