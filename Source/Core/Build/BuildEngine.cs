// <copyright company="Soup" file="BuildEngine.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.IO;
	using System.Threading.Tasks;

	public class BuildEngine
	{
		private ICompiler _compiler;

		public BuildEngine(ICompiler compiler)
		{
			_compiler = compiler;
		}

		public async Task ExecuteAsync(Recipe recipe)
		{
			var args = new CompilerArguments()
			{
				RootDirectory = "./",
				ObjectDirectory = "obj",
				SourceFiles = recipe.Source
			};

			// Ensure the object directory exists
			var objectDirectry = Path.Combine(args.RootDirectory, args.ObjectDirectory);
			if (!Directory.Exists(objectDirectry))
			{
				Directory.CreateDirectory(objectDirectry);
			}

			await _compiler.ExecuteAsync(args);
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
