// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace Soup
{
	class InstallCommand : ICommand
	{
		public string Name => "install";

		/// <summary>
		/// Invoke the install command
		/// </summary>
		public async Task InvokeAsync(string[] args, LocalUserConfig userConfig)
		{
			if (args.Length < 2)
			{
				ShowUsage();
				return;
			}

			var workingDirectory = @".\";
			var package = args[1];
			var recipe = await RecipeManager.LoadFromFileAsync(workingDirectory);
			if (recipe == null)
			{
				Log.Error("The recipe file is required to install.");
				return;
			}

			if (package.EndsWith(Constants.ArchiveFileExtension))
			{
				Log.Message($"Installing Local Package: {package}");

				if (!File.Exists(package))
				{
					throw new ArgumentException("The specified file does not exist.");
				}

				var packageReference = await InstallPackage(userConfig, package);
				if (!recipe.Dependencies.Any((dependency => dependency == packageReference)))
				{
					recipe.Dependencies.Add(packageReference);
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

		private async Task<PackageReference> InstallPackage(LocalUserConfig userConfig, string package)
		{
			// Ensure that the staging directory exists
			var stagingPath = Path.Combine(userConfig.PackageStore, Constants.StagingFolderName);
			PackageManager.EnsureStagingDirectoryExists(userConfig.PackageStore);
			var tempStagingPath = Path.Combine(stagingPath, "temp");

			// Unpack the zip file into the package directory
			var tempPackageDirectory = Path.Combine(tempStagingPath, Constants.PackageFolderName);
			Directory.CreateDirectory(tempPackageDirectory);
			await PackageManager.ExtractAsync(package, tempPackageDirectory);

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
			Directory.CreateDirectory(buildDirectory);
			var buildGenerator = new VisualStudioBuild.BuildGenerator();
			buildGenerator.GenerateDependencies(userConfig, recipe, packageDirectory, buildDirectory, "out\\");
			buildGenerator.GenerateInclude(recipe, packageDirectory, buildDirectory);
			buildGenerator.GenerateBuild(recipe, packageDirectory, buildDirectory);

			// Create the include objects
			var includeDirectory = Path.Combine(projectVersionPath, Constants.IncludeFolderName);
			Directory.CreateDirectory(includeDirectory);
			await PackageManager.CreatePublicIncludeHeaderAsync(recipe, packageDirectory, includeDirectory);

			// Cleanup
			Directory.Delete(stagingPath, true);

			// Build the package reference
			return new PackageReference(recipe.Name, recipe.Version);
		}

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		private static void ShowUsage()
		{
			Log.Message("");
			Log.Message("Usage: soup install <package_file>");
			Log.Message("\tpackage_file: Must be a zip file.");
		}
	}
}
