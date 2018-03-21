// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Threading.Tasks;

namespace Soup.Client
{
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
