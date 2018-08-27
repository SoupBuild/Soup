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
	internal class BuildCommand
	{
		public async Task InvokeAsync(BuildOptions Options)
		{
			var recipe = await RecipeManager.LoadFromFileAsync(@"./");
			if (recipe == null)
			{
				Log.Error("Could not find the recipe file.");
				return;
			}

			var compiler = Singleton<Compiler.ICompiler>.Instance;

			// Ensure the library directory exists
			var libraryPath = PackageManager.BuildKitchenLibraryPath();
			if (!Directory.Exists(libraryPath))
			{
				Directory.CreateDirectory(libraryPath);
			}

			// Now build the current project
			Log.Message("");
			Log.Message("Building Project");
			var buildPath = Path.Combine(Constants.ProjectGenerateFolderName, Constants.StoreBuildFolderName);
			var buildEngine = new BuildEngine(compiler);
			await buildEngine.ExecuteAsync(recipe);
		}
	}
}
