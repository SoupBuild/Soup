// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Utilities;
using System;
using System.Threading.Tasks;
using System.Xml;
using Path = Opal.Path;

namespace Soup.Build.Migrate;

public static class MigrationManager
{
	public static async Task RunAsync(Path target)
	{
		Log.Info($"Migrate {target}");
		switch (target.FileExtension)
		{
			case ".csproj":
				await MigrateCSProjAsync(target);
				break;
			default:
				Log.Error("Unknown file extension");
				throw new HandledException($"Invalid target {target}");
		}

		Log.Info("Migrate Finished");
	}

	private static async Task MigrateCSProjAsync(Path target)
	{
		var parser = ParseCSProj(target);

		await BuildRecipeAsync(target, parser);

		foreach (var reference in parser.ProjectReferenceItems)
		{
			if (reference.Include is null)
				throw new InvalidOperationException("Missing include on project reference");

			switch (reference.Include.FileExtension)
			{
				case ".csproj":
					var referenceTarget = target.GetParent() + reference.Include;
					await MigrateCSProjAsync(referenceTarget);
					break;
				default:
					throw new InvalidOperationException($"Unknown project reference type {reference.Include.FileExtension}");
			}
		}
	}

	private static CSProjParser ParseCSProj(Path target)
	{
		using var readFile = LifetimeManager.Get<IFileSystem>().OpenRead(target);

		using var reader = XmlReader.Create(readFile.GetInStream(), new XmlReaderSettings() { Async = true });

		var parser = new CSProjParser();
		parser.Deserialize(reader);

		return parser;
	}

	private static async Task BuildRecipeAsync(Path target, CSProjParser parser)
	{
		var projectName = target.FileStem;
		var targetRecipe = target.GetParent() + new Path("./Recipe.sml");

		var recipe = new Recipe(projectName, new LanguageReference("C#", new SemanticVersion(0)))
		{
			Version = new SemanticVersion(1, 0, 0)
		};

		switch (parser.OutputType)
		{
			case "":
			case "Library":
				// Leave default empty
				break;
			case "Exe":
				recipe.Type = "Executable";
				break;
			default:
				throw new InvalidOperationException($"Unknown output type {parser.OutputType}");
		}

		switch (parser.TargetFramework)
		{
			case "net8.0":
				recipe.Document.EnsureValueWithSyntax("TargetFramework", parser.TargetFramework);
				break;
			default:
				throw new InvalidOperationException($"Unknown target framework {parser.TargetFramework}");
		}

		if (parser.PackageReferenceItems.Count > 0)
		{
			recipe.AddBuildDependency("Soup|CSharp.Nuget@0");
		}

		foreach (var reference in parser.ProjectReferenceItems)
		{
			if (reference.Include is null)
				throw new InvalidOperationException("Missing include on project reference");

			var referencePath = reference.Include.GetParent();
			recipe.AddRuntimeDependency(referencePath.ToString());
		}

		if (parser.PackageReferenceItems.Count > 0)
		{
			var nugetTable = recipe.Document.EnsureTableWithSyntax("Nuget");
			var dependenciesTable = nugetTable.EnsureTableWithSyntax("Dependencies", 1);
			var runtimeDependencies = dependenciesTable.EnsureArrayWithSyntax("Runtime", 2);

			foreach (var reference in parser.PackageReferenceItems)
			{
				if (reference.Include is null)
					throw new InvalidOperationException("Missing include on package reference");

				if (reference.Version is null)
					throw new InvalidOperationException("Missing version on package reference");

				var referenceTable = runtimeDependencies.AddInlineTableWithSyntax(3);
				referenceTable.AddInlineItemWithSyntax("Name", reference.Include);
				referenceTable.AddInlineItemWithSyntax("Version", reference.Version);
			}
		}

		await RecipeExtensions.SaveToFileAsync(targetRecipe, recipe);
	}
}