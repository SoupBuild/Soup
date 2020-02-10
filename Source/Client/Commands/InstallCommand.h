// <copyright file="InstallCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "InstallOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Install Command
	/// </summary>
	class InstallCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="InstallCommand"/> class.
		/// </summary>
		InstallCommand(InstallOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("InstallCommand::Run");
			
		}

	private:
		SemanticVersion GetLatestAsync(string name)
		{
			 var package = await _soupApi.GetPackageAsync(name);
			 return package.Latest;
		}

		Stream DownloadPackageAsync(string name, SemanticVersion version)
		{
			 var stream = await _soupApi.DownloadPackageAsync(name, version);
			 return stream;
		}

		Recipe InstallPackageAsync(string tempPath, Stream packageFile)
		{
			 var recipe = await UnpackArchiveAsync(tempPath, packageFile);
			 return recipe;
		}

		Recipe UnpackArchiveAsync(string tempPath, Stream packageFile)
		{
			 // Unpack the zip file into the package directory
			 var tempPackagePath = Path.Combine(tempPath, Constants.StorePackageFolderName);
			 Directory.CreateDirectory(tempPackagePath);
			 await PackageManager.ExtractAsync(packageFile, tempPackagePath);

			 // Load the packages recipe file
			 var recipe = await RecipeManager.LoadFromFileAsync(tempPackagePath);
			 var packagePath = PackageManager.BuildKitchenPackagePath(_config, recipe);

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

		/// <summary>
		/// Recursively install all dependencies and transitive dependencies
		/// </summary>
		void InstallRecursiveDependencies(string tempPath, Recipe recipe)
		{
			 foreach (var dep in recipe.Dependencies)
			 {
				 Log.Info($"{dep}");

				 // Download the archive
				 using (var archiveStream = await DownloadPackageAsync(dep.Name, dep.Version))
				 {
					 // Install the package
					 var installedRecipe = await InstallPackageAsync(tempPath, archiveStream);

					 // Install dependecies recursively
					 await InstallRecursiveDependencies(tempPath, installedRecipe);
				 }
			 }
		}

	private:
		InstallOptions _options;
	};
}
