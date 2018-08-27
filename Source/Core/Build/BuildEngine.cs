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
		public async Task ExecuteAsync(LocalUserConfig config, Recipe recipe)
		{
			Log.Info("Build Recursive Dependencies.");
			await BuildAllDependenciesRecursivelyAsync(config, recipe);

			Log.Info("Build Toplevel Recipe.");
			await CoreBuildAsync(recipe);
		}

		/// <summary>
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		private async Task BuildAllDependenciesRecursivelyAsync(LocalUserConfig config, Recipe recipe)
		{
			Log.Info($"Seaching Dependencies: {recipe.Name}.");
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				var packagePath = PackageManager.BuildKitchenPackagePath(config, dependecy);
				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				// Build all recursive dependencies
				await BuildAllDependenciesRecursivelyAsync(config, dependecyRecipe);

				// Build this dependecy
				await CoreBuildAsync(dependecyRecipe);
			}
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public async Task CoreBuildAsync(Recipe recipe)
		{
			var objectDirectory = "obj";
			var binaryDirectory = "bin";

			Log.Info($"Building {recipe.Name}.");
			await CompileModuleAsync(recipe, objectDirectory);
			await CompileSourceAsync(recipe, objectDirectory);
			await LinkAsync(recipe, objectDirectory, binaryDirectory);
			CloneModuleInterface(recipe, objectDirectory, binaryDirectory);
		}

		/// <summary>
		/// Compile the module file
		/// </summary>
		private async Task CompileModuleAsync(Recipe recipe, string objectDirectory)
		{
			var args = new CompilerArguments()
			{
				Standard = Compiler.LanguageStandard.Latest,
				RootDirectory = "./",
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
		private async Task CompileSourceAsync(Recipe recipe, string objectDirectory)
		{
			var modules = new List<string>();

			// Add a reference to our own modules interface definition
			modules.Add($"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(recipe.Public)}.ifc");

			var args = new CompilerArguments()
			{
				Standard = Compiler.LanguageStandard.Latest,
				RootDirectory = "./",
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
		private async Task LinkAsync(Recipe recipe, string objectDirectory, string binaryDirectory)
		{
			var allFiles = new List<string>(recipe.Source);
			allFiles.Add(recipe.Public);
			var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.obj").ToList();

			var args = new LinkerArguments()
			{
				Name = recipe.Name,
				Target = LinkTarget.StaticLibrary,
				RootDirectory = "./",
				OutputDirectory = binaryDirectory,
				SourceFiles = objectFiles,
			};

			// Ensure the object directory exists
			var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
			if (!Directory.Exists(objectDirectry))
			{
				Directory.CreateDirectory(objectDirectry);
			}

			// Link the final library
			await _compiler.LinkAsync(args);
		}

		/// <summary>
		/// Clone the module interface definition from the object directoy to the binary directory
		/// </summary>
		private void CloneModuleInterface(Recipe recipe, string objectDirectory, string binaryDirectory)
		{
			var sourceModuleFile = $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(recipe.Public)}.ifc";
			var targetModuleFile = $"{binaryDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(recipe.Name)}.ifc";

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
