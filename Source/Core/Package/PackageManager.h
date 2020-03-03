// <copyright file="PackageManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "Api/SoupApi.h"
#include "LzmaExtractCallback.h"
#include "LzmaInStream.h"

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
		static void InstallPackage(const std::string& packageName, const Path& packageStore)
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

			// Create the staging directory
			auto stagingPath = EnsureStagingDirectoryExists(packageStore);

			try
			{
				Log::HighPriority("Install Package: " + packageName);

				// Check if the package is already installed
				auto packageNameNormalized = ToUpper(packageName);
				if (recipe.HasDependencies())
				{
					for (auto& dependency : recipe.GetDependencies())
					{
						auto dependencyNameNormalized = ToUpper(dependency.GetName());
						if (dependencyNameNormalized == packageNameNormalized)
						{
							Log::Warning("Package already installed.");
							return;
						}
					}
				}

				// Get the latest version
				auto packageModel = GetPackageModel(packageName);
				auto latestVersion = packageModel.GetLatest();
				Log::HighPriority("Latest Version: " + latestVersion.ToString());

				EnsurePackageDownloaded(
					packageModel.GetName(),
					latestVersion,
					packageStore,
					stagingPath);

				// Cleanup the working directory
				Log::Info("Deleting staging directory");
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);

				// Register the package in the recipe
				Log::Info("Adding reference to recipe");
				auto installedPackageReference = PackageReference(
					packageModel.GetName(),
					latestVersion);
				auto dependencies = std::vector<PackageReference>();
				if (recipe.HasDependencies())
					dependencies = recipe.GetDependencies();

				dependencies.push_back(installedPackageReference);
				recipe.SetDependencies(dependencies);

				// Save the state of the recipe
				RecipeExtensions::SaveToFile(recipePath, recipe);
			}
			catch(const std::exception& e)
			{
				// Cleanup the staging directory and accept that we failed
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);
				throw;
			}
		}

		/// <summary>
		/// Publish a package
		/// </summary>
		static void PublishPackage(const Path& packageStore)
		{
			Log::Info("Publish Project: {recipe.Name}@{recipe.Version}");

			auto workingDirectory = Path();
			auto recipePath =
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadFromFile(recipePath, recipe))
			{
				throw std::runtime_error("Could not load the recipe file.");
			}

			// Create the staging directory
			auto stagingPath = EnsureStagingDirectoryExists(packageStore);

			try
			{
				auto archivePath = stagingPath + Path(std::string(recipe.GetName()) + ".7z");
				auto files = GetPackageFiles(workingDirectory);

				// Create the archive of the package
				auto archive = LzmaSdk::ArchiveWriter(archivePath.ToString());
				archive.AddFiles(files);
				archive.Save();

				// Publish the archive, scope cleanup file access
				{
					auto archiveFile = System::IFileSystem::Current().OpenRead(archivePath, true);
					Api::SoupApi::PublishPackage(recipe.GetName(), recipe.GetVersion(), archiveFile->GetInStream());
				}

				// Cleanup the staging directory
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);

				Log::HighPriority("Done");
			}
			catch(const std::exception& e)
			{
				// Cleanup the staging directory and accept that we failed
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);
				throw;
			}
		}

	private:
		static Soup::Api::PackageResultModel GetPackageModel(const std::string& name)
		{
			try
			{
				auto package = Api::SoupApi::GetPackage(name);
				return package;
			}
			catch(const Api::ApiException& ex)
			{
				switch (ex.GetStatusCode())
				{
					case Network::HttpStatusCode::NotFound:
						Log::Error("Package does not exist: " + name);
				}

				throw;
			}
		}

		static std::vector<std::string> GetPackageFiles(const Path& workingDirectory)
		{
			auto result = std::vector<std::string>();

			for (auto& child : System::IFileSystem::Current().GetDirectoryChildren(workingDirectory))
			{
				if (child.IsDirectory)
				{
					// Ignore output folder
					if (child.Path.GetFileName() != "out")
					{
						auto directoryFiles = GetAllFilesRecursive(child.Path);
						result.insert(
							result.end(),
							directoryFiles.begin(),
							directoryFiles.end());
					}
				}
				else
				{
					result.push_back(child.Path.ToString());
				}
			}

			return result;
		}

		static std::vector<std::string> GetAllFilesRecursive(const Path& directory)
		{
			auto result = std::vector<std::string>();

			for (auto& child : System::IFileSystem::Current().GetDirectoryChildren(directory))
			{
				if (child.IsDirectory)
				{
					auto directoryFiles = GetAllFilesRecursive(child.Path);
					result.insert(
						result.end(),
						directoryFiles.begin(),
						directoryFiles.end());
				}
				else
				{
					result.push_back(child.Path.ToString());
				}
			}

			return result;
		}

		/// <summary>
		/// Ensure a package version is downloaded
		/// </summary>
		static void EnsurePackageDownloaded(
			const std::string name,
			SemanticVersion version,
			const Path& packagesDirectory,
			const Path& stagingDirectory)
		{
			auto packageRootFolder = packagesDirectory + Path(name);
			auto packageVersionFolder = packageRootFolder + Path(version.ToString());

			// Check if the package version already exists
			if (System::IFileSystem::Current().Exists(packageVersionFolder))
			{
				Log::HighPriority("Found local version");
			}
			else
			{
				// Download the archive
				Log::HighPriority("Downloading package");
				auto archiveContent = Api::SoupApi::DownloadPackage(name, version);
				auto archivePath = stagingDirectory + Path(name + ".7z");

				// Write the contents to disk, scope cleanup
				auto archiveWriteFile = System::IFileSystem::Current().OpenWrite(archivePath, true);
				archiveWriteFile->GetOutStream() << archiveContent;
				archiveWriteFile->Close();

				// Create the package folder to extract to
				auto stagingVersionFolder = stagingDirectory + Path(version.ToString());
				System::IFileSystem::Current().CreateDirectory2(stagingVersionFolder);

				// Unpack the contents of the archive
				{
					auto archiveStream = System::IFileSystem::Current().OpenRead(archivePath, true);
					auto inStream = std::make_shared<LzmaInStream>(archiveStream);
					auto archive = LzmaSdk::ArchiveReader(inStream);
					auto callback = std::make_shared<LzmaExtractCallback>();
					archive.ExtractAll(stagingVersionFolder.ToString(), callback);
				}

				// Ensure the package root folder exists
				if (!System::IFileSystem::Current().Exists(packageRootFolder))
				{
					// Create the folder
					System::IFileSystem::Current().CreateDirectory2(packageRootFolder);
				}

				// Move the extracted contents into the version folder
				System::IFileSystem::Current().Rename(stagingVersionFolder, packageVersionFolder);
			}
		}

		// /// <summary>
		// /// Recursively install all dependencies and transitive dependencies
		// /// </summary>
		// void InstallRecursiveDependencies(string tempPath, Recipe recipe)
		// {
		// 	 foreach (var dep in recipe.Dependencies)
		// 	 {
		// 		 Log.Info($"{dep}");

		// 		 // Download the archive
		// 		 using (var archiveStream = await DownloadPackageAsync(dep.Name, dep.Version))
		// 		 {
		// 			 // Install the package
		// 			 var installedRecipe = await InstallPackageAsync(tempPath, archiveStream);

		// 			 // Install dependecies recursively
		// 			 await InstallRecursiveDependencies(tempPath, installedRecipe);
		// 		 }
		// 	 }
		// }

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

		/// <summary>
		/// Ensure the staging directory exists
		/// </summary>
		static Path EnsureStagingDirectoryExists(const Path& packageStore)
		{
			auto stagingDirectory = packageStore + Path(Constants::StagingFolderName);
			if (System::IFileSystem::Current().Exists(stagingDirectory))
			{
				Log::Warning("The staging directory already exists from a previous failed run.");
				System::IFileSystem::Current().DeleteDirectory(stagingDirectory, true);
			}

			// Create the folder
			System::IFileSystem::Current().CreateDirectory2(stagingDirectory);

			return stagingDirectory;
		}

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
