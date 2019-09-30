// <copyright file="PackageManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
	/// <summary>
	/// The package manager
	/// </summary>
	export class PackageManager
	{
	public:
		/// <summary>
		/// Build the namespace version
		/// </summary>
		static std::string BuildNamespaceVersion(SemanticVersion version)
		{
			return $"v{version.Major}_{version.Minor}_{version.Patch}";
		}

		/// <summary>
		/// Build the kitchen library path
		/// </summary>
		public static string BuildKitchenLibraryPath(LocalUserConfig config)
		{
			return Path.Combine(
				config.PackageStore,
				Constants.StoreLibraryFolderName);
		}

		/// <summary>
		/// Build the kitchen build path
		/// </summary>
		public static string BuildKitchenBuildPath(LocalUserConfig config, Recipe recipe)
		{
			return BuildKitchenBuildPath(config, recipe.Name, recipe.Version);
		}

		/// <summary>
		/// Build the kitchen build path
		/// </summary>
		public static string BuildKitchenBuildPath(LocalUserConfig config, PackageReference reference)
		{
			return BuildKitchenBuildPath(config, reference.Name, reference.Version);
		}

		/// <summary>
		/// Build the kitchen package path
		/// </summary>
		public static string BuildKitchenPackagePath(LocalUserConfig config, Recipe recipe)
		{
			return BuildKitchenPackagePath(config, recipe.Name, recipe.Version);
		}

		/// <summary>
		/// Build the kitchen package path
		/// </summary>
		public static string BuildKitchenPackagePath(LocalUserConfig config, PackageReference reference)
		{
			return BuildKitchenPackagePath(config, reference.Name, reference.Version);
		}

		/// <summary>
		/// Build the kitchen package path
		/// </summary>
		public static string BuildKitchenIncludePath(LocalUserConfig config, Recipe recipe)
		{
			return BuildKitchenIncludePath(config, recipe.Name, recipe.Version);
		}

		/// <summary>
		/// Build the kitchen include path
		/// </summary>
		public static string BuildKitchenIncludePath(LocalUserConfig config, PackageReference reference)
		{
			return BuildKitchenIncludePath(config, reference.Name, reference.Version);
		}

		/// <summary>
		/// Build the recursive dependencies
		/// </summary>
		public static async Task<List<PackageReference>> BuildRecursiveDependeciesAsync(LocalUserConfig config, Recipe recipe)
		{
			List<PackageReference> result = new List<PackageReference>();
			foreach (var dependency in recipe.Dependencies)
			{
				result.Add(dependency);
				var dependencyPackagePath = BuildKitchenPackagePath(config, dependency);
				var dependencyRecipe = await RecipeManager.LoadFromFileAsync(dependencyPackagePath);
				var transientDependencies = await BuildRecursiveDependeciesAsync(config, dependencyRecipe);
				result.AddRange(transientDependencies);
			}

			return result;
		}

		/// <summary>
		/// Ensure the staging directory exists
		/// </summary>
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

		/// <summary>
		/// Ensure the project generate folder exists
		/// </summary>
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

		/// <summary>
		/// Find the source files
		/// </summary>
		public static List<string> FindSourceFiles(Recipe recipe, string packageDirectory)
		{
			return FindFiles(recipe.Source, packageDirectory);
		}

		/// <summary>
		/// Find the files
		/// </summary>
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

		private static string BuildPackageVersionDirectory(string projectName, SemanticVersion version)
		{
			return Path.Combine(projectName, $"{version}");
		}

		private static string BuildKitchenPackagePath(LocalUserConfig config, string projectName, SemanticVersion version)
		{
			var kitchenPath = config.PackageStore;
			var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
			var path = Path.Combine(kitchenPath, Constants.StorePackageFolderName, packageVersionDirectory);
			return path;
		}

		private static string BuildKitchenBuildPath(LocalUserConfig config, string projectName, SemanticVersion version)
		{
			var kitchenPath = config.PackageStore;
			var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
			var path = Path.Combine(kitchenPath, Constants.StoreBuildFolderName, packageVersionDirectory);
			return path;
		}

		private static string BuildKitchenIncludePath(LocalUserConfig config, string projectName, SemanticVersion version)
		{
			var kitchenPath = config.PackageStore;
			var packageVersionDirectory = BuildPackageVersionDirectory(projectName, version);
			var path = Path.Combine(kitchenPath, Constants.StoreIncludeRootFolderName, packageVersionDirectory);
			return path;
		}
	};
}
