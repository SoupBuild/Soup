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
		private ICompiler _compiler;

		/// <summary>
		/// Initializes a new instance of the <see cref="BuildEngine"/> class.
		/// </summary>
		public BuildEngine(ICompiler compiler)
		{
			_compiler = compiler;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public async Task ExecuteAsync(LocalUserConfig config, string path, Recipe recipe)
		{
			Log.Info("Build Recursive Dependencies.");
			await BuildAllDependenciesRecursivelyAsync(config, path, recipe);

			Log.Info("Build Toplevel Recipe.");
			await CoreBuildAsync(config, path, recipe);
		}

		/// <summary>
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		private async Task BuildAllDependenciesRecursivelyAsync(
			LocalUserConfig config,
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
					packagePath = PackageManager.BuildKitchenPackagePath(config, dependecy);
				}

				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				// Build all recursive dependencies
				await BuildAllDependenciesRecursivelyAsync(config, packagePath, dependecyRecipe);

				// Build this dependecy
				await CoreBuildAsync(config, packagePath, dependecyRecipe);
			}
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public async Task CoreBuildAsync(LocalUserConfig config, string path, Recipe recipe)
		{
			var objectDirectory = "obj";
			var binaryDirectory = "bin";

			Log.Info($"Building {recipe.Name}.");
			if (recipe.Type == RecipeType.Library)
			{
				await CompileModuleAsync(path, recipe, objectDirectory);
			}

			await CompileSourceAsync(config, path, recipe, objectDirectory);
			await LinkAsync(path, recipe, objectDirectory, binaryDirectory);

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
		private async Task CompileSourceAsync(LocalUserConfig config, string path, Recipe recipe, string objectDirectory)
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
					packagePath = PackageManager.BuildKitchenPackagePath(config, dependecy);
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
		private async Task LinkAsync(string path, Recipe recipe, string objectDirectory, string binaryDirectory)
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
			switch (recipe.Type)
			{
				case RecipeType.Library:
					await _compiler.LinkLibraryAsync(args);
					break;
				case RecipeType.Executable:
					await _compiler.LinkExecutableAsync(args);
					break;
				default:
					throw new NotSupportedException("Unknown recipe type.");
			}
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
	}
}
