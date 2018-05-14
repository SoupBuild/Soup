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
			var builder = new MSBuild.BuildRunner();

			// Ensure the library directory exists
			var libraryPath = PackageManager.BuildKitchenLibraryPath();
			if (!Directory.Exists(libraryPath))
			{
				Directory.CreateDirectory(libraryPath);
			}

			// Ensure all of the depencies are built first
			Log.Message("Building Dependencies...");
			await BuildAllDependenciesRecursivelyAsync(recipe, builder);

			// Now build the current project
			Log.Message("");
			Log.Message("Building Project");
			var buildPath = Path.Combine(Constants.ProjectGenerateFolderName, Constants.StoreBuildFolderName);
			builder.Build(buildPath, true, true);
		}

		/// <summary>
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		private async Task BuildAllDependenciesRecursivelyAsync(Recipe recipe, IBuildRunner builder)
		{
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				var packagePath = PackageManager.BuildKitchenPackagePath(dependecy);
				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				// Build all recursive dependencies
				await BuildAllDependenciesRecursivelyAsync(dependecyRecipe, builder);

				// Build this dependecy
				Log.Message($"Building {dependecy}");
				var buildPath = PackageManager.BuildKitchenBuildPath("MSBuild", dependecy);
				if (!builder.Build(buildPath, false, true) ||
					!builder.Build(buildPath, false, false))
				{
					Log.Error("Build failed!");
				}
			}
		}
	}
}
