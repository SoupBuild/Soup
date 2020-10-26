// <copyright file="PackageManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "Api/SoupApi.h"
#include "Auth/SoupAuth.h"
#include "LzmaExtractCallback.h"
#include "LzmaUpdateCallback.h"
#include "LzmaOutStream.h"
#include "LzmaInStream.h"

namespace Soup
{
	class UserDetails
	{
	public:
		std::string UserName;
		std::string Password;
	};

	/// <summary>
	/// The package manager
	/// </summary>
	export class PackageManager
	{
	public:
		/// <summary>
		/// Install packages
		/// </summary>
		static void InstallPackages(const Path& workingDirectory)
		{
			auto packageStore = System::IFileSystem::Current().GetUserProfileDirectory() +
				Path(".soup/packages/");
			Log::Info("Using Package Store: " + packageStore.ToString());

			// Create the staging directory
			auto stagingPath = EnsureStagingDirectoryExists(packageStore);

			try
			{
				InstallRecursiveDependencies(
					workingDirectory,
					packageStore,
					stagingPath);

				// Cleanup the working directory
				Log::Info("Deleting staging directory");
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);
			}
			catch(const std::exception& e)
			{
				// Cleanup the staging directory and accept that we failed
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);
				throw;
			}
		}

		/// <summary>
		/// Install a package
		/// </summary>
		static void InstallPackageReference(const Path& workingDirectory, const std::string& packageReference)
		{
			auto recipePath =
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadRecipeFromFile(recipePath, recipe))
			{
				throw std::runtime_error("Could not load the recipe file.");
			}

			auto packageStore = System::IFileSystem::Current().GetUserProfileDirectory() +
				Path(".soup/packages/");
			Log::Info("Using Package Store: " + packageStore.ToString());

			// Create the staging directory
			auto stagingPath = EnsureStagingDirectoryExists(packageStore);

			try
			{
				// Parse the package reference to get the name
				PackageReference targetPackageReference = PackageReference::Parse(packageReference);
				std::string packageName = packageReference;
				if (!targetPackageReference.IsLocal())
				{
					packageName = targetPackageReference.GetName();
				}

				// Check if the package is already installed
				auto packageNameNormalized = ToUpper(packageName);
				if (recipe.HasRuntimeDependencies())
				{
					for (auto& dependency : recipe.GetRuntimeDependencies())
					{
						if (!dependency.IsLocal())
						{
							auto dependencyNameNormalized = ToUpper(dependency.GetName());
							if (dependencyNameNormalized == packageNameNormalized)
							{
								Log::Warning("Package already installed.");
								return;
							}
						}
					}
				}

				// Get the latest version if no version provided
				if (targetPackageReference.IsLocal())
				{
					auto packageModel = GetPackageModel(packageReference);
					auto latestVersion = packageModel.GetLatest();
					Log::HighPriority("Latest Version: " + latestVersion.ToString());
					targetPackageReference = PackageReference(packageModel.GetName(), latestVersion);
				}

				EnsurePackageDownloaded(
					targetPackageReference.GetName(),
					targetPackageReference.GetVersion(),
					packageStore,
					stagingPath);

				// Cleanup the working directory
				Log::Info("Deleting staging directory");
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);

				// Register the package in the recipe
				Log::Info("Adding reference to recipe");
				auto dependencies = std::vector<PackageReference>();
				if (recipe.HasRuntimeDependencies())
					dependencies = recipe.GetRuntimeDependencies();

				dependencies.push_back(targetPackageReference);
				recipe.SetRuntimeDependencies(dependencies);

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
		static void PublishPackage(const Path& workingDirectory)
		{
			Log::Info("Publish Project: {recipe.Name}@{recipe.Version}");

			auto recipePath =
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadRecipeFromFile(recipePath, recipe))
			{
				throw std::runtime_error("Could not load the recipe file.");
			}

			auto packageStore = System::IFileSystem::Current().GetUserProfileDirectory() +
				Path(".soup/packages/");
			Log::Info("Using Package Store: " + packageStore.ToString());

			// Create the staging directory
			auto stagingPath = EnsureStagingDirectoryExists(packageStore);

			try
			{
				auto archivePath = stagingPath + Path(recipe.GetName() + ".7z");
				auto files = GetPackageFiles(workingDirectory);

				// Create the archive of the package
				{
					auto archiveStream = System::IFileSystem::Current().OpenWrite(archivePath, true);
					auto outStream = std::make_shared<LzmaOutStream>(archiveStream);
					auto archive = LzmaSdk::ArchiveWriter(outStream);
					archive.AddFiles(files);

					auto callback = std::make_shared<LzmaUpdateCallback>(workingDirectory);
					archive.Save(callback);
				}

				// Authenticate the user
				Log::Info("Request Authentication Token");
				auto userDetails = GetUserDetails();
				auto openIdConfiguration = Api::SoupAuth::GetOpenIdConfiguration();
				auto tokenResult = Api::SoupAuth::RequestClientCredentialsToken(
					openIdConfiguration.TokenEndpoint,
					userDetails.UserName,
					userDetails.Password);
				switch (tokenResult.Status)
				{
					case Api::RequestClientCredentialsTokenResult::Success:
						// All good
						break;
					case Api::RequestClientCredentialsTokenResult::InvalidUserNameOrPassword:
						Log::HighPriority("Invalid UserName or Password");
						return;
					default:
						throw std::runtime_error("Unknown RequestClientCredentialsTokenResult");
				}

				// Publish the archive, scope cleanup file access
				{
					Log::Info("Publish package");
					auto archiveFile = System::IFileSystem::Current().OpenRead(archivePath, true);
					auto publishResult = Api::SoupApi::PublishPackage(
						tokenResult.Result,
						recipe.GetName(),
						recipe.GetVersion(),
						archiveFile->GetInStream());

					// Check if we should publish the package
					bool createPackageRequired = false;
					switch (publishResult)
					{
						case Api::PublishPackageResult::Success:
							// All Good
							Log::Info("Package version created");
							break;
						case Api::PublishPackageResult::PackageDoesNotExist:
							// Not found indicates the package does not exist, create it
							Log::HighPriority("The provided package name does not exist");
							createPackageRequired = true;
							break;
						case Api::PublishPackageResult::AlreadyExists:
							Log::HighPriority("A Package with this version already exists");
							break;
						case Api::PublishPackageResult::Forbidden:
							Log::HighPriority("User account forbidden to create package. Make sure you have verified your email.");
							break;
						default:
							throw std::runtime_error("Unknown PublishPackageResult");
					}

					// Create the package if needed and retry
					if (createPackageRequired)
					{
						// Create the package
						Log::HighPriority("Create package");
						auto createModel = Api::PackageCreateOrUpdateModel();
						auto createdPackage = Api::SoupApi::CreatePackage(
							tokenResult.Result,
							recipe.GetName(),
							createModel);
			
						switch (createdPackage.first)
						{
							case Api::CreatePackageResult::Success:
							{
								Log::Info("Package version created");

								// Retry
								Log::HighPriority("Retry publish package");

								// Reset the archive file
								auto& archiveStream = archiveFile->GetInStream();
								archiveStream.clear();
								archiveStream.seekg(0, std::ios::beg);

								publishResult = Api::SoupApi::PublishPackage(
									tokenResult.Result,
									recipe.GetName(),
									recipe.GetVersion(),
									archiveStream);
								switch (publishResult)
								{
									case Api::PublishPackageResult::Success:
										// All Good
										Log::Info("Package version created");
										break;
									default:
										throw std::runtime_error("Unknown PublishPackageResult");
								}
								break;
							}
							case Api::CreatePackageResult::Forbidden:
							{
								Log::HighPriority("User account forbidden to create package. Make sure you have verified your email.");
								break;
							}
							default:
							{
								throw std::runtime_error("Unknown PublishPackageResult");
							}
						}
					}
				}

				// Cleanup the staging directory
				Log::Info("Cleanup staging directory");
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);
			}
			catch(const std::exception& e)
			{
				// Cleanup the staging directory and accept that we failed
				Log::Info("Publish Failed: Cleanup staging directory");
				System::IFileSystem::Current().DeleteDirectory(stagingPath, true);
				throw;
			}
		}

	private:
		static UserDetails GetUserDetails()
		{
			auto userDetails = UserDetails();

			auto inputStream = IO::IConsoleManager::Current().GetStandardInput();

			Log::HighPriority("UserName:");
			userDetails.UserName = inputStream->ReadLine();

			Log::HighPriority("Password:");
			userDetails.Password = inputStream->ReadPassword();

			return userDetails;
		}

		static Soup::Api::PackageResultModel GetPackageModel(const std::string& name)
		{
			auto package = Api::SoupApi::GetPackage(name);
			return package;
		}

		static std::vector<LzmaSdk::FileProperties> GetPackageFiles(const Path& workingDirectory)
		{
			auto result = std::vector<LzmaSdk::FileProperties>();

			for (auto& child : System::IFileSystem::Current().GetDirectoryChildren(workingDirectory))
			{
				if (child.IsDirectory)
				{
					// Ignore output folder
					if (child.Path.GetFileName() != "out")
					{
						auto directoryFiles = GetAllFilesRecursive(child.Path, workingDirectory);
						result.insert(
							result.end(),
							directoryFiles.begin(),
							directoryFiles.end());
					}
				}
				else
				{
					auto relativePath = child.Path.GetRelativeTo(workingDirectory);
					auto fileProperties = LzmaSdk::FileProperties();
					fileProperties.Name = relativePath.ToString();
					fileProperties.Size = child.Size;
					fileProperties.CreateTime = child.CreateTime;
					fileProperties.AccessTime = child.AccessTime;
					fileProperties.ModifiedTime = child.ModifiedTime;
					fileProperties.Attributes = child.Attributes;
					result.push_back(std::move(fileProperties));
				}
			}

			return result;
		}

		static std::vector<LzmaSdk::FileProperties> GetAllFilesRecursive(const Path& directory, const Path& workingDirectory)
		{
			auto result = std::vector<LzmaSdk::FileProperties>();

			for (auto& child : System::IFileSystem::Current().GetDirectoryChildren(directory))
			{
				if (child.IsDirectory)
				{
					auto directoryFiles = GetAllFilesRecursive(child.Path, workingDirectory);
					result.insert(
						result.end(),
						directoryFiles.begin(),
						directoryFiles.end());
				}
				else
				{
					auto relativePath = child.Path.GetRelativeTo(workingDirectory);
					auto fileProperties = LzmaSdk::FileProperties();
					fileProperties.Name = relativePath.ToString();
					fileProperties.Size = child.Size;
					fileProperties.CreateTime = child.CreateTime;
					fileProperties.AccessTime = child.AccessTime;
					fileProperties.ModifiedTime = child.ModifiedTime;
					fileProperties.Attributes = child.Attributes;
					result.push_back(std::move(fileProperties));
				}
			}

			return result;
		}

		/// <summary>
		/// Ensure a package version is downloaded
		/// </summary>
		static void EnsurePackageDownloaded(
			const std::string packageName,
			SemanticVersion packageVersion,
			const Path& packagesDirectory,
			const Path& stagingDirectory)
		{
			Log::HighPriority("Install Package: " + packageName + "@" + packageVersion.ToString());

			auto packageRootFolder = packagesDirectory + Path(packageName);
			auto packageVersionFolder = packageRootFolder + Path(packageVersion.ToString()) + Path("/");

			// Check if the package version already exists
			if (System::IFileSystem::Current().Exists(packageVersionFolder))
			{
				Log::HighPriority("Found local version");
			}
			else
			{
				// Download the archive
				Log::HighPriority("Downloading package");
				auto archiveContent = Api::SoupApi::DownloadPackage(packageName, packageVersion);
				auto archivePath = stagingDirectory + Path(packageName + ".7z");

				// Write the contents to disk, scope cleanup
				auto archiveWriteFile = System::IFileSystem::Current().OpenWrite(archivePath, true);
				archiveWriteFile->GetOutStream() << archiveContent;
				archiveWriteFile->Close();

				// Create the package folder to extract to
				auto stagingVersionFolder = stagingDirectory + Path(packageName) + Path(packageVersion.ToString()) + Path("/");
				System::IFileSystem::Current().CreateDirectory2(stagingVersionFolder);

				// Unpack the contents of the archive
				{
					auto archiveStream = System::IFileSystem::Current().OpenRead(archivePath, true);
					auto inStream = std::make_shared<LzmaInStream>(archiveStream);
					auto archive = LzmaSdk::ArchiveReader(inStream);
					auto callback = std::make_shared<LzmaExtractCallback>();
					archive.ExtractAll(stagingVersionFolder.ToString(), callback);
				}

				// Install transitive dependencies
				InstallRecursiveDependencies(
					stagingVersionFolder,
					packagesDirectory,
					stagingDirectory);

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

		/// <summary>
		/// Recursively install all dependencies and transitive dependencies
		/// </summary>
		static void InstallRecursiveDependencies(
			const Path& recipeDirectory,
			const Path& packagesDirectory,
			const Path& stagingDirectory)
		{
			auto recipePath =
				recipeDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadRecipeFromFile(recipePath, recipe))
			{
				throw std::runtime_error("Could not load the recipe file.");
			}
	
			if (recipe.HasRuntimeDependencies())
			{
				for (auto& dependency : recipe.GetRuntimeDependencies())
				{
					// If local then check children for external package references
					// Otherwise install the external package reference and its dependencies
					if (dependency.IsLocal())
					{
						auto dependencyPath = recipeDirectory + dependency.GetPath();
						InstallRecursiveDependencies(
							dependencyPath,
							packagesDirectory,
							stagingDirectory);
					}
					else
					{
						EnsurePackageDownloaded(
							dependency.GetName(),
							dependency.GetVersion(),
							packagesDirectory,
							stagingDirectory);
					}
				}
			}

			if (recipe.HasBuildDependencies())
			{
				for (auto& dependency : recipe.GetBuildDependencies())
				{
					// If local then check children for external package references
					// Otherwise install the external package reference and its dependencies
					if (dependency.IsLocal())
					{
						auto dependencyPath = recipeDirectory + dependency.GetPath();
						InstallRecursiveDependencies(
							dependencyPath,
							packagesDirectory,
							stagingDirectory);
					}
					else
					{
						EnsurePackageDownloaded(
							dependency.GetName(),
							dependency.GetVersion(),
							packagesDirectory,
							stagingDirectory);
					}
				}
			}
		}

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

		/// <summary>
		/// Ensure the staging directory exists
		/// </summary>
		static Path EnsureStagingDirectoryExists(const Path& packageStore)
		{
			auto stagingDirectory = packageStore + Path(Constants::StagingFolderName);
			if (System::IFileSystem::Current().Exists(stagingDirectory))
			{
				Log::Warning("The staging directory already exists from a previous failed run");
				System::IFileSystem::Current().DeleteDirectory(stagingDirectory, true);
			}

			// Create the folder
			System::IFileSystem::Current().CreateDirectory2(stagingDirectory);

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
