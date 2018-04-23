// <copyright company="Soup" file="PackageManager.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Collections.Generic;
	using System.IO;
	using System.IO.Compression;
	using System.Linq;
	using System.Text;
	using System.Threading.Tasks;
	using ICSharpCode.SharpZipLib.Tar;

	/// <summary>
	/// The package manager
	/// </summary>
	public static class PackageManager
	{
		public static string BuildKitchenLibraryPath()
		{
			return Path.Combine(
				Singleton<LocalUserConfig>.Instance.PackageStore,
				Constants.StoreLibraryFolderName);
		}

		public static string BuildKitchenBuildPath(string buildsystem, Recipe recipe)
		{
			return BuildKitchenBuildPath(buildsystem, recipe.Name, recipe.Version);
		}

		public static string BuildKitchenBuildPath(string buildsystem, PackageReference reference)
		{
			return BuildKitchenBuildPath(buildsystem, reference.Name, reference.Version);
		}

		public static string BuildKitchenPackagePath(Recipe recipe)
		{
			return BuildKitchenPackagePath(recipe.Name, recipe.Version);
		}

		public static string BuildKitchenPackagePath(PackageReference reference)
		{
			return BuildKitchenPackagePath(reference.Name, reference.Version);
		}

		public static string BuildKitchenIncludePath(Recipe recipe)
		{
			return BuildKitchenIncludePath(recipe.Name, recipe.Version);
		}

		public static string BuildKitchenIncludePath(PackageReference reference)
		{
			return BuildKitchenIncludePath(reference.Name, reference.Version);
		}

		public static string EnsureStagingDirectoryExists(string directory)
		{
			var path = Path.Combine(directory, Constants.StagingFolderName);
			if (!Directory.Exists(path))
			{
				// Create the folder
				var info = Directory.CreateDirectory(path);

				// Hide the folder
				info.Attributes |= FileAttributes.Hidden;
			}

			return path;
		}

		public static void EnsureProjectGenerateFolderExists(string directory)
		{
			var path = Path.Combine(directory, Constants.ProjectGenerateFolderName);
			if (!Directory.Exists(path))
			{
				// Create the folder
				var info = Directory.CreateDirectory(path);

				// Hide the folder
				info.Attributes |= FileAttributes.Hidden;
			}
		}

		public static List<string> FindSourceFiles(Recipe recipe, string packageDirectory)
		{
			return FindFiles(recipe.Source, packageDirectory);
		}

		public static List<string> FindPublicFiles(Recipe recipe, string packageDirectory)
		{
			// TODO : Verify they are all headers
			return FindFiles(recipe.Public, packageDirectory);
		}

		public static List<string> FindFiles(IList<string> patterns, string directory)
		{
			List<string> result = new List<string>();

			// Create matching patterns for each source items
			var includePatterns = new List<Glob.Glob>();
			foreach (var pattern in patterns)
			{
				var cleanPattern = pattern.Replace("/", "\\").ToLower();
				includePatterns.Add(new Glob.Glob(cleanPattern));
			}

			// Check every file in the directory
			foreach (var file in Directory.EnumerateFiles(directory, "*", SearchOption.AllDirectories))
			{
				var relativePath = Path.GetRelativePath(directory, file);
				var cleanRelativePath = relativePath.ToLower();
				bool match = includePatterns.Any(pattern => pattern.IsMatch(cleanRelativePath));
				if (match)
				{
					result.Add(relativePath);
				}
			}

			return result;
		}

		/// <summary>
		/// Pack the archive
		/// </summary>
		public static async Task PackAsync(Recipe recipe, string directory)
		{
			var zipFileName = $"{recipe.Name}_{recipe.Version}.tgz";
			var zipFilePath = Path.Combine(directory, zipFileName);
			using (var zipFile = File.Create(zipFilePath))
			{
				await PackAsync(recipe, directory, zipFile);
			}
		}

		/// <summary>
		/// Pack the archive
		/// </summary>
		public static Task PackAsync(Recipe recipe, string directory, Stream stream)
		{
			var includePatterns = new List<Glob.Glob>();

			// Include the Recipe file
			includePatterns.Add(new Glob.Glob(Constants.RecipeFileName.ToLower()));

			// Include all or the source filess
			foreach (var source in recipe.Source)
			{
				includePatterns.Add(new Glob.Glob(source.ToLower()));
			}

			using (var gzipStream = new GZipStream(stream, CompressionLevel.Optimal, true))
			using (var archive = TarArchive.CreateOutputTarArchive(gzipStream))
			{
				archive.RootPath = directory;

				// Check every file in the directory
				foreach (var file in Directory.EnumerateFiles(directory, "*", SearchOption.AllDirectories))
				{
					var relativePath = Path.GetRelativePath(directory, file);
					bool matchInclude = includePatterns.Any(pattern => pattern.IsMatch(relativePath.ToLower()));
					if (matchInclude)
					{
						Log.Verbose(relativePath);
						var entry = TarEntry.CreateEntryFromFile(file);
						archive.WriteEntry(entry, true);
					}
				}
			}

			return Task.CompletedTask;
		}

		/// <summary>
		/// Unpack the archive
		/// </summary>
		public static Task ExtractAsync(Stream source, string targetDirectory)
		{
			using (var gzipStream = new GZipStream(source, CompressionMode.Decompress))
			using (TarArchive archive = TarArchive.CreateInputTarArchive(gzipStream))
			{
				archive.ExtractContents(targetDirectory);
			}

			return Task.CompletedTask;
		}

		/// <summary>
		/// Verify the archive
		/// </summary>
		public static async Task<Recipe> VerifyArchiveAsync(Stream stream)
		{
			using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress, true))
			using (var archive = new TarInputStream(gzipStream))
			{
				Recipe recipe = null;
				TarEntry entry = archive.GetNextEntry();
				while (entry != null)
				{
					if (string.Compare(entry.Name, Constants.RecipeFileName, true) == 0)
					{
						recipe = await RecipeManager.LoadFromStreamAsync(archive);
						break;
					}

					entry = archive.GetNextEntry();
				}

				return recipe;
			}
		}

		/// <summary>
		/// Create the public include header file
		/// </summary>
		public static async Task CreatePublicIncludeHeaderAsync(Recipe recipe, string packageDirectory, string targetDirectory)
		{
			// Get the relative path back to the package directory
			// this is used to reference source files
			var packageRelativePath = Path.GetRelativePath(targetDirectory, packageDirectory);

			StringBuilder content = new StringBuilder();
			foreach (var file in FindPublicFiles(recipe, packageDirectory))
			{
				var fileRelativePath = Path.Combine(packageRelativePath, file);
				content.Append($"#include \"{fileRelativePath}\"");
			}

			var includeHeaderFileName = $"{recipe.Name}.h";
			var includeHeaderPath = Path.Combine(targetDirectory, includeHeaderFileName);
			await File.WriteAllTextAsync(includeHeaderPath, content.ToString());
		}

		private static string BuildPackageVersionDirectory(string projectName, SemanticVersion version)
		{
			return Path.Combine(projectName, $"{version}");
		}

		private static string BuildKitchenPackagePath(string projectName, SemanticVersion version)
		{
			var kitchenPath = Singleton<LocalUserConfig>.Instance.PackageStore;
			var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
			var path = Path.Combine(kitchenPath, Constants.StorePackageFolderName, packageVersionDirectory);
			return path;
		}

		private static string BuildKitchenBuildPath(string buildSystem, string projectName, SemanticVersion version)
		{
			var kitchenPath = Singleton<LocalUserConfig>.Instance.PackageStore;
			var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
			var path = Path.Combine(kitchenPath, Constants.StoreBuildFolderName, buildSystem, packageVersionDirectory);
			return path;
		}

		private static string BuildKitchenIncludePath(string projectName, SemanticVersion version)
		{
			var kitchenPath = Singleton<LocalUserConfig>.Instance.PackageStore;
			var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
			var path = Path.Combine(kitchenPath, Constants.StoreIncludeFolderName, packageVersionDirectory);
			return path;
		}
	}
}
