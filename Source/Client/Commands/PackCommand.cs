// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.IO;
using System.Threading.Tasks;

namespace Soup.Client
{
	internal class PackCommand : ICommand
	{
		public string Name => "pack";

		public async Task InvokeAsync(string[] args, LocalUserConfig userConfig)
		{
			var recipe = await RecipeManager.LoadFromFileAsync(@".\");
			Log.Message($"Packaging Project: {recipe.Name}@{recipe.Version}");

			await PackageManager.PackAsync(recipe, Directory.GetCurrentDirectory());
		}
	}
}
