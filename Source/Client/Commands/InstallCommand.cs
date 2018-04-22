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

			var workingDirectory = @".\";
			var recipe = await RecipeManager.LoadFromFileAsync(workingDirectory);
			if (recipe == null)
			{
				Log.Error("The recipe file is required to install.");
				return;
			}

			var package = args.Length < 2 ? null : args[1];
			if (string.IsNullOrEmpty(package))
			{
				Log.Message("Install All");

				await InstallRecursiveDependencies(recipe);
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
				var archiveFile = await DownloadPackageAsync(package, latestVersion);

				// Install the package
				var installedRecipe = await InstallPackageAsync(archiveFile);
				var installedPackageRef = new PackageReference(installedRecipe.Name, installedRecipe.Version);
				
				// Register the package in the recipe
				recipe.Dependencies.Add(installedPackageRef);
			}
			else if (Path.GetExtension(package) == Constants.ArchiveFileExtension)
			{
				Log.Message($"Installing Local Package: {package}");

				if (!File.Exists(package))
				{
					throw new ArgumentException("The specified file does not exist.");
				}

				// Install the package
				var installedRecipe = await InstallPackageAsync(package);
				var installedPackageRef = new PackageReference(installedRecipe.Name, installedRecipe.Version);

				// Register the package in the recipe if it does not exist
				if (!recipe.Dependencies.Any(dependency => dependency == installedPackageRef))
				{
					recipe.Dependencies.Add(installedPackageRef);
				}
			}
			else
			{
				Log.Error("Unknown install source type.");
				ShowUsage();
				return;
			}

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

		private async Task<string> DownloadPackageAsync(string name, SemanticVersion version)
		{
			var userConfig = Singleton<LocalUserConfig>.Instance;

			// Ensure that the staging directory exists
			var path = PackageManager.EnsureStagingDirectoryExists(userConfig.PackageStore);
			
			var stream = await Singleton<ISoupApi>.Instance.DownloadPackageAsync(name, version);

			var filename = $"{name}_{version}.tgz";
			var filepath = Path.Combine(path, filename);
			using (var archivefile = File.Create(filepath))
			{
				await stream.CopyToAsync(archivefile);
			}

			return filepath;
		}

		private async Task<Recipe> InstallPackageAsync(string packageFile)
		{
			var userConfig = Singleton<LocalUserConfig>.Instance;

			// Ensure that the staging directory exists
			var stagingPath = Path.Combine(userConfig.PackageStore, Constants.StagingFolderName);
			PackageManager.EnsureStagingDirectoryExists(userConfig.PackageStore);
			var tempStagingPath = Path.Combine(stagingPath, "temp");

			// Unpack the zip file into the package directory
			var tempPackageDirectory = Path.Combine(tempStagingPath, Constants.PackageFolderName);
			Directory.CreateDirectory(tempPackageDirectory);
			await PackageManager.ExtractAsync(packageFile, tempPackageDirectory);

			// Load the packages recipe file
			var recipe = await RecipeManager.LoadFromFileAsync(tempPackageDirectory);
			var projectVersionFolder = PackageManager.BuildPackageVersionPath(recipe.Name, recipe.Version);
			var projectVersionPath = Path.Combine(userConfig.PackageStore, projectVersionFolder);
			var packageDirectory = Path.Combine(projectVersionPath, Constants.PackageFolderName);

			// For now delete and recreate it
			if (Directory.Exists(projectVersionPath))
			{
				Directory.Delete(projectVersionPath, true);
			}

			// Move the results out of the staging directory
			if (!Directory.Exists(projectVersionPath))
			{
				// TODO : No nead to unzip and what not
				// Ensure that the project exists
				var projectPath = Path.Combine(userConfig.PackageStore, recipe.Name);
				Directory.CreateDirectory(projectPath);
				Directory.Move(tempStagingPath, projectVersionPath);
			}

			// Generate the build projects
			var buildDirectory = Path.Combine(projectVersionPath, Constants.BuildFolderName);
			var includeDirectory = Path.Combine(projectVersionPath, Constants.IncludeFolderName);
			Directory.CreateDirectory(buildDirectory);
			var buildGenerator = new VisualStudioBuild.BuildGenerator();
			buildGenerator.GenerateDependencies(recipe, packageDirectory, buildDirectory, "out\\");
			buildGenerator.GenerateInclude(recipe, buildDirectory, includeDirectory);
			buildGenerator.GenerateBuild(recipe, packageDirectory, buildDirectory);

			// Create the include objects
			Directory.CreateDirectory(includeDirectory);
			await PackageManager.CreatePublicIncludeHeaderAsync(recipe, packageDirectory, includeDirectory);

			// Cleanup
			Directory.Delete(stagingPath, true);

			// Return the recipe for the installed package
			return recipe;
		}

		/// <summary>
		/// Recusively install all dependencies and trasient dependecies
		/// </summary>
		/// <param name="recipe"></param>
		/// <returns></returns>
		private async Task InstallRecursiveDependencies(Recipe recipe)
		{
			foreach (var dep in recipe.Dependencies)
			{
				Log.Message($"{dep.Name}@{dep.Version}");

				// Download the archive
				var archiveFile = await DownloadPackageAsync(dep.Name, dep.Version);

				// Install the package
				var installedRecipe = await InstallPackageAsync(archiveFile);

				// Install dependecies recursively 
				await InstallRecursiveDependencies(installedRecipe);
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
