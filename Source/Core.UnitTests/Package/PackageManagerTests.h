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

			auto packageName = "TheirPackage";
			auto packageStore = Path("PackageStore");
			Assert::ThrowsRuntimeError([&packageName, &packageStore]() {
				PackageManager::InstallPackage(packageName, packageStore);
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

			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("Recipe.toml"),
				MockFileState(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"localhost",
				7071);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageResult = std::string(
				R"({
					"name": "TheirPackage",
					"latest": "2.2.2"
				})");
			testHttpClient->SetResponse("/api/v1/packages/TheirPackage", packageResult);

			auto packageName = "TheirPackage";
			auto packageStore = Path("PackageStore");
			PackageManager::InstallPackage(packageName, packageStore);

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Recipe.toml",
					"HIGH: Install Package: TheirPackage",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
					"OpenReadBinary: Recipe.toml",
					"Exists: PackageStore/.staging",
					"CreateDirectory: PackageStore/.staging",
					"OpenWrite: Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: localhost:7071",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /api/v1/packages/TheirPackage",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");

			// Verify the contents of the recipe file
			std::string expectedFinalRecipe = 
R"(Dependencies = ["TheirPackage@2.2.2"]
Name = "MyPackage"
Version = "1.2.3"
)";
			auto& mockRecipeFile = fileSystem->GetMockFile(Path("Recipe.toml"));
			Assert::AreEqual(expectedFinalRecipe, mockRecipeFile.Contents->str(), "Verify recipe file contents.");
		}
	};
}
