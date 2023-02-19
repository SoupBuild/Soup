// <copyright file="ClosureManager.cs" company="Soup">
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
	/// The closure builder
	/// </summary>
	public class ClosureManager : IClosureManager
	{
		private const int PackageLockVersion = 4;
		private const string RootClosureName = "Root";
		private const string BuiltInLanguageCSharp = "C#";
		private const string BuiltInLanguageCpp = "C++";
		private const string BuiltInLanguageWren = "Wren";
		private const string BuiltInLanguagePackageCSharp = "Soup.CSharp";
		private const string BuiltInLanguagePackageCpp = "Soup.Cpp";
		private const string BuiltInLanguagePackageWren = "Soup.Wren";
		private const string BuiltInLanguageSafeNameCSharp = "CSharp";
		private const string BuiltInLanguageSafeNameCpp = "Cpp";
		private const string BuiltInLanguageSafeNameWren = "Wren";

		private Uri _apiEndpoint;

		private HttpClient _httpClient;

		private SemanticVersion _builtInLanguageVersionCSharp;
		private SemanticVersion _builtInLanguageVersionCpp;
		private SemanticVersion _builtInLanguageVersionWren;

		public ClosureManager(
			Uri apiEndpoint,
			HttpClient httpClient,
			SemanticVersion builtInLanguageVersionCSharp,
			SemanticVersion builtInLanguageVersionCpp,
			SemanticVersion builtInLanguageVersionWren)
		{
			_apiEndpoint = apiEndpoint;
			_httpClient = httpClient;
			_builtInLanguageVersionCSharp = builtInLanguageVersionCSharp;
			_builtInLanguageVersionCpp = builtInLanguageVersionCpp;
			_builtInLanguageVersionWren = builtInLanguageVersionWren;
		}

		/// <summary>
		/// Restore packages
		/// </summary>
		public async Task GenerateAndRestoreRecursiveLocksAsync(
			Path workingDirectory,
			Path packageStoreDirectory,
			Path packageLockStoreDirectory,
			Path stagingDirectory)
		{
			// Place the lock in the local directory
			var packageLockPath =
				workingDirectory +
				BuildConstants.PackageLockFileName;

			var processedLocks = new HashSet<Path>();
			await CheckGenerateAndRestoreRecursiveLocksAsync(
				workingDirectory,
				packageLockPath,
				packageStoreDirectory,
				packageLockStoreDirectory,
				stagingDirectory,
				processedLocks);
		}

		private async Task CheckGenerateAndRestoreRecursiveLocksAsync(
			Path workingDirectory,
			Path packageLockPath,
			Path packageStoreDirectory,
			Path packageLockStoreDirectory,
			Path stagingDirectory,
			HashSet<Path> processedLocks)
		{
			if (processedLocks.Contains(packageLockPath))
			{
				Log.Info("Root already processed");
			}
			else
			{
				processedLocks.Add(packageLockPath);

				var packageLock = await EnsurePackageLockAsync(
					workingDirectory,
					packageLockPath);
				await RestorePackageLockAsync(
					packageStoreDirectory,
					stagingDirectory,
					packageLock);

				await CheckGenerateAndRestoreSubGraphDependencyLocksAsync(
					workingDirectory,
					packageStoreDirectory,
					packageLockStoreDirectory,
					stagingDirectory,
					packageLock,
					processedLocks);
			}
		}

		private async Task CheckGenerateAndRestoreSubGraphDependencyLocksAsync(
			Path workingDirectory,
			Path packageStoreDirectory,
			Path packageLockStoreDirectory,
			Path stagingDirectory,
			PackageLock packageLock,
			HashSet<Path> processedLocks)
		{
			foreach (var closure in packageLock.GetClosures().Values)
			{
				// Skip the root closure and only generate locks for the build extensions
				if (closure.Key != RootClosureName)
				{
					foreach (var (languageName, languageProjects) in closure.Value.Value.AsTable().Values)
					{
						foreach (var project in languageProjects.Value.AsArray().Values)
						{
							var projectTable = project.Value.AsTable();
							var projectName = projectTable.Values[PackageLock.Property_Name].Value.AsString().Value;
							var projectVersion = projectTable.Values[PackageLock.Property_Version].Value.AsString().Value;
							if (SemanticVersion.TryParse(projectVersion, out var version))
							{
								// Check if the package version already exists
								if ((projectName == BuiltInLanguagePackageCpp && version == _builtInLanguageVersionCpp) ||
									(projectName == BuiltInLanguagePackageCSharp && version == _builtInLanguageVersionCSharp) ||
									(projectName == BuiltInLanguagePackageWren && version == _builtInLanguageVersionWren))
								{
									Log.HighPriority("Skip built in language version in build closure");
								}
								else
								{
									var languageSafeName = GetLanguageSafeName(languageName);
									var packageLanguageNameVersionPath =
										new Path(languageSafeName) +
										new Path(projectName) +
										new Path(version.ToString()) +
										new Path();
									var packageContentDirectory = packageStoreDirectory + packageLanguageNameVersionPath;

									// Place the lock in the lock store
									var packageLockDirectory =
										packageLockStoreDirectory +
										packageLanguageNameVersionPath;
									var packageLockPath =
										packageLockDirectory +
										BuildConstants.PackageLockFileName;

									EnsureDirectoryExists(packageLockDirectory);

									await CheckGenerateAndRestoreRecursiveLocksAsync(
										packageContentDirectory,
										packageLockPath,
										packageStoreDirectory,
										packageLockStoreDirectory,
										stagingDirectory,
										processedLocks);
								}
							}
							else
							{
								// Process the local dependency and place the lock in the root
								var referencePath = new Path(projectVersion);
								var dependencyPath = workingDirectory + referencePath;
								var dependencyLockPath =
									dependencyPath +
									BuildConstants.PackageLockFileName;

								await CheckGenerateAndRestoreRecursiveLocksAsync(
									dependencyPath,
									dependencyLockPath,
									packageStoreDirectory,
									packageLockStoreDirectory,
									stagingDirectory,
									processedLocks);
							}
						}
					}
				}
			}
		}

		/// <summary>
		/// Restore packages
		/// </summary>
		private async Task<PackageLock> EnsurePackageLockAsync(
			Path workingDirectory,
			Path packageLockPath)
		{
			Log.Info($"Ensure Package Lock Exists: {packageLockPath}");
			var loadPackageLock = await PackageLockExtensions.TryLoadFromFileAsync(packageLockPath);
			if (loadPackageLock.IsSuccess)
			{
				Log.Info("Restore from package lock");
				return loadPackageLock.Result;
			}
			else
			{ 
				Log.Info("Discovering full closure");
				var closure = new Dictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>>();
				var buildClosures = new Dictionary<string, IDictionary<string, IDictionary<string, PackageReference>>>();
				var toolClosures = new Dictionary<string, IDictionary<string, IDictionary<string, PackageReference>>>();
				await EnsureDiscoverLocalDependenciesAsync(
					workingDirectory,
					closure,
					buildClosures,
					toolClosures);

				// Attempt to resolve all dependencies to compatible and up-to-date versions
				Log.Info("Generate final service closure");
				await GenerateServiceClosureAsync(closure, buildClosures);

				// Build up the package lock file
				var packageLock = BuildPackageLock(workingDirectory, closure, buildClosures);

				// Save the updated package lock
				await PackageLockExtensions.SaveToFileAsync(packageLockPath, packageLock);

				return packageLock;
			}
		}

		private async Task GenerateServiceClosureAsync(
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> runtimeClosure,
			Dictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			// Publish the archive
			var packageClient = new Api.Client.ClosureClient(_httpClient)
			{
				BaseUrl = _apiEndpoint.ToString(),
			};

			// Pass in all non local runtime packages to build runtime closure
			var runtimePackages = new List<Api.Client.PackageFeedReferenceModel>();
			foreach (var (language, languageClosure) in runtimeClosure.OrderBy(value => value.Key))
			{
				foreach (var (packageName, (package, buildClosure)) in languageClosure.OrderBy(value => value.Key))
				{
					if (!package.IsLocal)
					{
						if (package.Version == null)
							throw new InvalidOperationException("External package reference version cannot be null");
						runtimePackages.Add(new Api.Client.PackageFeedReferenceModel()
						{
							Language = language,
							Name = package.Name,
							Version = new Api.Client.SemanticVersionModel()
							{
								Major = package.Version.Major,
								Minor = package.Version.Minor,
								Patch = package.Version.Patch,
							},
						});
					}
				}
			}

			// Pass in known build closures so the service can resolve them to actual build packages
			var requestBuildClosures = new List<Api.Client.BuildClosureModel>();
			foreach (var (buildClosureName, buildClosure) in buildClosures.OrderBy(value => value.Key))
			{
				var buildPackages = new List<Api.Client.PackageFeedReferenceModel>();
				foreach (var (language, languageClosure) in buildClosure.OrderBy(value => value.Key))
				{
					foreach (var (packageName, package) in languageClosure.OrderBy(value => value.Key))
					{
						if (!package.IsLocal)
						{
							if (package.Version == null)
								throw new InvalidOperationException("External package reference version cannot be null");
							buildPackages.Add(new Api.Client.PackageFeedReferenceModel()
							{
								Language = language,
								Name = package.Name,
								Version = new Api.Client.SemanticVersionModel()
								{
									Major = package.Version.Major,
									Minor = package.Version.Minor,
									Patch = package.Version.Patch,
								},
							});
						}
					}
				}

				requestBuildClosures.Add(new Api.Client.BuildClosureModel()
				{
					Name = buildClosureName,
					Closure = buildPackages,
				});
			}

			// Request the built in versions for the language extensions
			var requestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>();
			requestedVersions.Add(new Api.Client.PackageFeedExactReferenceModel()
			{
				Language = BuiltInLanguageWren,
				Name = BuiltInLanguagePackageCSharp,
				Version = new Api.Client.SemanticVersionExactModel()
				{
					Major = _builtInLanguageVersionCSharp.Major,
					Minor = _builtInLanguageVersionCSharp.Minor ?? throw new InvalidOperationException("Built In Language must be fully resolved"),
					Patch = _builtInLanguageVersionCSharp.Patch ?? throw new InvalidOperationException("Built In Language must be fully resolved"),
				},
			});
			requestedVersions.Add(new Api.Client.PackageFeedExactReferenceModel()
			{
				Language = BuiltInLanguageWren,
				Name = BuiltInLanguagePackageCpp,
				Version = new Api.Client.SemanticVersionExactModel()
				{
					Major = _builtInLanguageVersionCpp.Major,
					Minor = _builtInLanguageVersionCpp.Minor ?? throw new InvalidOperationException("Built In Language must be fully resolved"),
					Patch = _builtInLanguageVersionCpp.Patch ?? throw new InvalidOperationException("Built In Language must be fully resolved"),
				},
			});
			requestedVersions.Add(new Api.Client.PackageFeedExactReferenceModel()
			{
				Language = BuiltInLanguageWren,
				Name = BuiltInLanguagePackageWren,
				Version = new Api.Client.SemanticVersionExactModel()
				{
					Major = _builtInLanguageVersionWren.Major,
					Minor = _builtInLanguageVersionWren.Minor ?? throw new InvalidOperationException("Built In Language must be fully resolved"),
					Patch = _builtInLanguageVersionWren.Patch ?? throw new InvalidOperationException("Built In Language must be fully resolved"),
				},
			});

			var generateClosureRequest = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = runtimePackages,
				BuildClosures = requestBuildClosures,
				RequestedVersions = requestedVersions,
			};

			Api.Client.GenerateClosureResultModel result;
			try
			{
				result = await packageClient.GenerateClosureAsync(generateClosureRequest);
			}
			catch (HttpRequestException)
			{
				Log.Error("Failed to connect, check internet connection and try again");
				throw new HandledException();
			}
			catch (Api.Client.ApiException)
			{
				Log.Error("Service request failed");
				throw;
			}

			if (result.Result != Api.Client.GenerateClosureResult.Success)
			{
				Log.Error($"Unable to create closure: {result.Message}");
				throw new HandledException();
			}

			// Update the closure to use the new values
			foreach (var package in result.RuntimeClosure)
			{
				var originalLanguageClosure = runtimeClosure[package.Language];

				var packageReference = new PackageReference(
					null,
					package.Name,
					new SemanticVersion(package.Version.Major, package.Version.Minor, package.Version.Patch));
				originalLanguageClosure[package.Name] = (packageReference, package.Build);
			}

			// Update the build closures to use the new values
			foreach (var buildClosure in result.BuildClosures)
			{
				if (buildClosures.TryGetValue(buildClosure.Name, out var existingBuildClosure))
				{
					// Update the existing build closure
					foreach (var package in buildClosure.Closure)
					{
						if (!existingBuildClosure.ContainsKey(package.Language))
							existingBuildClosure.Add(package.Language, new Dictionary<string, PackageReference>());

						var packageReference = new PackageReference(
							null,
							package.Name,
							new SemanticVersion(package.Version.Major, package.Version.Minor, package.Version.Patch));
						existingBuildClosure[package.Language][package.Name] = packageReference;
					}
				}
				else
				{
					// Copy over a new closure
					var newBuildClosure = new Dictionary<string, IDictionary<string, PackageReference>>();
					foreach (var package in buildClosure.Closure)
					{
						var packageReference = new PackageReference(
							null,
							package.Name,
							new SemanticVersion(package.Version.Major, package.Version.Minor, package.Version.Patch));

						if (!newBuildClosure.ContainsKey(package.Language))
							newBuildClosure.Add(package.Language, new Dictionary<string, PackageReference>());
						newBuildClosure[package.Language].Add(package.Name, packageReference);
					}

					buildClosures.Add(buildClosure.Name, newBuildClosure);
				}
			}
		}

		private PackageLock BuildPackageLock(
			Path workingDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures)
		{
			var packageLock = new PackageLock();
			packageLock.SetVersion(PackageLockVersion);
			foreach (var (languageName, languageClosure) in closure.OrderBy(value => value.Key))
			{
				foreach (var (packageName, (package, buildClosure)) in languageClosure.OrderBy(value => value.Key))
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

					Log.Diag($"{RootClosureName}:{languageName} {packageName} -> {value}");
					packageLock.AddProject(
						RootClosureName,
						languageName,
						packageName,
						value,
						buildClosure);
				}
			}

			foreach (var buildClosure in buildClosures.OrderBy(value => value.Key))
			{
				packageLock.EnsureClosure(buildClosure.Key);
				foreach (var (languageName, languageClosure) in buildClosure.Value.OrderBy(value => value.Key))
				{
					foreach (var (packageName, package) in languageClosure)
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

						Log.Diag($"{buildClosure.Key}:{languageName} {packageName} -> {value}");
						packageLock.AddProject(
							buildClosure.Key,
							languageName,
							packageName,
							value,
							null);
					}
				}
			}

			return packageLock;
		}

		/// <summary>
		/// Recursively discover all dependencies
		/// </summary>
		private async Task EnsureDiscoverLocalDependenciesAsync(
			Path recipeDirectory,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> toolClosures)
		{
			var recipePath =
				recipeDirectory +
				BuildConstants.RecipeFileName;
			var (isSuccess, recipe) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipePath);
			if (!isSuccess)
			{
				throw new InvalidOperationException($"Could not load the recipe file: {recipePath}");
			}

			if (closure.TryGetValue(recipe.Language.Name, out var languageClosure) && languageClosure.ContainsKey(recipe.Name))
			{
				Log.Diag("Recipe already processed.");
			}
			else
			{
				// Create the unique build closure
				var buildClosure = await CreateBuildClosureAsync(recipe, recipeDirectory);

				var buildClosureName = string.Empty;
				var buildClosureMatch = buildClosures.FirstOrDefault(value => AreEqual(value.Value, buildClosure));
				if (buildClosureMatch.Key != null)
				{
					buildClosureName = buildClosureMatch.Key;
				}
				else
				{
					buildClosureName = $"BuildSet{buildClosures.Count}";
					buildClosures.Add(buildClosureName, buildClosure);
				}

				// Create the unique tool closure
				var toolClosure = await CreateToolClosureAsync(recipe, recipeDirectory);

				var toolClosureName = string.Empty;
				var toolClosureMatch = buildClosures.FirstOrDefault(value => AreEqual(value.Value, toolClosure));
				if (toolClosureMatch.Key != null)
				{
					toolClosureName = toolClosureMatch.Key;
				}
				else
				{
					toolClosureName = $"ToolSet{toolClosures.Count}";
					toolClosures.Add(toolClosureName, toolClosure);
				}

				// Add the project to the closure
				if (!closure.ContainsKey(recipe.Language.Name))
					closure.Add(recipe.Language.Name, new Dictionary<string, (PackageReference Package, string BuildClosure)>());
				closure[recipe.Language.Name].Add(recipe.Name, (new PackageReference(recipeDirectory), buildClosureName));

				await DiscoverLocalDependenciesAsync(
					recipeDirectory,
					recipe,
					closure,
					buildClosures,
					toolClosures);
			}
		}

		private static async Task<IDictionary<string, IDictionary<string, PackageReference>>> CreateBuildClosureAsync(
			Recipe recipe,
			Path recipeDirectory)
		{
			var buildClosure = new Dictionary<string, IDictionary<string, PackageReference>>();
			var implicitLanguage = BuiltInLanguageWren;

			// Add the language build extension
			var recipeLanguagePackage = GetLanguagePackage(recipe.Language.Name);
			buildClosure.Add(implicitLanguage, new Dictionary<string, PackageReference>());
			buildClosure[implicitLanguage].Add(
				recipeLanguagePackage,
				FillDefaultVersion(new PackageReference(implicitLanguage, recipeLanguagePackage, recipe.Language.Version)));

			// Discover any dependency build references
			if (recipe.HasBuildDependencies)
			{
				foreach (var dependency in recipe.BuildDependencies)
				{
					PackageReference dependencyPackage;
					string dependencyName;
					string dependencyLanguage;
					if (dependency.IsLocal)
					{
						// Load the recipe to check for the language and name of the package
						var dependencyPath = recipeDirectory + dependency.Path;
						var dependencyRecipePath =
							dependencyPath +
							BuildConstants.RecipeFileName;
						var (isDependencySuccess, dependencyRecipe) =
							await RecipeExtensions.TryLoadRecipeFromFileAsync(dependencyRecipePath);
						if (!isDependencySuccess)
						{
							throw new InvalidOperationException("Could not load dependency recipe file.");
						}

						dependencyPackage = dependency;
						dependencyName = dependencyRecipe.Name;
						dependencyLanguage = dependencyRecipe.Language.Name;
					}
					else
					{
						dependencyPackage = FillDefaultVersion(dependency);
						dependencyName = dependencyPackage.Name;
						dependencyLanguage = dependencyPackage.Language != null ? dependencyPackage.Language : implicitLanguage;
					}

					if (!buildClosure.ContainsKey(dependencyLanguage))
						buildClosure.Add(dependencyLanguage, new Dictionary<string, PackageReference>());

					buildClosure[dependencyLanguage].Add(dependencyName, dependencyPackage);
				}
			}

			return buildClosure;
		}

		private static async Task<IDictionary<string, IDictionary<string, PackageReference>>> CreateToolClosureAsync(
			Recipe recipe,
			Path recipeDirectory)
		{
			var toolClosure = new Dictionary<string, IDictionary<string, PackageReference>>();

			// Discover any dependency tool references
			if (recipe.HasToolDependencies)
			{
				foreach (var dependency in recipe.ToolDependencies)
				{
					PackageReference dependencyPackage;
					string dependencyName;
					string dependencyLanguage;
					if (dependency.IsLocal)
					{
						// Load the recipe to check for the language and name of the package
						var dependencyPath = recipeDirectory + dependency.Path;
						var dependencyRecipePath =
							dependencyPath +
							BuildConstants.RecipeFileName;
						var (isDependencySuccess, dependencyRecipe) =
							await RecipeExtensions.TryLoadRecipeFromFileAsync(dependencyRecipePath);
						if (!isDependencySuccess)
						{
							throw new InvalidOperationException("Could not load dependency recipe file.");
						}

						dependencyPackage = dependency;
						dependencyName = dependencyRecipe.Name;
						dependencyLanguage = dependencyRecipe.Language.Name;
					}
					else
					{
						dependencyPackage = FillDefaultVersion(dependency);

						if (dependencyPackage.Language == null)
						{
							throw new InvalidOperationException("Dependency must have explicit language.");
						}

						dependencyName = dependencyPackage.Name;
						dependencyLanguage = dependencyPackage.Language;
					}

					if (!toolClosure.ContainsKey(dependencyLanguage))
						toolClosure.Add(dependencyLanguage, new Dictionary<string, PackageReference>());

					toolClosure[dependencyLanguage].Add(dependencyName, dependencyPackage);
				}
			}

			return toolClosure;
		}

		/// <summary>
		/// Recursively discover all local dependencies, assume that the closure has been updated correctly for current recipe
		/// </summary>
		private async Task DiscoverLocalDependenciesAsync(
			Path recipeDirectory,
			Recipe recipe,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> toolClosures)
		{
			// Restore the explicit dependencies
			foreach (var dependencyType in recipe.GetDependencyTypes())
			{
				// Build dependencies covered in build closure
				if (dependencyType != Recipe.Property_Build &&
					dependencyType != Recipe.Property_Tool)
				{
					await DiscoverRuntimeDependenciesAsync(
						recipeDirectory,
						recipe,
						dependencyType,
						closure,
						buildClosures,
						toolClosures);
				}
			}
		}

		/// <summary>
		/// Recursively restore all runtime dependencies
		/// </summary>
		private async Task DiscoverRuntimeDependenciesAsync(
			Path recipeDirectory,
			Recipe recipe,
			string dependencyType,
			IDictionary<string, IDictionary<string, (PackageReference Package, string BuildClosure)>> closure,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> buildClosures,
			IDictionary<string, IDictionary<string, IDictionary<string, PackageReference>>> toolClosures)
		{
			// Same language as parent is implied
			var implicitLanguage = recipe.Language.Name;

			foreach (var dependency in recipe.GetNamedDependencies(dependencyType))
			{
				// If local then check children for external package references
				// Otherwise add the external package reference which will be expanded on the service call
				if (dependency.IsLocal)
				{
					Path dependencyPath = dependency.Path;
					if (!dependencyPath.HasRoot)
						dependencyPath = recipeDirectory + dependencyPath;

					await EnsureDiscoverLocalDependenciesAsync(
						dependencyPath,
						closure,
						buildClosures,
						toolClosures);
				}
				else
				{
					if (dependency.Version == null)
						throw new ArgumentException("Local package version was null");

					var language = dependency.Language != null ? dependency.Language : implicitLanguage;

					if (!closure.ContainsKey(language))
						closure.Add(language, new Dictionary<string, (PackageReference, string)>());

					if (closure[language].TryGetValue(dependency.Name, out var existingPackage))
					{
						// TODO: Verify compatible.
					}
					else
					{
						closure[language].Add(dependency.Name, (dependency, string.Empty));
					}
				}
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
				var isRuntime = closure.Key == RootClosureName;
				foreach (var (languageName, languageProjects) in closure.Value.Value.AsTable().Values)
				{
					Log.Info($"Restore Packages for Language {languageName}");
					foreach (var project in languageProjects.Value.AsArray().Values)
					{
						var projectTable = project.Value.AsTable();
						var projectName = projectTable.Values[PackageLock.Property_Name].Value.AsString().Value;
						var projectVersion = projectTable.Values[PackageLock.Property_Version].Value.AsString().Value;
						if (SemanticVersion.TryParse(projectVersion, out var version))
						{
							await EnsurePackageDownloadedAsync(
								isRuntime,
								languageName,
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
		/// Ensure a package version is downloaded
		/// </summary>
		private async Task EnsurePackageDownloadedAsync(
			bool isRuntime,
			string languageName,
			string packageName,
			SemanticVersion packageVersion,
			Path packageStore,
			Path stagingDirectory)
		{
			Log.HighPriority($"Install Package: {languageName} {packageName}@{packageVersion}");

			var languageSafeName = GetLanguageSafeName(languageName);
			var languageRootFolder = packageStore + new Path(languageSafeName);
			var packageRootFolder = languageRootFolder + new Path(packageName);
			var packageVersionFolder = packageRootFolder + new Path(packageVersion.ToString()) + new Path();

			// Check if the package version already exists
			if (!isRuntime &&
				((packageName == BuiltInLanguagePackageCpp && packageVersion == _builtInLanguageVersionCpp) ||
				(packageName == BuiltInLanguagePackageCSharp && packageVersion == _builtInLanguageVersionCSharp) ||
				(packageName == BuiltInLanguagePackageWren && packageVersion == _builtInLanguageVersionWren)))
			{
				Log.HighPriority("Skip built in language version in build closure");
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

				var client = new Api.Client.PackageVersionsClient(_httpClient)
				{
					BaseUrl = _apiEndpoint.ToString(),
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

		private static string GetLanguageSafeName(string language)
		{
			switch (language)
			{
				case BuiltInLanguageCSharp:
					return BuiltInLanguageSafeNameCSharp;
				case BuiltInLanguageCpp:
					return BuiltInLanguageSafeNameCpp;
				case BuiltInLanguageWren:
					return BuiltInLanguageSafeNameWren;
				default:
					throw new InvalidOperationException($"Unknown language name: {language}");
			}
		}

		private static string GetLanguagePackage(string language)
		{
			switch (language)
			{
				case BuiltInLanguageCSharp:
					return BuiltInLanguagePackageCSharp;
				case BuiltInLanguageCpp:
					return BuiltInLanguagePackageCpp;
				case BuiltInLanguageWren:
					return BuiltInLanguagePackageWren;
				default:
					throw new InvalidOperationException($"Unknown language name: {language}");
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
		private static void EnsureDirectoryExists(Path directory)
		{
			if (!LifetimeManager.Get<IFileSystem>().Exists(directory))
			{
				// Create the folder
				Log.Diag($"Create Directory: {directory}");
				LifetimeManager.Get<IFileSystem>().CreateDirectory2(directory);
			}
		}
	}
}
