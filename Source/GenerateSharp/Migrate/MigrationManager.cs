// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Utilities;
using System.Xml;
using Path = Opal.Path;

namespace Soup.Build.Migrate;

public static class MigrationManager
{
	public static async Task RunAsync(Path target)
	{
		switch (target.FileExtension)
		{
			case ".csproj":
				await MigrateCSProjAsync(target);
				break;
			default:
				throw new HandledException($"Invalid target {target}");
		}
	}

	private static async Task MigrateCSProjAsync(Path target)
	{
		using var readFile = LifetimeManager.Get<IFileSystem>().OpenRead(target);

		using var reader = XmlReader.Create(readFile.GetInStream(), new XmlReaderSettings() { Async = true });

		var parser = new CSProjParser();
		parser.Deserialize(reader);

		var projectName = target.FileStem;
		var targetRecipe = target.GetParent() + new Path("Recipe.sml");

		var recipe = new Recipe(projectName, new LanguageReference("C#", new SemanticVersion(0)))
		{
			Version = new SemanticVersion(1, 0, 0)
		};

		switch (parser.OutputType)
		{
			case "":
				// Leave default empty
				break;
			case "Exe":
				recipe.Type = "Executable";
				break;
			default:
				throw new InvalidOperationException($"Unknown output type {parser.OutputType}");
		}

		foreach (var reference in parser.ProjectReferences)
		{
			if (reference.Include is null)
				throw new InvalidOperationException("Missing include on project reference");

			var referencePath = reference.Include.GetParent();
			recipe.AddRuntimeDependency(referencePath.ToString());
		}

		await RecipeExtensions.SaveToFileAsync(targetRecipe, recipe);
	}
}