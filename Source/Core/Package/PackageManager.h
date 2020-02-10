// <copyright file="PackageManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"

namespace Soup
{
	/// <summary>
	/// The package manager
	/// </summary>
	export class PackageManager
	{
	public:
		/// <summary>
		/// Install a package
		/// </summary>
		static void InstallPackage(const std::string& name)
		{
			auto workingDirectory = Path();
			auto recipePath =
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadFromFile(recipePath, recipe))
			{
				throw std::runtime_error("Could not load the recipe file.");
			}

			// auto stagingPath = EnsureStagingDirectoryExists(_config.PackageStore);

			// // Ensure that the staging directory exists
			// var tempStagingPath = Path.Combine(stagingPath, "temp");

			// var package = options.Package;
			// if (string.IsNullOrEmpty(package))
			// {
			// 	 Log::Info("Install All");

			// 	 InstallRecursiveDependencies(tempStagingPath, recipe);
			// }
			// else if (!Path.HasExtension(package))
			// {
			// 	 Log::Info($"Install Package: {package}");

			// 	 // Check if the package is already installed
			// 	 if (recipe.Dependencies.Any(dependency => dependency.Name == package))
			// 	 {
			// 		 Log::Warning("Package already installed.");
			// 		 return;
			// 	 }

			// 	 // Get the latest version
			// 	 auto latestVersion = await GetLatestAsync(package);

			// 	 // Download the archive
			// 	 using (auto archiveStream = await DownloadPackageAsync(package, latestVersion))
			// 	 {
			// 		 // Install the package
			// 		 auto installedRecipe = await InstallPackageAsync(tempStagingPath, archiveStream);
			// 		 auto installedPackageRef = new PackageReference(installedRecipe.Name, installedRecipe.Version);

			// 		 // Register the package in the recipe
			// 		 recipe.Dependencies.Add(installedPackageRef);
			// 	 }
			// }
			// else if (Path.GetExtension(package) == Constants.ArchiveFileExtension)
			// {
			// 	 Log.Info($"Installing Local Package: {package}");

			// 	 if (!File.Exists(package))
			// 	 {
			// 		 throw ArgumentException("The specified file does not exist.");
			// 	 }

			// 	 // Install the package
			// 	 using (var archiveStream = File.OpenRead(package))
			// 	 {
			// 		 auto installedRecipe = await InstallPackageAsync(tempStagingPath, archiveStream);
			// 		 auto installedPackageRef = new PackageReference(installedRecipe.Name, installedRecipe.Version);

			// 		 // Register the package in the recipe if it does not exist
			// 		 if (!recipe.Dependencies.Any(dependency => dependency == installedPackageRef))
			// 		 {
			// 			 recipe.Dependencies.Add(installedPackageRef);
			// 		 }
			// 	 }
			// }
			// else
			// {
			// 	 Log.Error("Unknown install source type.");
			// 	 Directory.Delete(tempStagingPath, true);
			// 	 return;
			// }

			// // Cleanup the working directory
			// if (Directory.Exists(tempStagingPath))
			// {
			// 	 Directory.Delete(tempStagingPath, true);
			// }

			// Save the state of the recipe
			RecipeExtensions::SaveToFile(recipePath, recipe);
		}

	private:
		/// <summary>
		/// Build the kitchen library path
		/// </summary>
		// static string BuildKitchenLibraryPath(LocalUserConfig config)
		// {
		// 	return config.PackageStore +
		// 		Path(Constants::StoreLibraryFolderName);
		// }

		// /// <summary>
		// /// Build the kitchen build path
		// /// </summary>
		// static std::string BuildKitchenBuildPath(LocalUserConfig config, Recipe recipe)
		// {
		// 	return BuildKitchenBuildPath(config, recipe.Name, recipe.Version);
		// }

