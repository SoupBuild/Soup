// <copyright company="Soup" file="BuildCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.Threading.Tasks;

	/// <summary>
	/// Build Command
	/// </summary>
	internal class BuildCommand : ICommand
	{
		public string Name => "build";

		public async Task InvokeAsync(string[] args)
		{
			Log.Message("Build");

			var recipe = await RecipeManager.LoadFromFileAsync(@".\");
			Log.Message(recipe.Name);

			var builder = new VisualStudioBuild.BuildRunner();
			builder.Build();
		}
	}
}
