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
	internal class PackCommand : ICommand
	{
		public string Name => "pack";

		public async Task InvokeAsync(string[] args)
		{
			var recipe = await RecipeManager.LoadFromFileAsync(@"./");
			Log.Message($"Packaging Project: {recipe.Name}@{recipe.Version}");

			await PackageManager.PackAsync(recipe, Directory.GetCurrentDirectory());
		}
	}
}
