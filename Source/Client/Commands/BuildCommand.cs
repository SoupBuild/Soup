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
		private LocalUserConfig _config;
		private Compiler.ICompiler _compiler;

		public BuildCommand(LocalUserConfig config, Compiler.ICompiler compiler)
		{
			_config = config;
			_compiler = compiler;
		}

		public async Task InvokeAsync(BuildOptions Options)
		{
			var recipe = await RecipeManager.LoadFromFileAsync(@"./");
			if (recipe == null)
			{
				Log.Error("Could not find the recipe file.");
				return;
			}

			// Ensure the library directory exists
			var libraryPath = PackageManager.BuildKitchenLibraryPath(_config);
			if (!Directory.Exists(libraryPath))
			{
				Directory.CreateDirectory(libraryPath);
			}

			// Now build the current project
			Log.Info("");
			Log.Info("Building Project");
			var buildPath = Path.Combine(Constants.ProjectGenerateFolderName, Constants.StoreBuildFolderName);
			var buildEngine = new BuildEngine(_compiler);
			await buildEngine.ExecuteAsync(recipe);
		}
	}
}
