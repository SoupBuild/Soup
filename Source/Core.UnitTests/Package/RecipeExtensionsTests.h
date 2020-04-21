// <copyright file="RecipeExtensionsTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class RecipeExtensionsTests
	{
	public:
		[[Fact]]
		void TryLoadFromFile_MissingFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto directory = Path("TestFiles/NoFile/Recipe.toml");
			Recipe actual;
			auto result = RecipeExtensions::TryLoadFromFile(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/NoFile/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: TestFiles/NoFile/Recipe.toml",
					"INFO: Recipe file does not exist.",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}
		
		[[Fact]]
		void TryLoadFromFile_GarbageFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("TestFiles/GarbageRecipe/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto directory = Path("TestFiles/GarbageRecipe/Recipe.toml");
			Recipe actual;
			auto result = RecipeExtensions::TryLoadFromFile(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/GarbageRecipe/Recipe.toml",
					"OpenReadBinary: TestFiles/GarbageRecipe/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: TestFiles/GarbageRecipe/Recipe.toml",
					"ERRO: Deserialize Threw: Parsing the Recipe Toml failed: [error] toml::parse_key_value_pair: missing key-value separator `=`\n --> Recipe.toml\n   | \n 1 | garbage\n   |        ^ should be `=`",
					"INFO: Failed to parse Recipe.",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void TryLoadFromFile_SimpleFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleRecipe/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			auto directory = Path("TestFiles/SimpleRecipe/Recipe.toml");
			Recipe actual;
			auto result = RecipeExtensions::TryLoadFromFile(directory, actual);

			Assert::IsTrue(result, "Verify result is false.");

			auto expected = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreEqual(expected, actual, "Verify matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/SimpleRecipe/Recipe.toml",
					"OpenReadBinary: TestFiles/SimpleRecipe/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: TestFiles/SimpleRecipe/Recipe.toml",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void SaveToFile_SimpleFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto directory = Path("TestFiles/SimpleRecipe/Recipe.toml");
			auto recipe = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));
			RecipeExtensions::SaveToFile(directory, recipe);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"OpenWrite: TestFiles/SimpleRecipe/Recipe.toml",
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
R"(Name = "MyPackage"
Version = "1.2.3"
)";
			auto& mockBuildFile = fileSystem->GetMockFile(Path("TestFiles/SimpleRecipe/Recipe.toml"));
			Assert::AreEqual(expectedBuildFile, mockBuildFile->Content.str(), "Verify file contents.");
		}

		[[Fact]]
		void GetRecipeOutputPath_MissingRecipeThrows()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto packagePath = Path("Root/");
			auto binaryDirectory = Path("out/bin/mock/");
			std::string modileFileExtension = ".mock.bmi";
			Assert::ThrowsRuntimeError([&packagePath, &binaryDirectory, &modileFileExtension]() {
				auto result = RecipeExtensions::GetRecipeOutputPath(packagePath, binaryDirectory, modileFileExtension);
			});

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Root/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Root/Recipe.toml",
					"INFO: Recipe file does not exist.",
					"ERRO: Failed to load the package: Root/Recipe.toml",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void GetRecipeOutputPath_Exists()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("Root/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			auto packagePath = Path("Root/");
			auto binaryDirectory = Path("out/bin/mock/");
			std::string modileFileExtension = "mock.bmi";
			auto result = RecipeExtensions::GetRecipeOutputPath(packagePath, binaryDirectory, modileFileExtension);

			Assert::AreEqual(result, Path("Root/out/bin/mock/MyPackage.mock.bmi"), "Verify the result matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Root/Recipe.toml",
					"OpenReadBinary: Root/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Root/Recipe.toml",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void GetPackageReferencePath_IsRooted()
		{
			auto reference = Path("Root/Sub/");
			auto binaryDirectory = PackageReference(Path("C:/Other/Reference/"));
			auto result = RecipeExtensions::GetPackageReferencePath(reference, binaryDirectory);

			Assert::AreEqual(result, Path("C:/Other/Reference/"), "Verify the result matches expected.");
		}

		[[Fact]]
		void GetPackageReferencePath_NotRooted()
		{
			auto reference = Path("Root/Sub/");
			auto binaryDirectory = PackageReference(Path("../Reference/"));
			auto result = RecipeExtensions::GetPackageReferencePath(reference, binaryDirectory);

			Assert::AreEqual(result, Path("Root/Reference/"), "Verify the result matches expected.");
		}
	};
}