		// /// <summary>
		// /// Build the kitchen build path
		// /// </summary>
		// static string BuildKitchenBuildPath(LocalUserConfig config, PackageReference reference)
		// {
		// 	return BuildKitchenBuildPath(config, reference.Name, reference.Version);
		// }

		// /// <summary>
		// /// Build the kitchen package path
		// /// </summary>
		// static string BuildKitchenPackagePath(LocalUserConfig config, Recipe recipe)
		// {
		// 	return BuildKitchenPackagePath(config, recipe.Name, recipe.Version);
		// }

		// /// <summary>
		// /// Build the kitchen package path
		// /// </summary>
		// static string BuildKitchenPackagePath(LocalUserConfig config, PackageReference reference)
		// {
		// 	return BuildKitchenPackagePath(config, reference.Name, reference.Version);
		// }

		// /// <summary>
		// /// Build the kitchen package path
		// /// </summary>
		// static string BuildKitchenIncludePath(LocalUserConfig config, Recipe recipe)
		// {
		// 	return BuildKitchenIncludePath(config, recipe.Name, recipe.Version);
		// }

		// /// <summary>
		// /// Build the kitchen include path
		// /// </summary>
		// static string BuildKitchenIncludePath(LocalUserConfig config, PackageReference reference)
		// {
		// 	return BuildKitchenIncludePath(config, reference.Name, reference.Version);
		// }

		// /// <summary>
		// /// Build the recursive dependencies
		// /// </summary>
		// static async Task<List<PackageReference>> BuildRecursiveDependeciesAsync(LocalUserConfig config, Recipe recipe)
		// {
		// 	List<PackageReference> result = new List<PackageReference>();
		// 	foreach (var dependency in recipe.Dependencies)
		// 	{
		// 		result.Add(dependency);
		// 		var dependencyPackagePath = BuildKitchenPackagePath(config, dependency);
		// 		var dependencyRecipe = await RecipeManager.LoadFromFileAsync(dependencyPackagePath);
		// 		var transientDependencies = await BuildRecursiveDependeciesAsync(config, dependencyRecipe);
		// 		result.AddRange(transientDependencies);
		// 	}

		// 	return result;
		// }

		// /// <summary>
		// /// Ensure the staging directory exists
		// /// </summary>
		// static string EnsureStagingDirectoryExists(string directory)
		// {
		// 	var path = Path.Combine(directory, Constants.StagingFolderName);
		// 	if (!Directory.Exists(path))
		// 	{
		// 		// Create the folder
		// 		var info = Directory.CreateDirectory(path);

		// 		// Hide the folder
		// 		info.Attributes |= FileAttributes.Hidden;
		// 	}

		// 	return path;
		// }

		// /// <summary>
		// /// Ensure the project generate folder exists
		// /// </summary>
		// static void EnsureProjectGenerateFolderExists(string directory)
		// {
		// 	var path = Path.Combine(directory, Constants.ProjectGenerateFolderName);
		// 	if (!Directory.Exists(path))
		// 	{
		// 		// Create the folder
		// 		var info = Directory.CreateDirectory(path);

		// 		// Hide the folder
		// 		info.Attributes |= FileAttributes.Hidden;
		// 	}
		// }

		// /// <summary>
		// /// Find the source files
		// /// </summary>
		// static List<string> FindSourceFiles(Recipe recipe, string packageDirectory)
		// {
		// 	return FindFiles(recipe.Source, packageDirectory);
		// }

		// /// <summary>
		// /// Find the files
		// /// </summary>
		// static List<string> FindFiles(IList<string> patterns, string directory)
		// {
		// 	List<string> result = new List<string>();

		// 	// Create matching patterns for each source items
		// 	var includePatterns = new List<Glob.Glob>();
		// 	foreach (var pattern in patterns)
		// 	{
		// 		var cleanPattern = pattern.Replace("/", "\\").ToLower();
		// 		includePatterns.Add(new Glob.Glob(cleanPattern));
		// 	}

