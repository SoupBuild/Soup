// <copyright file="PackageManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

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

		//// private static string SoupApiEndpoint => "https://localhost:7071";
		private static string SoupApiEndpoint => "https://api.soupbuild.com";

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
					var packageModel = await GetPackageModelAsync(recipe.Language, packageName);
					var latestVersion = new SemanticVersion(packageModel.Latest.Major, packageModel.Latest.Minor, packageModel.Latest.Patch);
					Log.HighPriority("Latest Version: " + latestVersion.ToString());
					targetPackageReference = new PackageReference(packageModel.Name, latestVersion);
				}

				await EnsurePackageDownloadedAsync(
					recipe.Language,
					targetPackageReference.GetName,
					targetPackageReference.Version,
					packageStore,
					stagingPath);

				// Cleanup the working directory
				Log.Info("Deleting staging directory");
				LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);

				// Register the package in the recipe
				Log.Info("Adding reference to recipe");
				recipe.AddRuntimeDependency(targetPackageReference.ToString());

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

		/// <summary>
		/// Publish a package
		/// </summary>
		public static async Task PublishPackageAsync(Path workingDirectory)
		{
			Log.Info($"Publish Project: {workingDirectory}");

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
				var archivePath = stagingPath + new Path(recipe.Name + ".zip");

				// Create the archive of the package
				using (var writeArchiveFile = LifetimeManager.Get<IFileSystem>().OpenWrite(archivePath, true))
				using (var zipArchive = new ZipArchive(writeArchiveFile.GetOutStream(), ZipArchiveMode.Create, false))
				{
					AddPackageFiles(workingDirectory, zipArchive);
				}

				// Authenticate the user
				Log.Info("Request Authentication Token");
				var accessToken = await AuthenticationManager.EnsureSignInAsync();

				// Publish the archive
				Log.Info("Publish package");
				using (var httpClient = new HttpClient())
				{
					var packageClient = new Api.Client.PackageClient(httpClient)
					{
						BaseUrl = SoupApiEndpoint,
						BearerToken = accessToken,
					};

					// Check if the package exists
					bool packageExists = false;
					try
					{
						var package = await packageClient.GetPackageAsync(recipe.Language, recipe.Name);
						packageExists = true;
					}
					catch (Api.Client.ApiException ex)
					{
						if (ex.StatusCode == 404)
						{
							packageExists = false;
						}
						else
						{
							throw;
						}
					}

					// Create the package if it does not exist
					if (!packageExists)
					{
						var createPackageModel = new Api.Client.PackageCreateOrUpdateModel()
						{
							Description = string.Empty,
						};
						await packageClient.CreateOrUpdatePackageAsync(recipe.Language, recipe.Name, createPackageModel);
					}

					var packageVersionClient = new Api.Client.PackageVersionClient(httpClient)
					{
						BaseUrl = SoupApiEndpoint,
						BearerToken = accessToken,
					};

					using (var readArchiveFile = LifetimeManager.Get<IFileSystem>().OpenRead(archivePath, true))
					{
						try
						{
							await packageVersionClient.PublishPackageVersionAsync(
								recipe.Language,
								recipe.Name,
								recipe.Version.ToString(),
								new Api.Client.FileParameter(readArchiveFile.GetInStream(), string.Empty, "application/zip"));

							Log.Info("Package published");
						}
						catch (Api.Client.ApiException ex)
						{
							if (ex.StatusCode == 409)
							{
								Log.Info("Package version already exists");
							}
							else
							{
								throw;
							}
						}
					}
				}

				// Cleanup the staging directory
				Log.Info("Cleanup staging directory");
				LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);
			}
			catch (Exception)
			{
				// Cleanup the staging directory and accept that we failed
				Log.Info("Publish Failed: Cleanup staging directory");
				LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingPath, true);
				throw;
			}
		}

		private static async Task<Api.Client.PackageModel> GetPackageModelAsync(string languageName, string packageName)
		{
			using (var httpClient = new HttpClient())
			{
				var client = new Api.Client.PackageClient(httpClient)
				{
					BaseUrl = SoupApiEndpoint,
				};
				return await client.GetPackageAsync(languageName, packageName);
			}
		}

		private static void AddPackageFiles(Path workingDirectory, ZipArchive archive)
		{
			foreach (var child in LifetimeManager.Get<IFileSystem>().GetDirectoryChildren(workingDirectory))
			{
				if (child.IsDirectory)
				{
					// Ignore output folder
					if (child.Path.GetFileName() != "out")
					{
						AddAllFilesRecursive(child.Path, workingDirectory, archive);
					}
				}
				else
				{
					var relativePath = child.Path.GetRelativeTo(workingDirectory);
					var relativeName = relativePath.ToString().Substring(2);
					var fileEentry = archive.CreateEntryFromFile(child.Path.ToString(), relativeName);
				}
			}
		}

		private static void AddAllFilesRecursive(Path directory, Path workingDirectory, ZipArchive archive)
		{
			foreach (var child in LifetimeManager.Get<IFileSystem>().GetDirectoryChildren(directory))
			{
				if (child.IsDirectory)
				{
					AddAllFilesRecursive(child.Path, workingDirectory, archive);
				}
				else
				{
					var relativePath = child.Path.GetRelativeTo(workingDirectory);
					var relativeName = relativePath.ToString().Substring(2);
					var fileEentry = archive.CreateEntryFromFile(child.Path.ToString(), relativeName);
				}
			}
		}

		/// <summary>
		/// Ensure a package version is downloaded
		/// </summary>
		private static async Task EnsurePackageDownloadedAsync(
			string languageName,
			string packageName,
			 SemanticVersion packageVersion,
			 Path packagesDirectory,
			 Path stagingDirectory)
		{
			Log.HighPriority($"Install Package: {languageName} {packageName}@{packageVersion}");

			var languageRootFolder = packagesDirectory + new Path(languageName);
			var packageRootFolder = languageRootFolder + new Path(packageName);
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
					var client = new Api.Client.PackageVersionClient(httpClient)
					{
						BaseUrl = SoupApiEndpoint,
					};

					try
					{
						var result = await client.DownloadPackageVersionAsync(languageName, packageName, packageVersion.ToString());

						// Write the contents to disk, scope cleanup
						using (var archiveWriteFile = LifetimeManager.Get<IFileSystem>().OpenWrite(archivePath, true))
						{
							await result.Stream.CopyToAsync(archiveWriteFile.GetOutStream());
						}
					}
					catch (Api.Client.ApiException ex)
					{
						if (ex.StatusCode == 404)
						{
							Log.HighPriority("Package Version Missing");
							throw new HandledException();
						}
						else
						{
							throw;
						}
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
					// Same language as parent is implied
					var implicitLanguage = recipe.Language;
					if (dependecyType == "Build")
					{
						// Build dependencies do not inherit the parent language
						// Instead, they default to C#
						implicitLanguage = "C#";
					}

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
								implicitLanguage,
								dependency.GetName,
								dependency.Version,
								packagesDirectory,
								stagingDirectory);
						}
					}
				}
			}
		}

		///// <summary>
		///// Build the kitchen library path
		///// </summary>
		//// static string BuildKitchenLibraryPath()
		//// {
		//// 	return Path(Constants.StoreLibraryFolderName);
		//// }

		//// /// <summary>
		//// /// Build the kitchen build path
		//// /// </summary>
		//// static string BuildKitchenBuildPath(Recipe recipe)
		//// {
		//// 	return BuildKitchenBuildPath(recipe.Name, recipe.Version);
		//// }

		//// /// <summary>
		//// /// Build the kitchen build path
		//// /// </summary>
		//// static string BuildKitchenBuildPath(PackageReference reference)
		//// {
		//// 	return BuildKitchenBuildPath(reference.Name, reference.Version);
		//// }

		//// /// <summary>
		//// /// Build the kitchen package path
		//// /// </summary>
		//// static string BuildKitchenPackagePath(Recipe recipe)
		//// {
		//// 	return BuildKitchenPackagePath(recipe.Name, recipe.Version);
		//// }

		//// /// <summary>
		//// /// Build the kitchen package path
		//// /// </summary>
		//// static string BuildKitchenPackagePath(PackageReference reference)
		//// {
		//// 	return BuildKitchenPackagePath(reference.Name, reference.Version);
		//// }

		//// /// <summary>
		//// /// Build the kitchen package path
		//// /// </summary>
		//// static string BuildKitchenIncludePath(Recipe recipe)
		//// {
		//// 	return BuildKitchenIncludePath(recipe.Name, recipe.Version);
		//// }

		//// /// <summary>
		//// /// Build the kitchen include path
		//// /// </summary>
		//// static string BuildKitchenIncludePath(PackageReference reference)
		//// {
		//// 	return BuildKitchenIncludePath(reference.Name, reference.Version);
		//// }

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
