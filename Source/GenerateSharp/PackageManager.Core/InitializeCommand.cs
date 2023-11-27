// <copyright file="InitializeCommand.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using Opal;
using Opal.System;
using Soup.Build.Utilities;
using System;
using System.Threading.Tasks;

namespace Soup.Build.PackageManager;

/// <summary>
/// The initialize command
/// </summary>
public class InitializeCommand
{
	private const string CppTypeExecutable = "Executable";
	private const string BuiltInLanguageCpp = "C++";

	private SemanticVersion _builtInLanguageVersionCpp;

	public InitializeCommand(
		SemanticVersion builtInLanguageVersionCpp)
	{
		_builtInLanguageVersionCpp = builtInLanguageVersionCpp;
	}

	/// <summary>
	/// Initialize a package
	/// </summary>
	public async Task InitializePackageAsync(Path workingDirectory)
	{
		// Use the current directory as the default names
		var recipePath =
			workingDirectory +
			BuildConstants.RecipeFileName;

		// Todo: Opal path should have a way to get individual directories
		var workingFolderValue = workingDirectory.ToString();
		workingFolderValue = workingFolderValue.Remove(workingFolderValue.Length - 1, 1);
		var workingFolder = new Path(workingFolderValue);

		var recipe = new Recipe(
			workingFolder.GetFileName(),
			new LanguageReference(
				BuiltInLanguageCpp,
				new SemanticVersion(
					_builtInLanguageVersionCpp.Major,
					_builtInLanguageVersionCpp.Minor,
					null)));

		recipe.Type = CppTypeExecutable;
		recipe.Version = new SemanticVersion(1, 0, 0);
		recipe.AddSource("Main.cpp");

		UpdateDefaultValues(recipe);

		// Save the state of the recipe if it has changed
		await RecipeExtensions.SaveToFileAsync(recipePath, recipe);

		// Save a simple main method
		var mainFileContent =
@"#include <iostream>
int main()
{
	std::cout << ""Hello World"" << std::endl;
	return 0;
}";

		var mainFilePath = workingDirectory + new Path("Main.cpp");
		using var mainFile = LifetimeManager.Get<IFileSystem>().OpenWrite(mainFilePath, false);
		using var mainFileWriter = new System.IO.StreamWriter(mainFile.GetOutStream(), null, -1, true);

		await mainFileWriter.WriteAsync(mainFileContent);
	}

	private static void UpdateDefaultValues(Recipe recipe)
	{
		Log.HighPriority($"Name: ({recipe.Name})");
		var newName = Console.ReadLine();
		if (!string.IsNullOrWhiteSpace(newName))
		{
			recipe.Name = newName;
		}

		bool setVersion = false;
		while (!setVersion)
		{
			Log.HighPriority($"Version: ({recipe.Version})");
			var newVersion = Console.ReadLine();
			if (string.IsNullOrWhiteSpace(newVersion))
			{
				// Use the default
				setVersion = true;
			}
			else
			{
				if (SemanticVersion.TryParse(newVersion, out var value))
				{
					recipe.Version = value;
					setVersion = true;
				}
				else
				{
					Log.Warning($"Invalid version: \"{newVersion}\"");
				}
			}
		}
	}
}