		// 	// Check every file in the directory
		// 	foreach (var file in Directory.EnumerateFiles(directory, "*", SearchOption.AllDirectories))
		// 	{
		// 		var relativePath = Path.GetRelativePath(directory, file);
		// 		var cleanRelativePath = relativePath.ToLower();
		// 		bool match = includePatterns.Any(pattern => pattern.IsMatch(cleanRelativePath));
		// 		if (match)
		// 		{
		// 			result.Add(relativePath);
		// 		}
		// 	}

		// 	return result;
		// }

		// /// <summary>
		// /// Pack the archive
		// /// </summary>
		// static async Task PackAsync(Recipe recipe, string directory)
		// {
		// 	var zipFileName = $"{recipe.Name}_{recipe.Version}.tgz";
		// 	var zipFilePath = Path.Combine(directory, zipFileName);
		// 	using (var zipFile = File.Create(zipFilePath))
		// 	{
		// 		await PackAsync(recipe, directory, zipFile);
		// 	}
		// }

		// /// <summary>
		// /// Pack the archive
		// /// </summary>
		// static Task PackAsync(Recipe recipe, string directory, Stream stream)
		// {
		// 	var includePatterns = new List<Glob.Glob>();

		// 	// Include the Recipe file
		// 	includePatterns.Add(new Glob.Glob(Constants.RecipeFileName.ToLower()));

		// 	// Include all or the source filess
		// 	foreach (var source in recipe.Source)
		// 	{
		// 		includePatterns.Add(new Glob.Glob(source.ToLower()));
		// 	}

		// 	using (var gzipStream = new GZipStream(stream, CompressionLevel.Optimal, true))
		// 	using (var archive = TarArchive.CreateOutputTarArchive(gzipStream))
		// 	{
		// 		archive.RootPath = directory;

		// 		// Check every file in the directory
		// 		foreach (var file in Directory.EnumerateFiles(directory, "*", SearchOption.AllDirectories))
		// 		{
		// 			var relativePath = Path.GetRelativePath(directory, file);
		// 			bool matchInclude = includePatterns.Any(pattern => pattern.IsMatch(relativePath.ToLower()));
		// 			if (matchInclude)
		// 			{
		// 				Log.Verbose(relativePath);
		// 				var entry = TarEntry.CreateEntryFromFile(file);
		// 				archive.WriteEntry(entry, true);
		// 			}
		// 		}
		// 	}

		// 	return Task.CompletedTask;
		// }

		// /// <summary>
		// /// Unpack the archive
		// /// </summary>
		// static Task ExtractAsync(Stream source, string targetDirectory)
		// {
		// 	using (var gzipStream = new GZipStream(source, CompressionMode.Decompress))
		// 	using (TarArchive archive = TarArchive.CreateInputTarArchive(gzipStream))
		// 	{
		// 		archive.ExtractContents(targetDirectory);
		// 	}

		// 	return Task.CompletedTask;
		// }

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

		// private static string BuildKitchenPackagePath(LocalUserConfig config, string projectName, SemanticVersion version)
		// {
		// 	var kitchenPath = config.PackageStore;
		// 	var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
		// 	var path = Path.Combine(kitchenPath, Constants.StorePackageFolderName, packageVersionDirectory);
		// 	return path;
		// }

		// private static string BuildKitchenBuildPath(LocalUserConfig config, string projectName, SemanticVersion version)
		// {
		// 	var kitchenPath = config.PackageStore;
		// 	var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
		// 	var path = Path.Combine(kitchenPath, Constants.StoreBuildFolderName, packageVersionDirectory);
		// 	return path;
		// }

		// private static string BuildKitchenIncludePath(LocalUserConfig config, string projectName, SemanticVersion version)
		// {
		// 	var kitchenPath = config.PackageStore;
		// 	var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
		// 	var path = Path.Combine(kitchenPath, Constants.StoreIncludeRootFolderName, packageVersionDirectory);
		// 	return path;
		// }
	};
}
