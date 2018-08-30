// <copyright company="Soup" file="BuildEngine.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;
	using System.Collections.Generic;
	using System.IO;
	using System.Linq;
	using System.Threading.Tasks;
	using Soup.Compiler;

	public class BuildEngine
	{
		private LocalUserConfig _config;
		private ICompiler _compiler;

		/// <summary>
		/// Initializes a new instance of the <see cref="BuildEngine"/> class.
		/// </summary>
		public BuildEngine(LocalUserConfig config, ICompiler compiler)
		{
			_config = config;
			_compiler = compiler;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public async Task ExecuteAsync(string path, Recipe recipe)
		{
			Log.Info("Build Recursive Dependencies.");
			await BuildAllDependenciesRecursivelyAsync(path, recipe);

			Log.Info("Build Toplevel Recipe.");
			await CoreBuildAsync(path, recipe);
		}

		/// <summary>
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		private async Task BuildAllDependenciesRecursivelyAsync(
			string path,
			Recipe recipe)
		{
			Log.Info($"Seaching Dependencies: {recipe.Name}.");
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				string packagePath;
				if (dependecy.Path != null)
				{
					packagePath = dependecy.Path;
				}
				else
				{
					packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
				}

				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				// Build all recursive dependencies
				await BuildAllDependenciesRecursivelyAsync(packagePath, dependecyRecipe);

				// Build this dependecy
				await CoreBuildAsync(packagePath, dependecyRecipe);
			}
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public async Task CoreBuildAsync(string path, Recipe recipe)
		{
			var objectDirectory = "obj";
			var binaryDirectory = "bin";

			Log.Info($"Building {recipe.Name}.");
			if (recipe.Type == RecipeType.Library)
			{
				await CompileModuleAsync(path, recipe, objectDirectory);
			}

			await CompileSourceAsync(path, recipe, objectDirectory);
			switch (recipe.Type)
			{
				case RecipeType.Library:
					await LinkLibraryAsync(path, recipe, objectDirectory, binaryDirectory);
					break;
				case RecipeType.Executable:
					await LinkExecutableAsync(path, recipe, objectDirectory, binaryDirectory);
					break;
				default:
					throw new NotSupportedException("Unknown recipe type.");
			}

			if (recipe.Type == RecipeType.Library)
			{
				CloneModuleInterface(path, recipe, objectDirectory, binaryDirectory);
			}
		}

		/// <summary>
		/// Compile the module file
		/// </summary>
		private async Task CompileModuleAsync(string path, Recipe recipe, string objectDirectory)
		{
			Log.Info("Compile Module");
			var args = new CompilerArguments()
			{
				Standard = Compiler.LanguageStandard.Latest,
				RootDirectory = path,
				OutputDirectory = objectDirectory,
				SourceFiles = new List<string>() { recipe.Public },
				ExportModule = true,
			};

			// Ensure the object directory exists
			var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
			if (!Directory.Exists(objectDirectry))
			{
				Directory.CreateDirectory(objectDirectry);
			}

			// Compile each file
			await _compiler.CompileAsync(args);
		}

		/// <summary>
		/// Compile the supporting source files
		/// </summary>
		private async Task CompileSourceAsync(string path, Recipe recipe, string objectDirectory)
		{
			Log.Info("Compile Source");
			var modules = new List<string>();

			if (recipe.Type == RecipeType.Library)
			{
				// Add a reference to our own modules interface definition
				modules.Add($"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(recipe.Public)}.ifc");
			}

			// Add all of the dependencies as module references
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				string packagePath;
				if (dependecy.Path != null)
				{
					packagePath = dependecy.Path;
				}
				else
				{
					packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
				}

				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				var module = $"{packagePath}/bin/{dependecyRecipe.Name}.ifc";
				modules.Add(module);
			}

			var args = new CompilerArguments()
			{
				Standard = Compiler.LanguageStandard.Latest,
				RootDirectory = path,
				OutputDirectory = objectDirectory,
				SourceFiles = recipe.Source,
				Modules = modules,
			};

			// Ensure the object directory exists
			var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
			if (!Directory.Exists(objectDirectry))
			{
				Directory.CreateDirectory(objectDirectry);
			}

			// Compile each file
			await _compiler.CompileAsync(args);
		}

		/// <summary>
		/// Link the resulting object files
		/// </summary>
		private async Task LinkLibraryAsync(string path, Recipe recipe, string objectDirectory, string binaryDirectory)
		{
			Log.Info("Link");
			var allFiles = new List<string>(recipe.Source);
			if (recipe.Type == RecipeType.Library)
			{
				allFiles.Add(recipe.Public);
			}

			var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.obj").ToList();

			var args = new LinkerArguments()
			{
				Name = recipe.Name,
				RootDirectory = path,
				OutputDirectory = binaryDirectory,
				SourceFiles = objectFiles,
			};

			// Ensure the object directory exists
			var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
			if (!Directory.Exists(objectDirectry))
			{
				Directory.CreateDirectory(objectDirectry);
			}

			// Link
			await _compiler.LinkLibraryAsync(args);
		}

		/// <summary>
		/// Link the executable
		/// </summary>
		private async Task LinkExecutableAsync(string path, Recipe recipe, string objectDirectory, string binaryDirectory)
		{
			Log.Info("Link");
			var allFiles = new List<string>(recipe.Source);
			if (recipe.Type == RecipeType.Library)
			{
				allFiles.Add(recipe.Public);
			}

			// Add all of the dependencies as module references
			var librarySet = new HashSet<string>();
			await GenerateDependencyLibrarySetAsync(recipe, librarySet);

			var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.obj").ToList();
			var libraryFiles = librarySet.ToList();
			var args = new LinkerArguments()
			{
				Name = recipe.Name,
				RootDirectory = path,
				OutputDirectory = binaryDirectory,
				SourceFiles = objectFiles,
				LibraryFiles = libraryFiles,
			};

			// Ensure the object directory exists
			var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
			if (!Directory.Exists(objectDirectry))
			{
				Directory.CreateDirectory(objectDirectry);
			}

			// Link
			await _compiler.LinkExecutableAsync(args);
		}

		/// <summary>
		/// Clone the module interface definition from the object directoy to the binary directory
		/// </summary>
		private void CloneModuleInterface(string path, Recipe recipe, string objectDirectory, string binaryDirectory)
		{
			Log.Info("Clone Module Interface");
			var sourceModuleFile = Path.Combine(path, objectDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Public)}.ifc");
			var targetModuleFile = Path.Combine(path, binaryDirectory, $"{Path.GetFileNameWithoutExtension(recipe.Name)}.ifc");

			// Ensure the object directory exists
			if (!File.Exists(sourceModuleFile))
			{
				throw new InvalidOperationException("The resulting module interface definition was missing.");
			}

			Log.Info($"Clone IFC: {sourceModuleFile} -> {targetModuleFile}");
			File.Copy(sourceModuleFile, targetModuleFile, true);
		}

		private async Task GenerateDependencyLibrarySetAsync(Recipe recipe, HashSet<string> set)
		{
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				string packagePath;
				if (dependecy.Path != null)
				{
					packagePath = dependecy.Path;
				}
				else
				{
					packagePath = PackageManager.BuildKitchenPackagePath(_config, dependecy);
				}

				// Get recursive dependencies
				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);
				await GenerateDependencyLibrarySetAsync(dependecyRecipe, set);

				set.Add(Path.Combine(packagePath, "bin", $"{dependecyRecipe.Name}.lib").ToLower());
			}
		}
	}
}
