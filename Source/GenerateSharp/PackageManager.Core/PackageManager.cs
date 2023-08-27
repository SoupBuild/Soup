// <copyright file="PackageManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

namespace Soup.Build.PackageManager
{
	using System;
	using System.Linq;
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
		private const string StagingFolderName = ".staging/";

		private Uri _apiEndpoint;

		private HttpClient _httpClient;

		private IClosureManager _closureManager;

		public PackageManager(
			Uri apiEndpoint,
			HttpClient httpClient,
			IClosureManager closureManager)
		{
			_apiEndpoint = apiEndpoint;
			_httpClient = httpClient;
			_closureManager = closureManager;
		}

		/// <summary>
		/// Restore packages
		/// </summary>
		public async Task RestorePackagesAsync(Path workingDirectory, bool forceRestore)
		{
			var userProfileDirectory = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory();
			var packageStore = userProfileDirectory + new Path(".soup/packages/");
			var lockStore = userProfileDirectory + new Path(".soup/locks/");

			Log.Diag("Using Package Store: " + packageStore.ToString());
			Log.Diag("Using Lock Store: " + lockStore.ToString());

			// Create the staging directory
			var stagingPath = EnsureStagingDirectoryExists(packageStore);

			try
			{
				await _closureManager.GenerateAndRestoreRecursiveLocksAsync(
					workingDirectory,
					packageStore,
					lockStore,
					stagingPath);

				// Cleanup the working directory
				Log.Diag("Deleting staging directory");
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
		public async Task InstallPackageReferenceAsync(Path workingDirectory, string packageReference)
		{
			var recipePath =
				workingDirectory +
				BuildConstants.RecipeFileName;
			var (isSuccess, recipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipePath);
			if (!isSuccess)
			{
				throw new InvalidOperationException($"Could not load the recipe file: {recipePath}");
			}

			var userProfileDirectory = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory();
			var packageStore = userProfileDirectory + new Path(".soup/packages/");
			var lockStore = userProfileDirectory + new Path(".soup/locks/");
			Log.Diag("Using Package Store: " + packageStore.ToString());
			Log.Diag("Using Lock Store: " + lockStore.ToString());

			// Parse the package reference to get the name
			var targetPackageReference = PackageReference.Parse(packageReference);
			string packageName = packageReference;
			if (!targetPackageReference.IsLocal)
			{
				packageName = targetPackageReference.Name;
			}

			// Check if the package is already installed
			var packageNameNormalized = packageName.ToUpperInvariant();
			if (recipe.HasRuntimeDependencies)
			{
				foreach (var dependency in recipe.RuntimeDependencies)
				{
					if (!dependency.IsLocal)
					{
						var dependencyNameNormalized = dependency.Name.ToUpperInvariant();
						if (dependencyNameNormalized == packageNameNormalized)
						{
							Log.Warning("Package already installed.");
							return;
						}
					}
				}
			}

			// Get the latest version if no version provided
			if (targetPackageReference.Version == null)
			{
				var packageModel = await GetPackageModelAsync(recipe.Language.Name, packageName);
				var latestVersion = new SemanticVersion(packageModel.Latest.Major, packageModel.Latest.Minor, packageModel.Latest.Patch);
				Log.HighPriority("Latest Version: " + latestVersion.ToString());
				targetPackageReference = new PackageReference(null, packageModel.Name, latestVersion);
			}

			if (targetPackageReference.Version == null)
				throw new InvalidOperationException("Target package version was null");

			// Register the package in the recipe
			Log.Info("Adding reference to recipe");
			recipe.AddRuntimeDependency(targetPackageReference.ToString());

			// Save the state of the recipe
			await RecipeExtensions.SaveToFileAsync(recipePath, recipe);

			// Create the staging directory
			var stagingDirectory = EnsureStagingDirectoryExists(packageStore);

			try
			{
				await _closureManager.GenerateAndRestoreRecursiveLocksAsync(
					workingDirectory,
					packageStore,
					lockStore,
					stagingDirectory);

				// Cleanup the working directory
				Log.Info("Deleting staging directory");
				LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingDirectory, true);
			}
			catch (Exception)
			{
				// Cleanup the staging directory and accept that we failed
				LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingDirectory, true);
				throw;
			}
		}

