// <copyright file="RecipeBuildLocationManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class RecipeBuildLocationManagerTests
	{
	public:
		// [[Fact]]
		void GetOutputDirectory_Simple()
		{
			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto workingDirectory = Path("C:/WorkingDirectory/");
			auto recipe = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)),
				SemanticVersion(1, 2, 3),
				std::nullopt,
				std::nullopt,
				std::nullopt);
			auto globalParameters = ValueTable();
			auto recipeCache = RecipeCache();
			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1),
						Path("Soup.Cpp.dll"))
				}
			});
			auto uut = RecipeBuildLocationManager(builtInLanguages);
			auto targetDirectory = uut.GetOutputDirectory(
				workingDirectory,
				recipe,
				globalParameters,
				recipeCache);

			Assert::AreEqual(Path("C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/"), targetDirectory, "Verify target directory matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/RootRecipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
		
		// [[Fact]]
		void GetOutputDirectory_RootRecipe()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create a root recipe
			fileSystem->CreateMockFile(
				Path("C:/RootRecipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					OutputRoot: "BuildOut/"
				)")));

			auto workingDirectory = Path("C:/WorkingDirectory/");
			auto recipe = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)),
				SemanticVersion(1, 2, 3),
				std::nullopt,
				std::nullopt,
				std::nullopt);
			auto globalParameters = ValueTable();
			auto recipeCache = RecipeCache();
			auto builtInLanguages = std::map<std::string, BuiltInLanguagePackage>(
			{
				{
					"C++",
					 BuiltInLanguagePackage(
						"Cpp",
						"Soup.Cpp",
						SemanticVersion(1, 1, 1),
						Path("Soup.Cpp.dll"))
				}
			});
			auto uut = RecipeBuildLocationManager(builtInLanguages);
			auto targetDirectory = uut.GetOutputDirectory(
				workingDirectory,
				recipe,
				globalParameters,
				recipeCache);

			Assert::AreEqual(Path("C:/BuildOut/Cpp/MyPackage/1.2.3/J_HqSstV55vlb-x6RWC_hLRFRDU/"), targetDirectory, "Verify target directory matches expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Found Root Recipe: 'C:/RootRecipe.sml'",
					"DIAG: Load Root Recipe: C:/RootRecipe.sml",
					"INFO: Override root output: C:/BuildOut/Cpp/MyPackage/1.2.3/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"OpenReadBinary: C:/RootRecipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
