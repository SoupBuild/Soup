// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.IO;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;

namespace Soup
{
	class PublishCommand : ICommand
	{
		public string Name => "publish";

		public async Task InvokeAsync(string[] args, LocalUserConfig userConfig)
		{
			var projectDirectory = Directory.GetCurrentDirectory();
			var recipe = await RecipeManager.LoadFromFileAsync(projectDirectory);
			Log.Message($"Publish Project: {recipe.Name}@{recipe.Version}");

			using (var stream = new MemoryStream())
			{
				// Pack the project into a memory stream
				await PackageManager.PackAsync(recipe, projectDirectory, stream);

				// Reset the stream so we can read from it
				stream.Seek(0, SeekOrigin.Begin);

				// Publish the package to the service
				try
				{
					var result = await SoupRESTApi.PublishAsync(stream);
					if (!result.IsSuccessStatusCode)
					{
						switch (result.StatusCode)
						{
							case HttpStatusCode.Conflict:
								Log.Error("The Package version already exists.");
								break;
							default:
								Log.Error("Failed");
								break;
						}
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
