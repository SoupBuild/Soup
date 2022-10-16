// <copyright file="PackageManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

namespace Soup.Build.PackageManager
{
	using System;
	using System.Collections.Generic;
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
		private static string StagingFolderName => ".staging/";

		//// private static string SoupApiEndpoint => "https://localhost:7071";
		private static string SoupApiEndpoint => "https://api.soupbuild.com";

		private HttpClient _httpClient;

		public PackageManager(HttpClient httpClient)
		{
			_httpClient = httpClient;
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
				// Place the lock in the local directory
				var packageLockPath =
					workingDirectory +
					BuildConstants.PackageLockFileName;

				await CheckRestorePackageClosureAsync(
					workingDirectory,
					packageLockPath,
					forceRestore,
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
				throw new InvalidOperationException("Could not load the recipe file.");
			}

			var userProfileDirectory = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory();
			var packageStore = userProfileDirectory + new Path(".soup/packages/");
			var lockStore = userProfileDirectory + new Path(".soup/locks/");
			Log.Diag("Using Package Store: " + packageStore.ToString());
			Log.Diag("Using Lock Store: " + lockStore.ToString());

			// Create the staging directory
			var stagingDirectory = EnsureStagingDirectoryExists(packageStore);

			try
			{
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

				var closure = new Dictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>>();
				var buildClosures = new Dictionary<string, IDictionary<string, IDictionary<string, PackageReference>>>();
				await CheckRecursiveEnsurePackageDownloadedAsync(
					recipe.Language.Name,
					targetPackageReference.Name,
					targetPackageReference.Version,
					false,
					packageStore,
					lockStore,
					stagingDirectory,
					closure,
					buildClosures);

				// Cleanup the working directory
				Log.Info("Deleting staging directory");
				LifetimeManager.Get<IFileSystem>().DeleteDirectory(stagingDirectory, true);

				// Register the package in the recipe
				Log.Info("Adding reference to recipe");
				recipe.AddRuntimeDependency(targetPackageReference.ToString());

				// Save the state of the recipe
				await RecipeExtensions.SaveToFileAsync(recipePath, recipe);
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
				using (var zipArchive = LifetimeManager.Get<IZipManager>().OpenCreate(archivePath))
				{
					AddPackageFiles(workingDirectory, zipArchive);
				}

				// Authenticate the user
				Log.Info("Request Authentication Token");
				var accessToken = await LifetimeManager.Get<IAuthenticationManager>().EnsureSignInAsync();

				// Publish the archive
				Log.Info("Publish package");
				var packageClient = new Api.Client.PackageClient(_httpClient)
				{
					BaseUrl = SoupApiEndpoint,
					BearerToken = accessToken,
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

				var packageVersionClient = new Api.Client.PackageVersionClient(_httpClient)
				{
					BaseUrl = SoupApiEndpoint,
					BearerToken = accessToken,
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
						if (ex.StatusCode == 409)
						{
							Log.Info("Package version already exists");
						}
						else if (ex.StatusCode == 403)
						{
							Log.Error("You do not have permission to edit this package");
						}
						else
						{
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
			var client = new Api.Client.PackageClient(_httpClient)
			{
				BaseUrl = SoupApiEndpoint,
			};

			return await client.GetPackageAsync(languageName, packageName);
		}

		private static void AddPackageFiles(Path workingDirectory, IZipArchive archive)
		{
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
					var relativePath = child.Path.GetRelativeTo(workingDirectory);
					var relativeName = relativePath.ToString().Substring(2);
					archive.CreateEntryFromFile(child.Path, relativeName);
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
		/// Ensure a package version is downloaded
		/// </summary>
		private async Task CheckRecursiveEnsurePackageDownloadedAsync(
			string languageName,
			string packageName,
			SemanticVersion packageVersion,
			bool forceRestore,
			Path packageStore,
			Path lockStore,
			Path stagingDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			if (closure.ContainsKey(languageName) && closure[languageName].ContainsKey(packageName))
			{
				Log.Diag("Recipe already processed.");
			}
			else
			{
				await EnsurePackageDownloadedAsync(
					languageName,
					packageName,
					packageVersion,
					packageStore,
					stagingDirectory);

				var languageRootFolder = packageStore + new Path(languageName);
				var packageRootFolder = languageRootFolder + new Path(packageName);
				var packageVersionFolder = packageRootFolder + new Path(packageVersion.ToString()) + new Path("/");

				var recipePath =
					packageVersionFolder +
					BuildConstants.RecipeFileName;
				var (isSuccess, recipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipePath);
				if (!isSuccess)
				{
					throw new InvalidOperationException("Could not load the recipe file.");
				}

				// Create the unique build closure
				var buildClosure = await CreateBuildClosureAsync(recipe, packageVersionFolder);

				var buildClosureName = string.Empty;
				var match = buildClosures.FirstOrDefault(value => AreEqual(value.Value, buildClosure));
				if (match.Key != null)
				{
					buildClosureName = match.Key;
				}
				else
				{
					buildClosureName = $"Build{buildClosures.Count}";
					buildClosures.Add(buildClosureName, buildClosure);
				}

				// Add the new package to the closure
				if (!closure.ContainsKey(languageName))
					closure.Add(languageName, new Dictionary<string, (PackageReference Package, string BuildClosure)>());
				closure[languageName].Add(packageName, (new PackageReference(null, packageName, packageVersion), buildClosureName));

				// Install recursive dependencies
				await RestoreRecursiveDependenciesAsync(
					packageVersionFolder,
					recipe,
					forceRestore,
					packageStore,
					lockStore,
					stagingDirectory,
					closure,
					buildClosures);
			}
		}

		/// <summary>
		/// Ensure a package version is downloaded
		/// </summary>
		private async Task EnsurePackageDownloadedAsync(
			string languageName,
			string packageName,
			SemanticVersion packageVersion,
			Path packageStore,
			Path stagingDirectory)
		{
			Log.HighPriority($"Install Package: {languageName} {packageName}@{packageVersion}");

			var languageRootFolder = packageStore + new Path(languageName);
			var packageRootFolder = languageRootFolder + new Path(packageName);
			var packageVersionFolder = packageRootFolder + new Path(packageVersion.ToString()) + new Path("/");

			// Check if the package version already exists
			if (packageName == "C#" || packageName == "C++")
			{
				Log.HighPriority("Skip build logic for now");
			}
			else if (LifetimeManager.Get<IFileSystem>().Exists(packageVersionFolder))
			{
				Log.HighPriority("Found local version");
			}
			else
			{
				// Download the archive
				Log.HighPriority("Downloading package");
				var archivePath = stagingDirectory + new Path(packageName + ".zip");

				var client = new Api.Client.PackageVersionClient(_httpClient)
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

				// Create the package folder to extract to
				var stagingVersionFolder = stagingDirectory + new Path($"{languageName}_{packageName}_{packageVersion}/");
				LifetimeManager.Get<IFileSystem>().CreateDirectory2(stagingVersionFolder);

				// Unpack the contents of the archive
				LifetimeManager.Get<IZipManager>().ExtractToDirectory(archivePath, stagingVersionFolder);

				// Delete the archive file
				LifetimeManager.Get<IFileSystem>().DeleteFile(archivePath);

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
		/// Restore package lock
		/// </summary>
		private async Task RestorePackageLockAsync(
			Path packageStore,
			Path stagingDirectory,
			PackageLock packageLock)
		{
			foreach (var closure in packageLock.GetClosures().Values)
			{
				Log.Info($"Restore Packages for Closure {closure.Key}");
				foreach (var languageProjects in closure.Value.Value.AsTable().Values)
				{
					Log.Info($"Restore Packages for Language {languageProjects.Key}");
					foreach (var project in languageProjects.Value.Value.AsArray().Values)
					{
						var projectTable = project.Value.AsTable();
						var projectName = projectTable.Values[PackageLock.Property_Name].Value.AsString().Value;
						var projectVersion = projectTable.Values[PackageLock.Property_Version].Value.AsString().Value;
						if (SemanticVersion.TryParse(projectVersion, out var version))
						{
							await EnsurePackageDownloadedAsync(
								languageProjects.Key,
								projectName,
								version,
								packageStore,
								stagingDirectory);
						}
						else
						{
							Log.Info($"Skip Package: {projectName} -> {projectVersion}");
						}
					}
				}
			}
		}

		/// <summary>
		/// Restore packages
		/// </summary>
		public async Task CheckRestorePackageClosureAsync(
			Path workingDirectory,
			Path packageLockPath,
			bool forceRestore,
			Path packageStore,
			Path lockStore,
			Path stagingPath)
		{
			bool wasRestored = false;
			if (!forceRestore)
			{
				var loadPackageLock = await PackageLockExtensions.TryLoadFromFileAsync(packageLockPath);
				if (loadPackageLock.IsSuccess)
				{
					Log.Info("Restore from package lock");
					await RestorePackageLockAsync(
						packageStore,
						stagingPath,
						loadPackageLock.Result);
					wasRestored = true;
				}
			}

			if (!wasRestored)
			{
				Log.Info("Discovering full closure");
				var closure = new Dictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>>();
				var buildClosures = new Dictionary<string, IDictionary<string, IDictionary<string, PackageReference>>>();
				await DiscoverRecursiveDependenciesAsync(
					workingDirectory,
					forceRestore,
					packageStore,
					lockStore,
					stagingPath,
					closure,
					buildClosures);

				// Build up the package lock file
				var packageLock = new PackageLock();
				packageLock.SetVersion(3);
				var rootClosureName = "Root";
				foreach (var languageClosure in closure.OrderBy(value => value.Key))
				{
					foreach (var (key, (package, buildClosure)) in languageClosure.Value.OrderBy(value => value.Key))
					{
						var value = string.Empty;
						if (package.IsLocal)
						{
							value = package.Path.GetRelativeTo(workingDirectory).ToString();
						}
						else
						{
							if (package.Version == null)
								throw new InvalidOperationException("Package lock closure must have version");
							value = package.Version.ToString();
						}

						Log.Diag($"{rootClosureName}:{languageClosure.Key} {key} -> {value}");
						packageLock.AddProject(
							rootClosureName,
							languageClosure.Key,
							key,
							value,
							buildClosure);
					}
				}

				foreach (var buildClosure in buildClosures.OrderBy(value => value.Key))
				{
					packageLock.EnsureClosure(buildClosure.Key);
					foreach (var languageClosure in buildClosure.Value.OrderBy(value => value.Key))
					{
						foreach (var (key, package) in languageClosure.Value)
						{
							var value = string.Empty;
							if (package.IsLocal)
							{
								value = package.Path.GetRelativeTo(workingDirectory).ToString();
							}
							else
							{
								if (package.Version == null)
									throw new InvalidOperationException("Package lock closure must have version");
								value = package.Version.ToString();
							}

							Log.Diag($"{buildClosure.Key}:{languageClosure.Key} {key} -> {value}");
							packageLock.AddProject(
								buildClosure.Key,
								languageClosure.Key,
								key,
								value,
								null);
						}
					}
				}

				// Save the updated package lock
				await PackageLockExtensions.SaveToFileAsync(packageLockPath, packageLock);
			}
		}

		/// <summary>
		/// Recursively restore all dependencies
		/// </summary>
		private async Task DiscoverRecursiveDependenciesAsync(
			Path recipeDirectory,
			bool forceRestore,
			Path packageStore,
			Path lockStore,
			Path stagingDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			var recipePath =
				recipeDirectory +
				BuildConstants.RecipeFileName;
			var (isSuccess, recipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipePath);
			if (!isSuccess)
			{
				throw new InvalidOperationException("Could not load the recipe file.");
			}

			if (closure.ContainsKey(recipe.Language.Name) && closure[recipe.Language.Name].ContainsKey(recipe.Name))
			{
				Log.Diag("Recipe already processed.");
			}
			else
			{
				// Create the unique build closure
				var buildClosure = await CreateBuildClosureAsync(recipe, recipeDirectory);

				var buildClosureName = string.Empty;
				var match = buildClosures.FirstOrDefault(value => AreEqual(value.Value, buildClosure));
				if (match.Key != null)
				{
					buildClosureName = match.Key;
				}
				else
				{
					buildClosureName = $"Build{buildClosures.Count}";
					buildClosures.Add(buildClosureName, buildClosure);
				}

				// Add the project to the closure
				if (!closure.ContainsKey(recipe.Language.Name))
					closure.Add(recipe.Language.Name, new Dictionary<string, (PackageReference Package, string BuildClosure)>());
				closure[recipe.Language.Name].Add(recipe.Name, (new PackageReference(recipeDirectory), buildClosureName));

				await RestoreRecursiveDependenciesAsync(
					recipeDirectory,
					recipe,
					forceRestore,
					packageStore,
					lockStore,
					stagingDirectory,
					closure,
					buildClosures);
			}
		}

		private static async Task<IDictionary<string, IDictionary<string, PackageReference>>> CreateBuildClosureAsync(
			Recipe recipe,
			Path recipeDirectory)
		{
			var buildClosure = new Dictionary<string, IDictionary<string, PackageReference>>();
			var implicitLanguage = "C#";

			// Add the language build extension
			buildClosure.Add(implicitLanguage, new Dictionary<string, PackageReference>());
			buildClosure[implicitLanguage].Add(
				recipe.Language.Name,
				FillDefaultVersion(new PackageReference(implicitLanguage, recipe.Language.Name, recipe.Language.Version)));

			// Discover any dependency build references
			if (recipe.HasBuildDependencies)
			{
				foreach (var dependency in recipe.BuildDependencies)
				{
					var dependencyPackage = dependency;
					if (dependency.IsLocal)
					{
						var dependencyPath = recipeDirectory + dependency.Path;
						var dependencyRecipePath =
							dependencyPath +
							BuildConstants.RecipeFileName;
						var (isDependencySuccess, dependencyRecipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(dependencyRecipePath);
						if (!isDependencySuccess)
						{
							throw new InvalidOperationException("Could not load dependency recipe file.");
						}

						dependencyPackage = new PackageReference(
							dependencyRecipe.Language.Name,
							dependencyRecipe.Name,
							dependencyRecipe.Version);
					}

					var language = dependencyPackage.Language != null ? dependencyPackage.Language : implicitLanguage;

					if (!buildClosure.ContainsKey(language))
						buildClosure.Add(language, new Dictionary<string, PackageReference>());

					buildClosure[language].Add(dependencyPackage.Name, FillDefaultVersion(dependencyPackage));
				}
			}

			return buildClosure;
		}

		private static PackageReference FillDefaultVersion(PackageReference package)
		{
			if (package.Version == null)
				throw new ArgumentException("Package version was null");

			// TODO: Discover the latest available version
			// For now auto assume missing values are zero
			if (package.Version.Minor is null)
			{
				return new PackageReference(
					package.Language,
					package.Name,
					new SemanticVersion(package.Version.Major, 0, 0));
			}
			else if (package.Version.Patch is null)
			{
				return new PackageReference(
					package.Language,
					package.Name,
					new SemanticVersion(package.Version.Major, package.Version.Minor, 0));
			}
			else
			{
				return package;
			}
		}

		/// <summary>
		/// Recursively restore all dependencies, assume that the closure has been updated correctly for current recipe
		/// </summary>
		private async Task RestoreRecursiveDependenciesAsync(
			Path recipeDirectory,
			Recipe recipe,
			bool forceRestore,
			Path packageStore,
			Path lockStore,
			Path stagingDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			foreach (var dependencyType in recipe.GetDependencyTypes())
			{
				// Build dependencies do not inherit the parent language
				// Instead, they default to C#
				bool isBuildDependency = dependencyType == Recipe.Property_Build;
				if (isBuildDependency)
				{
					await RestoreBuildDependenciesAsync(
						recipeDirectory,
						recipe,
						forceRestore,
						packageStore,
						lockStore,
						stagingDirectory,
						closure,
						buildClosures);
				}
				else
				{
					await RestoreRuntimeDependenciesAsync(
						recipeDirectory,
						recipe,
						dependencyType,
						forceRestore,
						packageStore,
						lockStore,
						stagingDirectory,
						closure,
						buildClosures);
				}
			}
		}

		/// <summary>
		/// Recursively restore all runtime dependencies
		/// </summary>
		private async Task RestoreRuntimeDependenciesAsync(
			Path recipeDirectory,
			Recipe recipe,
			string dependencyType,
			bool forceRestore,
			Path packageStore,
			Path lockStore,
			Path stagingDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			// Same language as parent is implied
			var implicitLanguage = recipe.Language.Name;

			foreach (var dependency in recipe.GetNamedDependencies(dependencyType))
			{
				// If local then check children for external package references
				// Otherwise install the external package reference and its dependencies
				if (dependency.IsLocal)
				{
					var dependencyPath = recipeDirectory + dependency.Path;
					await DiscoverRecursiveDependenciesAsync(
						dependencyPath,
						forceRestore,
						packageStore,
						lockStore,
						stagingDirectory,
						closure,
						buildClosures);
				}
				else
				{
					if (dependency.Version == null)
						throw new ArgumentException("Local package version was null");

					var language = dependency.Language != null ? dependency.Language : implicitLanguage;
					await CheckRecursiveEnsurePackageDownloadedAsync(
						language,
						dependency.Name,
						dependency.Version,
						forceRestore,
						packageStore,
						lockStore,
						stagingDirectory,
						closure,
						buildClosures);
				}
			}
		}

		/// <summary>
		/// Recursively restore all build dependencies
		/// </summary>
		private async Task RestoreBuildDependenciesAsync(
			Path recipeDirectory,
			Recipe recipe,
			bool forceRestore,
			Path packageStore,
			Path lockStore,
			Path stagingDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			// Build dependencies do not inherit the parent language
			// Instead, they default to C#
			var implicitLanguage = "C#";

			foreach (var dependency in recipe.GetNamedDependencies(Recipe.Property_Build))
			{
				// If local then check children for external package references
				// Otherwise install the external package reference and its dependencies
				if (dependency.IsLocal)
				{
					var dependencyPath = recipeDirectory + dependency.Path;

					// Place the lock in the local directory
					var packageLockPath =
						dependencyPath +
						BuildConstants.PackageLockFileName;

					await CheckRestorePackageClosureAsync(
						dependencyPath,
						packageLockPath,
						forceRestore,
						packageStore,
						lockStore,
						stagingDirectory);
				}
				else
				{
					if (dependency.Version == null)
						throw new ArgumentException("Local package version was null");

					var language = dependency.Language != null ? dependency.Language : implicitLanguage;

					await EnsurePackageDownloadedAsync(
						language,
						dependency.Name,
						dependency.Version,
						packageStore,
						stagingDirectory);

					var packageLanguageNameVersionPath = new Path(language) +
						new Path(dependency.Name) +
						new Path(dependency.Version.ToString()) + new Path("/");
					var packageContentDirectory = packageStore + packageLanguageNameVersionPath;

					// Place the lock in the lock store
					var packageLockPath =
						lockStore +
						packageLanguageNameVersionPath +
						BuildConstants.PackageLockFileName;

					await CheckRestorePackageClosureAsync(
						packageContentDirectory,
						packageLockPath,
						forceRestore,
						packageStore,
						lockStore,
						stagingDirectory);
				}
			}
		}

		private static bool AreEqual(
			IDictionary<string, IDictionary<string, PackageReference>> lhs,
			IDictionary<string, IDictionary<string, PackageReference>> rhs)
		{
			return lhs.Keys.Count == rhs.Keys.Count &&
				lhs.Keys.All(value => rhs.Keys.Contains(value)) &&
				lhs.All(value => AreEqual(value.Value, rhs[value.Key]));
		}

		private static bool AreEqual(
			IDictionary<string, PackageReference> lhs,
			IDictionary<string, PackageReference> rhs)
		{

			return lhs.Keys.Count == rhs.Keys.Count &&
				lhs.Keys.All(value => rhs.Keys.Contains(value)) &&
				lhs.All(value => value.Value == rhs[value.Key]);
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
