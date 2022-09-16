// <copyright file="RecipeExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System;
using System.Threading.Tasks;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The recipe extensions
	/// </summary>
	public static class RecipeExtensions
	{
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		public static async Task<(bool IsSuccess, Recipe Result)> TryLoadRecipeFromFileAsync(Path recipeFile)
		{
			// Verify the requested file exists
			Log.Diag("Load Recipe: " + recipeFile.ToString());
			if (!System.IO.File.Exists(recipeFile.ToString()))
			{
				Log.Info("Recipe file does not exist.");
				return (false, new Recipe());
			}

			// Open the file to read from
			using (var fileStream = System.IO.File.OpenRead(recipeFile.ToString()))
			using (var reader = new System.IO.StreamReader(fileStream))
			{
				// Read the contents of the recipe file
				try
				{
					var content = await reader.ReadToEndAsync();
					var result = SMLManager.Deserialize(content);

					return (true, new Recipe(result));
				}
				catch (Exception ex)
				{
					Log.Error($"Deserialize Threw: {ex.Message}");
					Log.Info("Failed to parse Recipe.");
					return (false, new Recipe());
				}
			}
		}

		/// <summary>
		/// Build up the recipe file location from the package reference
		/// </summary>
		public static Path GetPackageRecipeFile(
			Path workingDirectory,
			PackageReference package)
		{
			if (package.IsLocal)
			{
				var recipeFile = workingDirectory + package.Path;
				return recipeFile;
			}
			else
			{
				throw new InvalidOperationException("Non-local packages not supported.");
			}
		}

		/// <summary>
		/// Get the package reference path
		/// </summary>
		public static Path GetPackageReferencePath(
			Path workingDirectory,
			PackageReference reference)
		{
			// If the path is relative then combine with the working directory
			var packagePath = reference.Path;
			if (!packagePath.HasRoot)
			{
				packagePath = workingDirectory + packagePath;
			}

			return packagePath;
		}

		/// <summary>
		/// Save the recipe to file
		/// </summary>
		public static async Task SaveToFileAsync(
			Path recipeFile,
			Recipe recipe)
		{
			// Open the file to write to
			var file = LifetimeManager.Get<IFileSystem>().OpenWrite(recipeFile, true);

			// Write the recipe to the file stream
			await SMLManager.SerializeAsync(
				new SMLDocument(recipe.Table),
				file.GetOutStream());
		}
	}
}
