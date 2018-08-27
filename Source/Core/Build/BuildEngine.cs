// <copyright company="Soup" file="BuildEngine.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Collections.Generic;
	using System.IO;
	using System.Linq;
	using System.Threading.Tasks;
	using Soup.Compiler;

	public class BuildEngine
	{
		private ICompiler _compiler;

		public BuildEngine(ICompiler compiler)
		{
			_compiler = compiler;
		}

		public async Task ExecuteAsync(Recipe recipe)
		{
			var objectDirectory = "obj";
			var binaryDirectory = "bin";

			await CompileModuleAsync(recipe, objectDirectory);
			await CompileSourceAsync(recipe, objectDirectory);
			await LinkAsync(recipe, objectDirectory, binaryDirectory);
		}

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
		/// Build the dependecies for the provided recipe recursively
		/// </summary>
		private async Task BuildAllDependenciesRecursivelyAsync(Recipe recipe)
		{
			foreach (var dependecy in recipe.Dependencies)
			{
				// Load this package recipe
				var packagePath = PackageManager.BuildKitchenPackagePath(dependecy);
				var dependecyRecipe = await RecipeManager.LoadFromFileAsync(packagePath);

				// Build all recursive dependencies
				await BuildAllDependenciesRecursivelyAsync(dependecyRecipe);

				// Build this dependecy
				Log.Message($"Building {dependecy}");
				var buildPath = PackageManager.BuildKitchenBuildPath(dependecy);
				Build();
			}
		}

		private void Build()
		{

		}
	}
}
