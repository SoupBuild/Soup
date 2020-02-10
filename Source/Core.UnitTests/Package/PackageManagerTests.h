// <copyright file="PackageManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class PackageManagerTests
	{
	public:
		[[Fact]]
		void InstallPackage_MissingRecipeThrows()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			Assert::ThrowsRuntimeError([]() {
				PackageManager::InstallPackage("TheirPackage");
			});

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Recipe.toml",
					"INFO: Recipe file does not exist.",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void InstallPackage_SingleSuccess()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("Recipe.toml"),
				MockFileState(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			PackageManager::InstallPackage("TheirPackage");

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Recipe.toml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
					"OpenReadBinary: Recipe.toml",
					"OpenWrite: Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
