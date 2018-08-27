// <copyright company="Soup" file="PackCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.IO;
	using System.Threading.Tasks;

	/// <summary>
	/// Pack Command
	/// </summary>
	internal class PackCommand
	{
		public async Task InvokeAsync(PackOptions options)
		{
			var recipe = await RecipeManager.LoadFromFileAsync(@"./");
			Log.Message($"Packaging Project: {recipe.Name}@{recipe.Version}");

			await PackageManager.PackAsync(recipe, Directory.GetCurrentDirectory());
		}
	}
}
