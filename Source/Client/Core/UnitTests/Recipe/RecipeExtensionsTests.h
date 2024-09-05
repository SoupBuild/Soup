// <copyright file="RecipeExtensionsTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class RecipeExtensionsTests
	{
	public:
		// [[Fact]]
		void TryLoadRecipeFromFile_MissingFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto directory = Path("./TestFiles/NoFile/Recipe.sml");
			Recipe actual;
			auto result = RecipeExtensions::TryLoadRecipeFromFile(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/NoFile/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: ./TestFiles/NoFile/Recipe.sml",
					"INFO: Recipe file does not exist.",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}
		
		// [[Fact]]
		void TryLoadRecipeFromFile_GarbageFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("./TestFiles/GarbageRecipe/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto directory = Path("./TestFiles/GarbageRecipe/Recipe.sml");
			Recipe actual;
			auto result = RecipeExtensions::TryLoadRecipeFromFile(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/GarbageRecipe/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: ./TestFiles/GarbageRecipe/Recipe.sml",
					"ERRO: Deserialize Threw: Parsing the Recipe SML failed: Failed to parse at 1:7  ./TestFiles/GarbageRecipe/Recipe.sml",
					"INFO: Failed to parse Recipe.",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		// [[Fact]]
		void TryLoadRecipeFromFile_SimpleFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("./TestFiles/SimpleRecipe/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'MyPackage'
					Language: 'C++|1'
				)")));

			auto directory = Path("./TestFiles/SimpleRecipe/Recipe.sml");
			Recipe actual;
			auto result = RecipeExtensions::TryLoadRecipeFromFile(directory, actual);

			Assert::IsTrue(result, "Verify result is false.");

			auto expected = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
			}));

			Assert::AreEqual(expected, actual, "Verify matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/SimpleRecipe/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: ./TestFiles/SimpleRecipe/Recipe.sml",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		// [[Fact]]
		void SaveToFile_SimpleFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto directory = Path("./TestFiles/SimpleRecipe/Recipe.sml");
			auto recipe = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
			}));
			RecipeExtensions::SaveToFile(directory, recipe);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"OpenWrite: ./TestFiles/SimpleRecipe/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");

			// Verify the contents of the build file
			std::string expectedBuildFile = 
R"(Name: 'MyPackage'
Language: 'C++|1'
)";
			auto mockBuildFile = fileSystem->GetMockFile(Path("./TestFiles/SimpleRecipe/Recipe.sml"));
			Assert::AreEqual(expectedBuildFile, mockBuildFile->Content.str(), "Verify file contents.");
		}
	};
}
