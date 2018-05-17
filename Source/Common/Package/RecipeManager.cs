// <copyright company="Soup" file="RecipeManager.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.IO;
	using System.Text;
	using System.Threading.Tasks;
	using Newtonsoft.Json;

	/// <summary>
	/// The recipe manager
	/// </summary>
	public static class RecipeManager
	{
		/// <summary>
		/// Load the recipe from the root file
		/// </summary>
		public static async Task<Recipe> LoadFromFileAsync(string path)
		{
			Recipe result = null;
			var recipePath = Path.Combine(path, Constants.RecipeFileName);
			if (File.Exists(recipePath))
			{
				using (var file = File.OpenRead(recipePath))
				{
					result = await LoadFromStreamAsync(file);
				}
			}

			return result;
		}

		/// <summary>
		/// Load from stream async
		/// </summary>
		public static async Task<Recipe> LoadFromStreamAsync(Stream stream)
		{
			Recipe result = null;
			using (var reader = new StreamReader(stream, Encoding.UTF8, false, 2048, true))
			{
				// Read the contents of the recipe file
				var content = await reader.ReadToEndAsync();
				result = JsonConvert.DeserializeObject<Recipe>(content);
			}

			return result;
		}

		/// <summary>
		/// Save the recipe to the root file
		/// </summary>
		public static async Task SaveToFileAsync(Recipe recipe)
		{
			// Serialize the contents of the recipe
			var content = JsonConvert.SerializeObject(
				recipe,
				Formatting.Indented,
				new JsonSerializerSettings() { DefaultValueHandling = DefaultValueHandling.Ignore });

			// Replace the contents of the file
			var recipePath = Path.Combine(Directory.GetCurrentDirectory(), Constants.RecipeFileName);
			using (var writer = new StreamWriter(File.OpenWrite(recipePath)))
			{
				await writer.WriteAsync(content);
			}
		}
	}
}
