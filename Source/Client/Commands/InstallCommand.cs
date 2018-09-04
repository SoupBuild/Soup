// <copyright file="InstallCommand.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using System;
    using System.IO;
    using System.Linq;
    using System.Threading.Tasks;
    using Soup.Api;

    /// <summary>
    /// Install Command
    /// </summary>
    internal class InstallCommand
    {
        private LocalUserConfig _config;
        private ISoupApi _soupApi;

        /// <summary>
        /// Initializes a new instance of the <see cref="InstallCommand"/> class.
        /// </summary>
        public InstallCommand(
            LocalUserConfig config,
            ISoupApi soupApi)
        {
            _config = config;
            _soupApi = soupApi;
        }

        /// <summary>
        /// Invoke the install command
        /// </summary>
        public async Task InvokeAsync(InstallOptions options)
        {
            var workingDirectory = @"./";
            Recipe recipe = null;
            try
            {
                recipe = await RecipeManager.LoadFromFileAsync(workingDirectory);
            }
            catch (FileNotFoundException)
            {
                Log.Error("The recipe file is required to install.");
                return;
            }

            var stagingPath = PackageManager.EnsureStagingDirectoryExists(_config.PackageStore);

            // Ensure that the staging directory exists
            var tempStagingPath = Path.Combine(stagingPath, "temp");

            var package = options.Package;
            if (string.IsNullOrEmpty(package))
            {
                Log.Info("Install All");

                await InstallRecursiveDependencies(tempStagingPath, recipe);
            }
            else if (!Path.HasExtension(package))
            {
                Log.Info($"Install Package: {package}");

                // Check if the package is already installed
                if (recipe.Dependencies.Any(dependency => dependency.Name == package))
                {
                    Log.Warning("Packge already installed.");
                    return;
                }

                // Get the latest version
                var latestVersion = await GetLatestAsync(package);

                // Download the archive
                using (var archiveStream = await DownloadPackageAsync(package, latestVersion))
                {
                    // Install the package
                    var installedRecipe = await InstallPackageAsync(tempStagingPath, archiveStream);
                    var installedPackageRef = new PackageReference(installedRecipe.Name, installedRecipe.Version);

                    // Register the package in the recipe
                    recipe.Dependencies.Add(installedPackageRef);
                }
            }
            else if (Path.GetExtension(package) == Constants.ArchiveFileExtension)
            {
                Log.Info($"Installing Local Package: {package}");

                if (!File.Exists(package))
                {
                    throw new ArgumentException("The specified file does not exist.");
                }

                // Install the package
                using (var archiveStream = File.OpenRead(package))
                {
                    var installedRecipe = await InstallPackageAsync(tempStagingPath, archiveStream);
                    var installedPackageRef = new PackageReference(installedRecipe.Name, installedRecipe.Version);

                    // Register the package in the recipe if it does not exist
                    if (!recipe.Dependencies.Any(dependency => dependency == installedPackageRef))
                    {
                        recipe.Dependencies.Add(installedPackageRef);
                    }
                }
            }
            else
            {
                Log.Error("Unknown install source type.");
                Directory.Delete(tempStagingPath, true);
                return;
            }

            // Cleanup the working directory
            if (Directory.Exists(tempStagingPath))
            {
                Directory.Delete(tempStagingPath, true);
            }

            // Save the state of the recipe if it has changed
            if (recipe.IsDirty)
            {
                await RecipeManager.SaveToFileAsync(recipe);
            }
        }

        private async Task<SemanticVersion> GetLatestAsync(string name)
        {
            var package = await _soupApi.GetPackageAsync(name);
            return package.Latest;
        }

        private async Task<Stream> DownloadPackageAsync(string name, SemanticVersion version)
        {
            var stream = await _soupApi.DownloadPackageAsync(name, version);
            return stream;
        }

        private async Task<Recipe> InstallPackageAsync(string tempPath, Stream packageFile)
        {
            var recipe = await UnpackArchiveAsync(tempPath, packageFile);
            return recipe;
        }

        private async Task<Recipe> UnpackArchiveAsync(string tempPath, Stream packageFile)
        {
            // Unpack the zip file into the package directory
            var tempPackagePath = Path.Combine(tempPath, Constants.StorePackageFolderName);
            Directory.CreateDirectory(tempPackagePath);
            await PackageManager.ExtractAsync(packageFile, tempPackagePath);

            // Load the packages recipe file
            var recipe = await RecipeManager.LoadFromFileAsync(tempPackagePath);
            var packagePath = PackageManager.BuildKitchenPackagePath(_config, recipe);

            // TODO : Perform some verification that the package is valid

            // TODO : Should not hit this when, verify the package exists before download
            // For now delete and recreate it
            if (Directory.Exists(packagePath))
            {
                Directory.Delete(packagePath, true);
            }

            // Ensure the parent directory exists
            var packageParentDirectory = Directory.GetParent(packagePath);
            if (!packageParentDirectory.Exists)
            {
                packageParentDirectory.Create();
            }

            // Move the results out of the staging directory
            Directory.Move(tempPackagePath, packagePath);

            return recipe;
        }

        /// <summary>
        /// Recusively install all dependencies and trasient dependecies
        /// </summary>
        private async Task InstallRecursiveDependencies(string tempPath, Recipe recipe)
        {
            foreach (var dep in recipe.Dependencies)
            {
                Log.Info($"{dep}");

                // Download the archive
                using (var archiveStream = await DownloadPackageAsync(dep.Name, dep.Version))
                {
                    // Install the package
                    var installedRecipe = await InstallPackageAsync(tempPath, archiveStream);

                    // Install dependecies recursively
                    await InstallRecursiveDependencies(tempPath, installedRecipe);
                }
            }
        }
    }
}
