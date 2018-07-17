// <copyright company="Soup" file="BuildCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.IO;
	using System.Threading.Tasks;

	/// <summary>
	/// Build Command
	/// </summary>
	internal class BuildCommand : ICommand
	{
		public string Name => "build";

		public async Task InvokeAsync(string[] args)
		{
			var recipe = await RecipeManager.LoadFromFileAsync(@"./");
			if (recipe == null)
			{
				Log.Error("Could not find the recipe file.");
				return;
			}

			var buildEngine = Singleton<IBuildEngine>.Instance;

			// Ensure the library directory exists
			var libraryPath = PackageManager.BuildKitchenLibraryPath();
			if (!Directory.Exists(libraryPath))
			{
				Directory.CreateDirectory(libraryPath);
			}

			// Ensure all of the depencies are built first
			Log.Message("Building Dependencies...");
			await BuildAllDependenciesRecursivelyAsync(recipe, buildEngine);

			// Now build the current project
			Log.Message("");
			Log.Message("Building Project");
			var buildPath = Path.Combine(Constants.ProjectGenerateFolderName, Constants.StoreBuildFolderName);
			buildEngine.Build(buildPath, true, true);
		}

		/// <summary>
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		private async Task BuildAllDependenciesRecursivelyAsync(Recipe recipe, IBuildEngine buildEngine)
		{
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				var packagePath = PackageManager.BuildKitchenPackagePath(dependecy);
				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				// Build all recursive dependencies
				await BuildAllDependenciesRecursivelyAsync(dependecyRecipe, buildEngine);

				// Build this dependecy
				Log.Message($"Building {dependecy}");
				var buildPath = PackageManager.BuildKitchenBuildPath(buildEngine.Name, dependecy);
				if (!buildEngine.Build(buildPath, false, true) ||
					!buildEngine.Build(buildPath, false, false))
				{
					Log.Error("Build failed!");
				}
			}
		}
	}
}
