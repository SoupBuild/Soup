// <copyright file="PackageManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using Opal;

namespace Soup.Build.PackageManager
{
    using System;
    using System.Collections.Generic;
    using System.IO.Compression;
    using System.Net.Http;
    using System.Threading.Tasks;
    using Opal;
    using Opal.System;
    using Soup.Build.Utilities;

    /// <summary>
    /// The package manager
    /// </summary>
    public class PackageManager
    {
        private static string StagingFolderName => ".staging/";

        /// <summary>
        /// Install packages
        /// </summary>
        public static async Task InstallPackagesAsync(Path workingDirectory)
        {
            var packageStore = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
                new Path(".soup/packages/");
            Log.Info("Using Package Store: " + packageStore.ToString());

            // Create the staging directory
            var stagingPath = EnsureStagingDirectoryExists(packageStore);

            try
            {
                await InstallRecursiveDependenciesAsync(
                    workingDirectory,
                    packageStore,
                    stagingPath);

                // Cleanup the working directory
                Log.Info("Deleting staging directory");
                LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);
            }
            catch (Exception)
            {
                // Cleanup the staging directory and accept that we failed
                LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);
                throw;
            }
        }

        /// <summary>
        /// Install a package
        /// </summary>
        public static async Task InstallPackageReferenceAsync(Path workingDirectory, string packageReference)
        {
            var recipePath =
                workingDirectory +
                BuildConstants.RecipeFileName;
            var (isSuccess, recipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipePath);
            if (!isSuccess)
            {
                throw new InvalidOperationException("Could not load the recipe file.");
            }

            var packageStore = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
                new Path(".soup/packages/");
            Log.Info("Using Package Store: " + packageStore.ToString());

            // Create the staging directory
            var stagingPath = EnsureStagingDirectoryExists(packageStore);

            try
            {
                // Parse the package reference to get the name
                var targetPackageReference = PackageReference.Parse(packageReference);
                string packageName = packageReference;
                if (!targetPackageReference.IsLocal)
                {
                    packageName = targetPackageReference.GetName;
                }

                // Check if the package is already installed
                var packageNameNormalized = packageName.ToUpperInvariant();
                if (recipe.HasRuntimeDependencies)
                {
                    foreach (var dependency in recipe.RuntimeDependencies)
                    {
                        if (!dependency.IsLocal)
                        {
                            var dependencyNameNormalized = dependency.GetName.ToUpperInvariant();
                            if (dependencyNameNormalized == packageNameNormalized)
                            {
                                Log.Warning("Package already installed.");
                                return;
                            }
                        }
                    }
                }

                // Get the latest version if no version provided
                if (targetPackageReference.IsLocal)
                {
                    var packageModel = await GetPackageModelAsync(packageReference);
                    var latestVersion = new SemanticVersion(packageModel.Latest.Major, packageModel.Latest.Minor, packageModel.Latest.Patch);
                    Log.HighPriority("Latest Version: " + latestVersion.ToString());
                    targetPackageReference = new PackageReference(packageModel.Name, latestVersion);
                }

                await EnsurePackageDownloadedAsync(
                    targetPackageReference.GetName,
                    targetPackageReference.Version,
                    packageStore,
                    stagingPath);

                // Cleanup the working directory
                Log.Info("Deleting staging directory");
                LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);

                // Register the package in the recipe
                Log.Info("Adding reference to recipe");
                IList<PackageReference> dependencies = new List<PackageReference>();
                if (recipe.HasRuntimeDependencies)
                    dependencies = recipe.RuntimeDependencies;

                dependencies.Add(targetPackageReference);
                recipe.RuntimeDependencies = dependencies;

                // Save the state of the recipe
                await RecipeExtensions.SaveToFileAsync(recipePath, recipe);
            }
            catch (Exception)
            {
                // Cleanup the staging directory and accept that we failed
                LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);
                throw;
            }
        }

        /////// <summary>
        /////// Publish a package
        /////// </summary>
        ////static void PublishPackage(Path workingDirectory)
        ////{
        ////    Log.Info("Publish Project: {recipe.Name}@{recipe.Version}");

        ////    var recipePath =
        ////        workingDirectory +
        ////        BuildConstants.RecipeFileName;
        ////    Recipe recipe = null;
        ////    if (!RecipeExtensions.TryLoadRecipeFromFile(recipePath, recipe))
        ////    {
        ////        throw new InvalidOperationException("Could not load the recipe file.");
        ////    }

        ////    var packageStore = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
        ////        new Path(".soup/packages/");
        ////    Log.Info("Using Package Store: " + packageStore.ToString());

        ////    // Create the staging directory
        ////    var stagingPath = EnsureStagingDirectoryExists(packageStore);

        ////    try
        ////    {
        ////        var archivePath = stagingPath + new Path(recipe.Name + ".7z");
        ////        var files = GetPackageFiles(workingDirectory);

        ////        // Create the archive of the package
        ////        ////{
        ////        ////    var archiveStream = LifetimeManager.Get<IFileSystem>().OpenWrite(archivePath, true);
        ////        ////    var outStream = make_shared<LzmaOutStream>(archiveStream);
        ////        ////    var archive = LzmaSdk.ArchiveWriter(outStream);
        ////        ////    archive.AddFiles(files);

        ////        ////    var callback = make_shared<LzmaUpdateCallback>(workingDirectory);
        ////        ////    archive.Save(callback);
        ////        ////}

        ////        // Authenticate the user
        ////        Log.Info("Request Authentication Token");
        ////        var userDetails = GetUserDetails();
        ////        var openIdConfiguration = Api.SoupAuth.GetOpenIdConfiguration();
        ////        var tokenResult = Api.SoupAuth.RequestClientCredentialsToken(
        ////            openIdConfiguration.TokenEndpoint,
        ////            userDetails.UserName,
        ////            userDetails.Password);
        ////        switch (tokenResult.Status)
        ////        {
        ////            case Api.RequestClientCredentialsTokenResult.Success:
        ////                // All good
        ////                break;
        ////            case Api.RequestClientCredentialsTokenResult.InvalidUserNameOrPassword:
        ////                Log.HighPriority("Invalid UserName or Password");
        ////                return;
        ////            default:
        ////                throw runtime_error("Unknown RequestClientCredentialsTokenResult");
        ////        }

        ////        // Publish the archive, scope cleanup file access
        ////        {
        ////            Log.Info("Publish package");
        ////            var archiveFile = LifetimeManager.Get<IFileSystem>().OpenRead(archivePath, true);
        ////            var publishResult = Api.SoupApi.PublishPackage(
        ////                tokenResult.Result,
        ////                recipe.GetName(),
        ////                recipe.GetVersion(),
        ////                archiveFile->GetInStream());

        ////            // Check if we should publish the package
        ////            bool createPackageRequired = false;
        ////            switch (publishResult)
        ////            {
        ////                case Api.PublishPackageResult.Success:
        ////                    // All Good
        ////                    Log.Info("Package version created");
        ////                    break;
        ////                case Api.PublishPackageResult.PackageDoesNotExist:
        ////                    // Not found indicates the package does not exist, create it
        ////                    Log.HighPriority("The provided package name does not exist");
        ////                    createPackageRequired = true;
        ////                    break;
        ////                case Api.PublishPackageResult.AlreadyExists:
        ////                    Log.HighPriority("A Package with this version already exists");
        ////                    break;
        ////                case Api.PublishPackageResult.Forbidden:
        ////                    Log.HighPriority("User account forbidden to create package. Make sure you have verified your email.");
        ////                    break;
        ////                default:
        ////                    throw runtime_error("Unknown PublishPackageResult");
        ////            }

        ////            // Create the package if needed and retry
        ////            if (createPackageRequired)
        ////            {
        ////                // Create the package
        ////                Log.HighPriority("Create package");
        ////                var createModel = Api.PackageCreateOrUpdateModel();
        ////                var createdPackage = Api.SoupApi.CreatePackage(
        ////                    tokenResult.Result,
        ////                    recipe.GetName,
        ////                    createModel);

        ////                switch (createdPackage.first)
        ////                {
        ////                    case Api.CreatePackageResult.Success:
        ////                        {
        ////                            Log.Info("Package version created");

        ////                            // Retry
        ////                            Log.HighPriority("Retry publish package");

        ////                            // Reset the archive file
        ////                            var & archiveStream = archiveFile->GetInStream();
        ////                            archiveStream.clear();
        ////                            archiveStream.seekg(0, ios.beg);

        ////                            publishResult = Api.SoupApi.PublishPackage(
        ////                                tokenResult.Result,
        ////                                recipe.GetName(),
        ////                                recipe.GetVersion(),
        ////                                archiveStream);
        ////                            switch (publishResult)
        ////                            {
        ////                                case Api.PublishPackageResult.Success:
        ////                                    // All Good
        ////                                    Log.Info("Package version created");
        ////                                    break;
        ////                                default:
        ////                                    throw runtime_error("Unknown PublishPackageResult");
        ////                            }
        ////                            break;
        ////                        }
        ////                    case Api.CreatePackageResult.Forbidden:
        ////                        {
        ////                            Log.HighPriority("User account forbidden to create package. Make sure you have verified your email.");
        ////                            break;
        ////                        }
        ////                    default:
        ////                        {
        ////                            throw new InvalidOperationException("Unknown PublishPackageResult");
        ////                        }
        ////                }
        ////            }
        ////        }

        ////        // Cleanup the staging directory
        ////        Log.Info("Cleanup staging directory");
        ////        LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);
        ////    }
        ////    catch (Exception)
        ////    {
        ////        // Cleanup the staging directory and accept that we failed
        ////        Log.Info("Publish Failed: Cleanup staging directory");
        ////        LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);
        ////        throw;
        ////    }
        ////}

        private static async Task<Api.Client.PackageModel> GetPackageModelAsync(string name)
        {
            using (var httpClient = new HttpClient())
            {
                var client = new Api.Client.PackageClient(httpClient);
                return await client.GetPackageAsync(name);
            }
        }

        ////  private static IList<LzmaSdk.FileProperties> GetPackageFiles(Path workingDirectory)
        ////  {
        ////      var result = new List<LzmaSdk.FileProperties>();

        ////      foreach (var child in LifetimeManager.Get<IFileSystem>().GetDirectoryChildren(workingDirectory))
        ////      {
        ////          if (child.IsDirectory)
        ////          {
        ////              // Ignore output folder
        ////              if (child.Path.GetFileName() != "out")
        ////              {
        ////                  var directoryFiles = GetAllFilesRecursive(child.Path, workingDirectory);
        ////                  result.insert(
        ////                      result.end(),
        ////                      directoryFiles.begin(),
        ////                      directoryFiles.end());
        ////              }
        ////          }
        ////          else
        ////          {
        ////              var relativePath = child.Path.GetRelativeTo(workingDirectory);
        ////              var fileProperties = LzmaSdk.FileProperties();
        ////              fileProperties.Name = relativePath.ToString();
        ////              fileProperties.Size = child.Size;
        ////              fileProperties.CreateTime = child.CreateTime;
        ////              fileProperties.AccessTime = child.AccessTime;
        ////              fileProperties.ModifiedTime = child.ModifiedTime;
        ////              fileProperties.Attributes = child.Attributes;
        ////              result.push_back(move(fileProperties));
        ////          }
        ////      }

        ////      return result;
        ////  }

        ////  static IList<LzmaSdk.FileProperties> GetAllFilesRecursive(Path directory, Path workingDirectory)
        ////  {
        ////      var result = new List<LzmaSdk.FileProperties>();

        ////      for (var child : LifetimeManager.Get<IFileSystem>().GetDirectoryChildren(directory))
        ////      {
        ////          if (child.IsDirectory)
        ////          {
        ////              var directoryFiles = GetAllFilesRecursive(child.Path, workingDirectory);
        ////              result.Add(
        ////                  result.end(),
        ////                  directoryFiles.begin(),
        ////                  directoryFiles.end());
        ////          }
        ////          else
        ////          {
        ////              var relativePath = child.Path.GetRelativeTo(workingDirectory);
        ////              var fileProperties = LzmaSdk.FileProperties();
        ////              fileProperties.Name = relativePath.ToString();
        ////              fileProperties.Size = child.Size;
        ////              fileProperties.CreateTime = child.CreateTime;
        ////              fileProperties.AccessTime = child.AccessTime;
        ////              fileProperties.ModifiedTime = child.ModifiedTime;
        ////              fileProperties.Attributes = child.Attributes;
        ////              result.Add(move(fileProperties));
        ////          }
        ////      }

        ////      return result;
        ////  }

        /// <summary>
        /// Ensure a package version is downloaded
        /// </summary>
        private static async Task EnsurePackageDownloadedAsync(
            string packageName,
             SemanticVersion packageVersion,
             Path packagesDirectory,
             Path stagingDirectory)
        {
            Log.HighPriority("Install Package: " + packageName + "@" + packageVersion.ToString());

            var packageRootFolder = packagesDirectory + new Path(packageName);
            var packageVersionFolder = packageRootFolder + new Path(packageVersion.ToString()) + new Path("/");

            // Check if the package version already exists
            if (LifetimeManager.Get<IFileSystem>().Exists(packageVersionFolder))
            {
                Log.HighPriority("Found local version");
            }
            else
            {
                // Download the archive
                Log.HighPriority("Downloading package");
                var archivePath = stagingDirectory + new Path(packageName + ".zip");
                using (var httpClient = new HttpClient())
                {
                    var client = new Api.Client.PackageVersionClient(httpClient);
                    var result = await client.DownloadPackageVersionAsync(packageName, packageVersion.ToString());

                    // Write the contents to disk, scope cleanup
                    using (var archiveWriteFile = LifetimeManager.Get<IFileSystem>().OpenWrite(archivePath, true))
                    {
                        await result.Stream.CopyToAsync(archiveWriteFile.GetOutStream());
                    }
                }

                // Create the package folder to extract to
                var stagingVersionFolder = stagingDirectory + new Path(packageName) + new Path(packageVersion.ToString()) + new Path("/");
                LifetimeManager.Get<IFileSystem>().CreateDirectory2(stagingVersionFolder);

                // Unpack the contents of the archive
                ZipFile.ExtractToDirectory(archivePath.ToString(), stagingVersionFolder.ToString());

                // Install transitive dependencies
                await InstallRecursiveDependenciesAsync(
                    stagingVersionFolder,
                    packagesDirectory,
                    stagingDirectory);

                // Ensure the package root folder exists
                if (!LifetimeManager.Get<IFileSystem>().Exists(packageRootFolder))
                {
                    // Create the folder
                    LifetimeManager.Get<IFileSystem>().CreateDirectory2(packageRootFolder);
                }

                // Move the extracted contents into the version folder
                LifetimeManager.Get<IFileSystem>().Rename(stagingVersionFolder, packageVersionFolder);
            }
        }

        /// <summary>
        /// Recursively install all dependencies and transitive dependencies
        /// </summary>
        static async Task InstallRecursiveDependenciesAsync(
            Path recipeDirectory,
            Path packagesDirectory,
            Path stagingDirectory)
        {
            var recipePath =
                recipeDirectory +
                BuildConstants.RecipeFileName;
            var (isSuccess, recipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipePath);
            if (!isSuccess)
            {
                throw new InvalidOperationException("Could not load the recipe file.");
            }

            var knownDependecyTypes = new List<string>()
            {
                "Runtime",
                "Test",
                "Build",
            };

            foreach (var dependecyType in knownDependecyTypes)
            {
                if (recipe.HasNamedDependencies(dependecyType))
                {
                    foreach (var dependency in recipe.GetNamedDependencies(dependecyType))
                    {
                        // If local then check children for external package references
                        // Otherwise install the external package reference and its dependencies
                        if (dependency.IsLocal)
                        {
                            var dependencyPath = recipeDirectory + dependency.Path;
                            await InstallRecursiveDependenciesAsync(
                                dependencyPath,
                                packagesDirectory,
                                stagingDirectory);
                        }
                        else
                        {
                            await EnsurePackageDownloadedAsync(
                                dependency.GetName,
                                dependency.Version,
                                packagesDirectory,
                                stagingDirectory);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Build the kitchen library path
        /// </summary>
        // static string BuildKitchenLibraryPath()
        // {
        // 	return Path(Constants.StoreLibraryFolderName);
        // }

        // /// <summary>
        // /// Build the kitchen build path
        // /// </summary>
        // static string BuildKitchenBuildPath(Recipe recipe)
        // {
        // 	return BuildKitchenBuildPath(recipe.Name, recipe.Version);
        // }

        // /// <summary>
        // /// Build the kitchen build path
        // /// </summary>
        // static string BuildKitchenBuildPath(PackageReference reference)
        // {
        // 	return BuildKitchenBuildPath(reference.Name, reference.Version);
        // }

        // /// <summary>
        // /// Build the kitchen package path
        // /// </summary>
        // static string BuildKitchenPackagePath(Recipe recipe)
        // {
        // 	return BuildKitchenPackagePath(recipe.Name, recipe.Version);
        // }

        // /// <summary>
        // /// Build the kitchen package path
        // /// </summary>
        // static string BuildKitchenPackagePath(PackageReference reference)
        // {
        // 	return BuildKitchenPackagePath(reference.Name, reference.Version);
        // }

        // /// <summary>
        // /// Build the kitchen package path
        // /// </summary>
        // static string BuildKitchenIncludePath(Recipe recipe)
        // {
        // 	return BuildKitchenIncludePath(recipe.Name, recipe.Version);
        // }

        // /// <summary>
        // /// Build the kitchen include path
        // /// </summary>
        // static string BuildKitchenIncludePath(PackageReference reference)
        // {
        // 	return BuildKitchenIncludePath(reference.Name, reference.Version);
        // }

        /// <summary>
        /// Ensure the staging directory exists
        /// </summary>
        static Path EnsureStagingDirectoryExists(Path packageStore)
        {
            var stagingDirectory = packageStore + new Path(StagingFolderName);
            if (LifetimeManager.Get<IFileSystem>().Exists(stagingDirectory))
            {
                Log.Warning("The staging directory already exists from a previous failed run");
                LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingDirectory, true);
            }

            // Create the folder
            LifetimeManager.Get<IFileSystem>().CreateDirectory2(stagingDirectory);

            return stagingDirectory;
        }

        // /// <summary>
        // /// Verify the archive
        // /// </summary>
        // static async Task<Recipe> VerifyArchiveAsync(Stream stream)
        // {
        // 	using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress, true))
        // 	using (var archive = new TarInputStream(gzipStream))
        // 	{
        // 		Recipe recipe = null;
        // 		TarEntry entry = archive.GetNextEntry();
        // 		while (entry != null)
        // 		{
        // 			if (string.Compare(entry.Name, Constants.RecipeFileName, true) == 0)
        // 			{
        // 				recipe = await RecipeManager.LoadFromStreamAsync(archive);
        // 				break;
        // 			}

        // 			entry = archive.GetNextEntry();
        // 		}

        // 		return recipe;
        // 	}
        // }

        // private static string BuildPackageVersionDirectory(string projectName, SemanticVersion version)
        // {
        // 	return Path.Combine(projectName, $"{version}");
        // }

        // private static string BuildKitchenPackagePath(string projectName, SemanticVersion version)
        // {
        // 	var kitchenPath = config.PackageStore;
        // 	var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
        // 	var path = Path.Combine(kitchenPath, Constants.StorePackageFolderName, packageVersionDirectory);
        // 	return path;
        // }

        // private static string BuildKitchenBuildPath(string projectName, SemanticVersion version)
        // {
        // 	var kitchenPath = config.PackageStore;
        // 	var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
        // 	var path = Path.Combine(kitchenPath, Constants.StoreBuildFolderName, packageVersionDirectory);
        // 	return path;
        // }

        // private static string BuildKitchenIncludePath(string projectName, SemanticVersion version)
        // {
        // 	var kitchenPath = config.PackageStore;
        // 	var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
        // 	var path = Path.Combine(kitchenPath, Constants.StoreIncludeRootFolderName, packageVersionDirectory);
        // 	return path;
        // }
    }
}