		/// <summary>
		/// Publish a package
		/// </summary>
		public async Task PublishPackageAsync(Path workingDirectory)
		{
			Log.Info($"Publish Project: {workingDirectory}");

			var recipePath =
				workingDirectory +
				BuildConstants.RecipeFileName;
			var (isSuccess, recipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipePath);
			if (!isSuccess)
			{
				throw new InvalidOperationException($"Could not load the recipe file: {recipePath}");
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
				using (var zipArchive = LifetimeManager.Get<IZipManager>().OpenCreate(archivePath))
				{
					AddPackageFiles(workingDirectory, zipArchive);
				}

				// Authenticate the user
				Log.Info("Request Authentication Token");
				var accessToken = await LifetimeManager.Get<IAuthenticationManager>().EnsureSignInAsync();

				// Publish the archive
				Log.Info("Publish package");
				var packageClient = new Api.Client.PackagesClient(_httpClient, accessToken)
				{
					BaseUrl = _apiEndpoint.ToString(),
				};

				// Check if the package exists
				bool packageExists = false;
				try
				{
					var package = await packageClient.GetPackageAsync(recipe.Language.Name, recipe.Name);
					packageExists = true;
				}
				catch (Api.Client.ApiException ex)
				{
					if (ex.StatusCode == 404)
					{
						Log.Info("Package does not exist");
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
					Log.Info("Creating package");
					var createPackageModel = new Api.Client.PackageCreateOrUpdateModel()
					{
						Description = string.Empty,
					};
					await packageClient.CreateOrUpdatePackageAsync(recipe.Language.Name, recipe.Name, createPackageModel);
				}

				var packageVersionClient = new Api.Client.PackageVersionsClient(_httpClient, accessToken)
				{
					BaseUrl = _apiEndpoint.ToString(),
				};

				using (var readArchiveFile = LifetimeManager.Get<IFileSystem>().OpenRead(archivePath))
				{
					try
					{
						await packageVersionClient.PublishPackageVersionAsync(
							recipe.Language.Name,
							recipe.Name,
							recipe.Version.ToString(),
							new Api.Client.FileParameter(readArchiveFile.GetInStream(), string.Empty, "application/zip"));

						Log.Info("Package published");
					}
					catch (Api.Client.ApiException ex)
					{
						switch (ex.StatusCode)
						{
							case 400:
								if (ex is Api.Client.ApiException<Api.Client.ProblemDetails> problemDetailsEx)
									Log.Error(problemDetailsEx.Result.Detail ?? "Bad request");
								else
									Log.Error("Bad request");
								break;
							case 403:
								Log.Error("You do not have permission to edit this package");
								break;
							case 409:
								Log.Info("Package version already exists");
								break;
							default:
								throw;
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

		private async Task<Api.Client.PackageModel> GetPackageModelAsync(string languageName, string packageName)
		{
			var client = new Api.Client.PackagesClient(_httpClient, null)
			{
				BaseUrl = _apiEndpoint.ToString(),
			};

			return await client.GetPackageAsync(languageName, packageName);
		}

		private static void AddPackageFiles(Path workingDirectory, IZipArchive archive)
		{
			var ignoreFileList = new string[]
			{
				"PackageLock.sml",
			};
			var ignoreFolderList = new string[]
			{
				"out",
				".git",
			};
			foreach (var child in LifetimeManager.Get<IFileSystem>().GetChildren(workingDirectory))
			{
				if (child.IsDirectory)
				{
					// Ignore undesirables
					if (!ignoreFolderList.Contains(child.Path.GetFileName()))
					{
						AddAllFilesRecursive(child.Path, workingDirectory, archive);
					}
				}
				else
				{
					// Ignore undesirables
					if (!ignoreFileList.Contains(child.Path.GetFileName()))
					{
						var relativePath = child.Path.GetRelativeTo(workingDirectory);
						var relativeName = relativePath.ToString().Substring(2);
						archive.CreateEntryFromFile(child.Path, relativeName);
					}
				}
			}
		}

		private static void AddAllFilesRecursive(Path directory, Path workingDirectory, IZipArchive archive)
		{
			foreach (var child in LifetimeManager.Get<IFileSystem>().GetChildren(directory))
			{
				if (child.IsDirectory)
				{
					AddAllFilesRecursive(child.Path, workingDirectory, archive);
				}
				else
				{
					var relativePath = child.Path.GetRelativeTo(workingDirectory);
					var relativeName = relativePath.ToString().Substring(2);
					archive.CreateEntryFromFile(child.Path, relativeName);
				}
			}
		}

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
	}
}
