// <copyright file="PackageManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

namespace Soup.Build.PackageManager
{
	using System;
	using System.Collections.Generic;
	using System.IO.Compression;
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

		/// <summary>
		/// Restore packages
		/// </summary>
		public static async Task RestorePackagesAsync(Path workingDirectory, bool forceRestore)
		{
			var packageStore = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
				new Path(".soup/packages/");
			Log.Diag("Using Package Store: " + packageStore.ToString());

			// Create the staging directory
			var stagingPath = EnsureStagingDirectoryExists(packageStore);

			try
			{
				var packageLockPath =
					workingDirectory +
					BuildConstants.PackageLockFileName;
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
					await CheckRestoreRecursiveDependenciesAsync(
						workingDirectory,
						packageStore,
						stagingPath,
						closure,
						buildClosures);

					// Build up the package lock file
					var packageLock = new PackageLock();
					packageLock.SetVersion(2);
					var rootClosureName = "Root";
					foreach (var languageClosure in closure)
					{
						foreach (var (key, (package, buildClosure)) in languageClosure.Value)
						{
							var value = string.Empty;
							if (package.IsLocal)
							{
								value = package.Path.GetRelativeTo(workingDirectory).ToString();
							}
							else
							{
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

					foreach (var buildClosure in buildClosures)
					{
						packageLock.EnsureClosure(buildClosure.Key);
						foreach (var languageClosure in buildClosure.Value)
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
				if (targetPackageReference.IsLocal)
				{
					var packageModel = await GetPackageModelAsync(recipe.Language.Name, packageName);
					var latestVersion = new SemanticVersion(packageModel.Latest.Major, packageModel.Latest.Minor, packageModel.Latest.Patch);
					Log.HighPriority("Latest Version: " + latestVersion.ToString());
					targetPackageReference = new PackageReference(null, packageModel.Name, latestVersion);
				}

				var closure = new Dictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>>();
				var buildClosures = new Dictionary<string, IDictionary<string, IDictionary<string, PackageReference>>>();
				await CheckRecursiveEnsurePackageDownloadedAsync(
					recipe.Language.Name,
					targetPackageReference.Name,
					targetPackageReference.Version,
					packageStore,
					stagingPath,
					closure,
					buildClosures);

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

					var packageVersionClient = new Api.Client.PackageVersionClient(httpClient)
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
			var ignoreFolderList = new string[]
			{
				"out",
				".git",
			};
			foreach (var child in LifetimeManager.Get<IFileSystem>().GetDirectoryChildren(workingDirectory))
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
		private static async Task CheckRecursiveEnsurePackageDownloadedAsync(
			string languageName,
			string packageName,
			SemanticVersion packageVersion,
			Path packagesDirectory,
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
					var stagingVersionFolder = stagingDirectory + new Path($"{languageName}_{packageName}_{packageVersion}/");
					LifetimeManager.Get<IFileSystem>().CreateDirectory2(stagingVersionFolder);

					// Unpack the contents of the archive
					ZipFile.ExtractToDirectory(archivePath.ToString(), stagingVersionFolder.ToString());

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
					packagesDirectory,
					stagingDirectory,
					closure,
					buildClosures);
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
				var stagingVersionFolder = stagingDirectory + new Path($"{languageName}_{packageName}_{packageVersion}/");
				LifetimeManager.Get<IFileSystem>().CreateDirectory2(stagingVersionFolder);

				// Unpack the contents of the archive
				ZipFile.ExtractToDirectory(archivePath.ToString(), stagingVersionFolder.ToString());

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
		static async Task RestorePackageLockAsync(
			Path packagesDirectory,
			Path stagingDirectory,
			PackageLock packageLock)
		{
			foreach (var closure in packageLock.GetClosures())
			{
				Log.Info($"Restore Packages for Closure {closure.Key}");
				foreach (var languageProjects in closure.Value.AsTable())
				{
					Log.Info($"Restore Packages for Language {languageProjects.Key}");
					foreach (var project in languageProjects.Value.AsList())
					{
						var projectTable = project.AsTable();
						var projectName = projectTable["Name"].AsString();
						var projectVersion = projectTable["Version"].AsString();
						if (SemanticVersion.TryParse(projectVersion, out var version))
						{
							await EnsurePackageDownloadedAsync(
								languageProjects.Key,
								projectName,
								version,
								packagesDirectory,
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
		/// Recursively restore all dependencies
		/// </summary>
		static async Task CheckRestoreRecursiveDependenciesAsync(
			Path recipeDirectory,
			Path packagesDirectory,
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
					packagesDirectory,
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
			buildClosure[implicitLanguage].Add(recipe.Language.Name, new PackageReference(implicitLanguage, recipe.Language.Name, recipe.Language.Version));

			// Discover any dependency build references
			if (recipe.HasNamedDependencies("Build"))
			{
				foreach (var dependency in recipe.GetNamedDependencies("Build"))
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

						dependencyPackage = new PackageReference(dependencyRecipe.Language.Name, dependencyRecipe.Name, dependencyRecipe.Version);
					}

					var language = dependencyPackage.Language != null ? dependencyPackage.Language : implicitLanguage;

					if (!buildClosure.ContainsKey(language))
						buildClosure.Add(language, new Dictionary<string, PackageReference>());

					buildClosure[language].Add(dependencyPackage.Name, dependencyPackage);
				}
			}

			return buildClosure;
		}

		/// <summary>
		/// Recursively restore all dependencies, assume that the closure has been updated correctly for current recipe
		/// </summary>
		static async Task RestoreRecursiveDependenciesAsync(
			Path recipeDirectory,
			Recipe recipe,
			Path packagesDirectory,
			Path stagingDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			foreach (var dependecyType in recipe.GetDependencyTypes())
			{
				if (recipe.HasNamedDependencies(dependecyType))
				{
					// Same language as parent is implied
					var implicitLanguage = recipe.Language.Name;

					// Build dependencies do not inherit the parent language
					// Instead, they default to C#
					if (dependecyType == "Build")
					{
						implicitLanguage = "C#";
					}

					foreach (var dependency in recipe.GetNamedDependencies(dependecyType))
					{
						// If local then check children for external package references
						// Otherwise install the external package reference and its dependencies
						if (dependency.IsLocal)
						{
							var dependencyPath = recipeDirectory + dependency.Path;
							await CheckRestoreRecursiveDependenciesAsync(
								dependencyPath,
								packagesDirectory,
								stagingDirectory,
								closure,
								buildClosures);
						}
						else
						{
							var language = dependency.Language != null ? dependency.Language : implicitLanguage;
							await CheckRecursiveEnsurePackageDownloadedAsync(
								language,
								dependency.Name,
								dependency.Version,
								packagesDirectory,
								stagingDirectory,
								closure,
								buildClosures);
						}
					}
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
