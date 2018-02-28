// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.IO;
using System.Threading.Tasks;

namespace Soup
{
	class GenerateCommand : ICommand
	{
		public string Name => "generate";

		public async Task InvokeAsync(string[] args, LocalUserConfig userConfig)
		{
			Log.Message("Generate");
			var projectDirectory = Directory.GetCurrentDirectory();
			var buildPath = Path.Combine(
				projectDirectory,
				Constants.ProjectGenerateFolderName,
				Constants.BuildFolderName);
			var recipe = await RecipeManager.LoadFromFileAsync(projectDirectory);

			// Ensure the project folder exists
			PackageManager.EnsureProjectGenerateFolderExists(projectDirectory);
			if (!Directory.Exists(buildPath))
			{
				Directory.CreateDirectory(buildPath);
			}

			// Generate the project files
			var buildGenerator = new VisualStudioBuild.BuildGenerator();
			buildGenerator.GenerateDependencies(userConfig, recipe, projectDirectory, buildPath, @"$(PackageRoot)\out\");
			buildGenerator.GenerateBuild(recipe, projectDirectory, buildPath);
		}
	}
}
