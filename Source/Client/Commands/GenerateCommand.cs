// <copyright company="Soup" file="GenerateCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.IO;
	using System.Threading.Tasks;

	/// <summary>
	/// Generate Command
	/// </summary>
	internal class GenerateCommand : ICommand
	{
		public string Name => "generate";

		public async Task InvokeAsync(string[] args)
		{
			var projectDirectory = Directory.GetCurrentDirectory();
			var buildPath = Path.Combine(
				projectDirectory,
				Constants.ProjectGenerateFolderName,
				Constants.StoreBuildFolderName);
			var recipe = await RecipeManager.LoadFromFileAsync(projectDirectory);
			if (recipe == null)
			{
				Log.Error("Could not find the recipe file.");
				return;
			}

			Log.Message($"Generate {recipe.Type}");

			// Ensure the project folder exists
			PackageManager.EnsureProjectGenerateFolderExists(projectDirectory);
			if (!Directory.Exists(buildPath))
			{
				Directory.CreateDirectory(buildPath);
			}

			// Generate the project files
			var objectDirectory = Path.Combine(buildPath, "out", "obj");
			var binaryDirectory = Path.Combine(buildPath, "out", "bin");
			var buildEngine = Singleton<IBuildEngine>.Instance;
			await buildEngine.GenerateDependenciesAsync(
				recipe,
				buildPath);
			await buildEngine.GenerateBuildAsync(
				recipe,
				buildPath,
				buildPath,
				projectDirectory,
				binaryDirectory,
				objectDirectory);
		}
	}
}
