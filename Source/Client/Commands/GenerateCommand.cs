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
			Log.Message("Generate");
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

			// Ensure the project folder exists
			PackageManager.EnsureProjectGenerateFolderExists(projectDirectory);
			if (!Directory.Exists(buildPath))
			{
				Directory.CreateDirectory(buildPath);
			}

			// Generate the project files
			var buildGenerator = Singleton<IBuildGenerator>.Instance;
			await buildGenerator.GenerateDependenciesAsync(
				recipe,
				buildPath);
			await buildGenerator.GenerateBuildAsync(
				recipe,
				buildPath,
				projectDirectory,
				@"$(PackageRoot)\out\bin",
				@"$(PackageRoot)\out\obj");
		}
	}
}
