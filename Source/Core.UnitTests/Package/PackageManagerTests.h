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
			auto scopedTraceListener =  ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"soupapi.trafficmanager.net",
				80);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageResult = std::string(
				R"({
					"name": "TheirPackage",
					"latest": "2.2.2"
				})");
			testHttpClient->SetResponse("/api/v1/packages/TheirPackage", packageResult);

			auto packageContent = std::vector<unsigned char>(
			{
				0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C, 0x00, 0x04, 0x4A, 0xD5, 0xC7, 0xEB, 0x42, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xBD, 0x4C, 0x8C, 
				0x01, 0x00, 0x3D, 0x4E, 0x61, 0x6D, 0x65, 0x20, 0x3D, 0x20, 0x22, 0x4D, 0x79, 0x50, 0x72, 0x6F, 
				0x6A, 0x65, 0x63, 0x74, 0x22, 0x0D, 0x0A, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x3D, 
				0x20, 0x22, 0x31, 0x2E, 0x32, 0x2E, 0x33, 0x22, 0x0D, 0x0A, 0x50, 0x75, 0x62, 0x6C, 0x69, 0x63, 
				0x20, 0x3D, 0x20, 0x22, 0x4D, 0x6F, 0x64, 0x75, 0x6C, 0x65, 0x2E, 0x63, 0x70, 0x70, 0x22, 0x0D, 
				0x0A, 0x00, 0x01, 0x04, 0x06, 0x00, 0x01, 0x09, 0x42, 0x00, 0x07, 0x0B, 0x01, 0x00, 0x01, 0x21, 
				0x21, 0x01, 0x00, 0x0C, 0x3E, 0x00, 0x08, 0x0A, 0x01, 0x90, 0x59, 0x1A, 0xB9, 0x00, 0x00, 0x05, 
				0x01, 0x19, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 
				0x19, 0x00, 0x52, 0x00, 0x65, 0x00, 0x63, 0x00, 0x69, 0x00, 0x70, 0x00, 0x65, 0x00, 0x2E, 0x00, 
				0x74, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x19, 0x02, 0x00, 0x00, 0x14, 0x0A, 
				0x01, 0x00, 0x5F, 0xFB, 0xB6, 0x43, 0xE8, 0xED, 0xD5, 0x01, 0x15, 0x06, 0x01, 0x00, 0x20, 0x00, 
				0x00, 0x00, 0x00, 0x00,
			});
			auto packageContentString = std::string(packageContent.begin(), packageContent.end());
			testHttpClient->SetResponse("/api/v1/packages/TheirPackage/v2.2.2/download", packageContentString);

			auto packageName = "TheirPackage";
			auto packageStore = Path("PackageStore");
			PackageManager::InstallPackage(packageName, packageStore);

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: Recipe.toml",
					"HIGH: Install Package: TheirPackage",
					"HIGH: Latest Version: 2.2.2",
					"HIGH: Downloading package",
					"INFO: ExtractStart: 62",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 62",
					"INFO: ExtractGetStream: PackageStore/.staging/2.2.2/Recipe.toml",
					"INFO: ExtractOnOperationStart",
					"INFO: ExtractOperationCompleted",
					"INFO: ExtractProgress: 62",
					"INFO: Deleting staging directory",
					"INFO: Adding reference to recipe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Recipe.toml",
					"OpenReadBinary: Recipe.toml",
					"Exists: PackageStore/.staging",
					"CreateDirectory: PackageStore/.staging",
					"Exists: PackageStore/TheirPackage/2.2.2",
					"OpenWriteBinary: PackageStore/.staging/TheirPackage.7z",
					"CreateDirectory: PackageStore/.staging/2.2.2",
					"OpenReadBinary: PackageStore/.staging/TheirPackage.7z",
					"Exists: PackageStore/.staging/2.2.2/",
					"CreateDirectory: PackageStore/.staging/2.2.2/",
					"OpenWriteBinary: PackageStore/.staging/2.2.2/Recipe.toml",
					"SetLastWriteTime: PackageStore/.staging/2.2.2/Recipe.toml",
					"Exists: PackageStore/TheirPackage",
					"CreateDirectory: PackageStore/TheirPackage",
					"Rename: [PackageStore/.staging/2.2.2] -> [PackageStore/TheirPackage/2.2.2]",
					"DeleteDirectoryRecursive: PackageStore/.staging",
					"OpenWrite: Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: soupapi.trafficmanager.net:80",
					"CreateClient: soupapi.trafficmanager.net:80",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /api/v1/packages/TheirPackage",
					"Get: /api/v1/packages/TheirPackage/v2.2.2/download",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");

			// Verify the contents of the recipe file
			std::string expectedFinalRecipe = 
R"(Dependencies = [
"TheirPackage@2.2.2",
]
Name = "MyPackage"
Version = "1.2.3"
)";
			auto& mockRecipeFile = fileSystem->GetMockFile(Path("Recipe.toml"));
			Assert::AreEqual(expectedFinalRecipe, mockRecipeFile->Content.str(), "Verify recipe file contents.");
		}

		[[Fact]]
		void PublishPackage_Success()
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
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"soupapi.trafficmanager.net",
				80);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageResult = std::string(
				R"({
					"name": "TheirPackage",
					"latest": "2.2.2"
				})");
			testHttpClient->SetResponse("/api/v1/packages/MyPackage", packageResult);

			auto packageName = "TheirPackage";
			auto packageStore = Path("PackageStore");
			PackageManager::PublishPackage(packageStore);

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
					"DeleteDirectoryRecursive: PackageStore/.staging",
					"OpenWrite: Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: soupapi.trafficmanager.net:80",
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
			Assert::AreEqual(expectedFinalRecipe, mockRecipeFile->Content.str(), "Verify recipe file contents.");
		}
	};
}
