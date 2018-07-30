// <copyright company="Soup" file="PublishCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.IO;
	using System.Net.Http;
	using System.Threading.Tasks;
	using IdentityModel.Client;
	using Soup.Api;

	/// <summary>
	/// Publish Command
	/// </summary>
	internal class PublishCommand : ICommand
	{
		public string Name => "publish";

		public async Task InvokeAsync(string[] args)
		{
			var projectDirectory = Directory.GetCurrentDirectory();
			var recipe = await RecipeManager.LoadFromFileAsync(projectDirectory);
			Log.Message($"Publish Project: {recipe.Name}@{recipe.Version}");

			var result = await Singleton<ISoupIdentity>.Instance.AuthenticateUserAsync();

			using (var stream = new MemoryStream())
			{
				// Pack the project into a memory stream
				await PackageManager.PackAsync(recipe, projectDirectory, stream);

				// Reset the stream so we can read from it
				stream.Seek(0, SeekOrigin.Begin);

				// Publish the package to the service
				try
				{
					bool created = await Singleton<ISoupApi>.Instance.PublishPackageAsync(recipe.Name, stream);
					if (!created)
					{
						Log.Warning("The package version already existed! No change was made.");
					}
				}
				catch (HttpRequestException ex)
				{
					Log.Error($"Failed request: {ex.ToString()}");
				}
			}
		}
	}
}
