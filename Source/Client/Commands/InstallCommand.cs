// <copyright company="Soup" file="InstallCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
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
	internal class InstallCommand : ICommand
	{
		public string Name => "install";

		/// <summary>
		/// Invoke the install command
		/// </summary>
		public async Task InvokeAsync(string[] args)
		{
			if (args.Length > 2)
			{
				ShowUsage();
				return;
			}

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

			var userConfig = Singleton<LocalUserConfig>.Instance;
			var stagingPath = PackageManager.EnsureStagingDirectoryExists(userConfig.PackageStore);

			// Ensure that the staging directory exists
			var tempStagingPath = Path.Combine(stagingPath, "temp");

			var package = args.Length < 2 ? null : args[1];
			if (string.IsNullOrEmpty(package))
			{
				Log.Message("Install All");

				await InstallRecursiveDependencies(tempStagingPath, recipe);
			}
			else if (!Path.HasExtension(package))
			{
				Log.Message($"Install Package: {package}");

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
					await GenerateBuildAsync(tempStagingPath, installedRecipe);
					var installedPackageRef = new PackageReference(installedRecipe.Name, installedRecipe.Version);

					// Register the package in the recipe
					recipe.Dependencies.Add(installedPackageRef);
				}
			}
			else if (Path.GetExtension(package) == Constants.ArchiveFileExtension)
			{
				Log.Message($"Installing Local Package: {package}");

				if (!File.Exists(package))
				{
					throw new ArgumentException("The specified file does not exist.");
				}

				// Install the package
				using (var archiveStream = File.OpenRead(package))
				{
					var installedRecipe = await InstallPackageAsync(tempStagingPath, archiveStream);
					await GenerateBuildAsync(tempStagingPath, installedRecipe);
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
				ShowUsage();
				return;
			}

			// Cleanup
			Directory.Delete(tempStagingPath, true);

			// Save the state of the recipe if it has changed
			if (recipe.IsDirty)
			{
				await RecipeManager.SaveToFileAsync(recipe);
			}
		}

		private async Task<SemanticVersion> GetLatestAsync(string name)
		{
			var package = await Singleton<ISoupApi>.Instance.GetPackageAsync(name);
			return SemanticVersion.Parse(package.Latest);
		}

		private async Task<Stream> DownloadPackageAsync(string name, SemanticVersion version)
		{
			var stream = await Singleton<ISoupApi>.Instance.DownloadPackageAsync(name, version);
			return stream;
		}

		private async Task<Recipe> InstallPackageAsync(string tempPath, Stream packageFile)
		{
			var recipe = await UnpackArchiveAsync(tempPath, packageFile);
			await GenerateIncludeAsync(tempPath, recipe);

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
			var packagePath = PackageManager.BuildKitchenPackagePath(recipe);

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

		private async Task GenerateBuildAsync(string tempPath, Recipe recipe)
		{
			// Generate the build projects
			var tempBuildPath = Path.Combine(tempPath, Constants.StoreBuildFolderName);
			var buildPath = PackageManager.BuildKitchenBuildPath("MSBuild", recipe);
			var includePath = PackageManager.BuildKitchenIncludePath(recipe);
			var packagePath = PackageManager.BuildKitchenPackagePath(recipe);
			var libraryPath = PackageManager.BuildKitchenLibraryPath();

			Directory.CreateDirectory(tempBuildPath);

			var buildGenerator = new MSBuild.BuildGenerator();
			await buildGenerator.GenerateDependenciesAsync(recipe, tempBuildPath);
			buildGenerator.GenerateBuild(recipe, tempBuildPath, packagePath, libraryPath, "out");

			// TODO : Should not hit this when, verify the package exists before download
			// For now delete and recreate it
			if (Directory.Exists(buildPath))
			{
				Directory.Delete(buildPath, true);
			}

			// Ensure the parent directory exists
			var buildParentDirectory = Directory.GetParent(buildPath);
			if (!buildParentDirectory.Exists)
			{
				buildParentDirectory.Create();
			}

			// Move the results out of the staging directory
			Directory.Move(tempBuildPath, buildPath);
		}

		private async Task GenerateIncludeAsync(string tempPath, Recipe recipe)
		{
			var includePath = PackageManager.BuildKitchenIncludePath(recipe);
			var packagePath = PackageManager.BuildKitchenPackagePath(recipe);

			// Create the temporary include folders
			var tempIncludePath = Path.Combine(tempPath, Constants.StoreIncludeRootFolderName);
			Directory.CreateDirectory(tempIncludePath);

			// Create the soup include directory
			var soupIncludePath = Path.Combine(tempIncludePath, Constants.StoreSoupIncludeFolderName);
			Directory.CreateDirectory(soupIncludePath);

			// Create the include objects
			await PackageManager.CreatePublicIncludeHeaderAsync(recipe, packagePath, soupIncludePath);

			// TODO : Should not hit this when, verify the package exists before download
			// For now delete and recreate it
			if (Directory.Exists(includePath))
			{
				Log.Message("Removing old version.");
				Directory.Delete(includePath, true);
			}

			// Ensure the parent directory exists
			var includeParentDirectory = Directory.GetParent(includePath);
			if (!includeParentDirectory.Exists)
			{
				includeParentDirectory.Create();
			}

			// Move the results out of the staging directory
			Directory.Move(tempIncludePath, includePath);
		}

		/// <summary>
		/// Recusively install all dependencies and trasient dependecies
		/// </summary>
		private async Task InstallRecursiveDependencies(string tempPath, Recipe recipe)
		{
			foreach (var dep in recipe.Dependencies)
			{
				Log.Message($"{dep}");

				// Download the archive
				using (var archiveStream = await DownloadPackageAsync(dep.Name, dep.Version))
				{
					// Install the package
					var installedRecipe = await InstallPackageAsync(tempPath, archiveStream);

					// Install dependecies recursively 
					await InstallRecursiveDependencies(tempPath, installedRecipe);

					// Now generate the build files
					await GenerateBuildAsync(tempPath, installedRecipe);
				}
			}
		}

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		private void ShowUsage()
		{
			Log.Message("");
			Log.Message("Usage: soup install <package_file>");
			Log.Message("\tpackage_file: Must be a zip file.");
		}
	}
}
