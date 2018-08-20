// <copyright company="Soup" file="BuildEngine.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Threading.Tasks;

	public class BuildEngine
	{
		public Task ExecuteAsync(Recipe recipe)
		{
			return Task.CompletedTask;
		}

		/// <summary>
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		private async Task BuildAllDependenciesRecursivelyAsync(Recipe recipe)
		{
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				var packagePath = PackageManager.BuildKitchenPackagePath(dependecy);
				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				// Build all recursive dependencies
				await BuildAllDependenciesRecursivelyAsync(dependecyRecipe);

				// Build this dependecy
				Log.Message($"Building {dependecy}");
				var buildPath = PackageManager.BuildKitchenBuildPath(dependecy);
				Build();
			}
		}

		private void Build()
		{

		}
	}
}
