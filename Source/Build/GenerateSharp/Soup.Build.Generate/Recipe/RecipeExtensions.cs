// <copyright file="RecipeExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Utilities;
using System;
using System.Threading.Tasks;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The recipe extensions
	/// </summary>
	internal static class RecipeExtensions
	{
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		public static async Task<bool> TryLoadRecipeFromFileAsync(
			Path recipeFile,
			Recipe result)
		{
			// Verify the requested file exists
			Log.Diag("Load Recipe: " + recipeFile.ToString());
			if (!System.IO.File.Exists(recipeFile.ToString()))
			{
				Log.Info("Recipe file does not exist.");
				return false;
			}

			// Open the file to read from
			using (var fileStream = System.IO.File.OpenRead(recipeFile.ToString()))
			using (var reader = new System.IO.StreamReader(fileStream))
			{
				// Read the contents of the recipe file
				try
				{
					result = new Recipe(
						RecipeToml.Deserialize(
							recipeFile,
							await reader.ReadToEndAsync()));
					return true;
				}
				catch (Exception ex)
				{
					Log.Error($"Deserialize Threw: {ex.Message}");
					Log.Info("Failed to parse Recipe.");
					return false;
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
	}
}
