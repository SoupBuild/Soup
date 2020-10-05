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
		void InstallPackages_NoDependencies_Success()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("C:/TestLocation/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
				)")));

			auto workingDirectory = Path("C:/TestLocation");
			PackageManager::InstallPackages(workingDirectory);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"INFO: Deleting staging directory",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"Exists: C:/TestLocation/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/Recipe.toml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void InstallPackages_OnlyLocalDependencies_Success()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("C:/TestLocation/SubFolder/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
					Dependencies = [
						"../MyProject1/",
						"../../MyProject2/",
					]
				)")));

			fileSystem->CreateMockFile(
				Path("C:/TestLocation/MyProject1/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyProject1"
					Language = "C++"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/MyProject2/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyProject2"
					Language = "C++"
					Dependencies = [
						"../MyProject3/",
					]
				)")));

			fileSystem->CreateMockFile(
				Path("C:/MyProject3/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyProject3"
					Language = "C++"
				)")));

			auto workingDirectory = Path("C:/TestLocation/SubFolder/");
			PackageManager::InstallPackages(workingDirectory);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Load Recipe: C:/TestLocation/SubFolder/Recipe.toml",
					"DIAG: Load Recipe: C:/TestLocation/MyProject1/Recipe.toml",
					"DIAG: Load Recipe: C:/MyProject2/Recipe.toml",
					"DIAG: Load Recipe: C:/MyProject3/Recipe.toml",
					"INFO: Deleting staging directory",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"Exists: C:/TestLocation/SubFolder/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/SubFolder/Recipe.toml",
					"Exists: C:/TestLocation/MyProject1/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/MyProject1/Recipe.toml",
					"Exists: C:/MyProject2/Recipe.toml",
					"OpenReadBinary: C:/MyProject2/Recipe.toml",
					"Exists: C:/MyProject3/Recipe.toml",
					"OpenReadBinary: C:/MyProject3/Recipe.toml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void InstallPackages_SingleDependency_Success()
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
				Path("C:/TestLocation/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
					Dependencies = [
						"TheirPackage@2.2.2",
					]
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageContentResponse = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				GetTheirPackageArchive());
			testHttpClient->AddGetResponse("/v1/packages/TheirPackage/v2.2.2/download", packageContentResponse);

			auto workingDirectory = Path("C:/TestLocation");
			PackageManager::InstallPackages(workingDirectory);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"HIGH: Install Package: TheirPackage@2.2.2",
					"HIGH: Downloading package",
					"DIAG: /v1/packages/TheirPackage/v2.2.2/download",
					"INFO: ExtractStart: 58",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractGetStream: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"INFO: ExtractOnOperationStart",
					"INFO: ExtractOperationCompleted",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractDone",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"INFO: Deleting staging directory",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"Exists: C:/TestLocation/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"SetLastWriteTime: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage",
					"CreateDirectory: C:/Users/Me/.soup/packages/TheirPackage",
					"Rename: [C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/] -> [C:/Users/Me/.soup/packages/TheirPackage/2.2.2/]",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /v1/packages/TheirPackage/v2.2.2/download",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");
		}

		[[Fact]]
		void InstallPackageReference_MissingRecipe_Throws()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto workingDirectory = Path("C:/TestLocation");
			auto packageName = "TheirPackage";
			Assert::ThrowsRuntimeError([&workingDirectory, &packageName]() {
				PackageManager::InstallPackageReference(workingDirectory, packageName);
			});

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"INFO: Recipe file does not exist.",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestLocation/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void InstallPackageReference_Version_Success()
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
				Path("C:/TestLocation/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageContentResponse = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				GetTheirPackageArchive());
			testHttpClient->AddGetResponse("/v1/packages/TheirPackage/v2.2.2/download", packageContentResponse);

			auto workingDirectory = Path("C:/TestLocation");
			auto packageName = "TheirPackage@2.2.2";
			PackageManager::InstallPackageReference(workingDirectory, packageName);

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"HIGH: Install Package: TheirPackage@2.2.2",
					"HIGH: Downloading package",
					"DIAG: /v1/packages/TheirPackage/v2.2.2/download",
					"INFO: ExtractStart: 58",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractGetStream: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"INFO: ExtractOnOperationStart",
					"INFO: ExtractOperationCompleted",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractDone",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"INFO: Deleting staging directory",
					"INFO: Adding reference to recipe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestLocation/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"SetLastWriteTime: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage",
					"CreateDirectory: C:/Users/Me/.soup/packages/TheirPackage",
					"Rename: [C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/] -> [C:/Users/Me/.soup/packages/TheirPackage/2.2.2/]",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
					"OpenWrite: C:/TestLocation/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /v1/packages/TheirPackage/v2.2.2/download",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");

			// Verify the contents of the recipe file
			std::string expectedFinalRecipe = 
R"(Name = "MyPackage"
Language = "C++"
Dependencies = [
"TheirPackage@2.2.2",
]
)";
			auto& mockRecipeFile = fileSystem->GetMockFile(Path("C:/TestLocation/Recipe.toml"));
			Assert::AreEqual(expectedFinalRecipe, mockRecipeFile->Content.str(), "Verify recipe file contents.");
		}

		[[Fact]]
		void InstallPackageReference_Recursive_Success()
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
				Path("C:/TestLocation/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageOneContentResponse = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				GetTheirPackageWithDependencyArchive());
			testHttpClient->AddGetResponse("/v1/packages/TheirPackage/v2.2.2/download", packageOneContentResponse);

			auto packageTwoContentResponse = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				GetMyDependencyArchive());
			testHttpClient->AddGetResponse("/v1/packages/MyDependency/v1.0.0/download", packageTwoContentResponse);

			auto workingDirectory = Path("C:/TestLocation");
			auto packageName = "TheirPackage@2.2.2";
			PackageManager::InstallPackageReference(workingDirectory, packageName);

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"HIGH: Install Package: TheirPackage@2.2.2",
					"HIGH: Downloading package",
					"DIAG: /v1/packages/TheirPackage/v2.2.2/download",
					"INFO: ExtractStart: 104",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 104",
					"INFO: ExtractGetStream: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"INFO: ExtractOnOperationStart",
					"INFO: ExtractOperationCompleted",
					"INFO: ExtractProgress: 104",
					"INFO: ExtractDone",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"HIGH: Install Package: MyDependency@1.0.0",
					"HIGH: Downloading package",
					"DIAG: /v1/packages/MyDependency/v1.0.0/download",
					"INFO: ExtractStart: 58",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractGetStream: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/Recipe.toml",
					"INFO: ExtractOnOperationStart",
					"INFO: ExtractOperationCompleted",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractDone",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/Recipe.toml",
					"INFO: Deleting staging directory",
					"INFO: Adding reference to recipe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestLocation/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"SetLastWriteTime: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/MyDependency/1.0.0/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/MyDependency.7z",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/MyDependency.7z",
					"Exists: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/Recipe.toml",
					"SetLastWriteTime: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/Recipe.toml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/MyDependency",
					"CreateDirectory: C:/Users/Me/.soup/packages/MyDependency",
					"Rename: [C:/Users/Me/.soup/packages/.staging/MyDependency/1.0.0/] -> [C:/Users/Me/.soup/packages/MyDependency/1.0.0/]",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage",
					"CreateDirectory: C:/Users/Me/.soup/packages/TheirPackage",
					"Rename: [C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/] -> [C:/Users/Me/.soup/packages/TheirPackage/2.2.2/]",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
					"OpenWrite: C:/TestLocation/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: api.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /v1/packages/TheirPackage/v2.2.2/download",
					"Get: /v1/packages/MyDependency/v1.0.0/download",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");

			// Verify the contents of the recipe file
			std::string expectedFinalRecipe = 
R"(Name = "MyPackage"
Language = "C++"
Dependencies = [
"TheirPackage@2.2.2",
]
)";
			auto& mockRecipeFile = fileSystem->GetMockFile(Path("C:/TestLocation/Recipe.toml"));
			Assert::AreEqual(expectedFinalRecipe, mockRecipeFile->Content.str(), "Verify recipe file contents.");
		}

		[[Fact]]
		void InstallPackageReference_Latest_Success()
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
				Path("C:/TestLocation/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
				)")));

			// Create the required http client
			auto testHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testHttpClient);

			// Setup the expected http requests
			auto packageResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({
					"name": "TheirPackage",
					"latest": "2.2.2"
				})");
			testHttpClient->AddGetResponse("/v1/packages/TheirPackage", packageResult);

			auto packageContentResponse = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				GetTheirPackageArchive());
			testHttpClient->AddGetResponse("/v1/packages/TheirPackage/v2.2.2/download", packageContentResponse);

			auto workingDirectory = Path("C:/TestLocation");
			auto packageName = "TheirPackage";
			PackageManager::InstallPackageReference(workingDirectory, packageName);

			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: /v1/packages/TheirPackage",
					"HIGH: Latest Version: 2.2.2",
					"HIGH: Install Package: TheirPackage@2.2.2",
					"HIGH: Downloading package",
					"DIAG: /v1/packages/TheirPackage/v2.2.2/download",
					"INFO: ExtractStart: 58",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 0",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractGetStream: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"INFO: ExtractOnOperationStart",
					"INFO: ExtractOperationCompleted",
					"INFO: ExtractProgress: 58",
					"INFO: ExtractDone",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"INFO: Deleting staging directory",
					"INFO: Adding reference to recipe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestLocation/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage.7z",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"SetLastWriteTime: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/Recipe.toml",
					"Exists: C:/Users/Me/.soup/packages/TheirPackage",
					"CreateDirectory: C:/Users/Me/.soup/packages/TheirPackage",
					"Rename: [C:/Users/Me/.soup/packages/.staging/TheirPackage/2.2.2/] -> [C:/Users/Me/.soup/packages/TheirPackage/2.2.2/]",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
					"OpenWrite: C:/TestLocation/Recipe.toml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: api.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /v1/packages/TheirPackage",
					"Get: /v1/packages/TheirPackage/v2.2.2/download",
				}),
				testHttpClient->GetRequests(),
				"Verify http requests match expected.");

			// Verify the contents of the recipe file
			std::string expectedFinalRecipe = 
R"(Name = "MyPackage"
Language = "C++"
Dependencies = [
"TheirPackage@2.2.2",
]
)";
			auto& mockRecipeFile = fileSystem->GetMockFile(Path("C:/TestLocation/Recipe.toml"));
			Assert::AreEqual(expectedFinalRecipe, mockRecipeFile->Content.str(), "Verify recipe file contents.");
		}

		[[Fact]]
		void PublishPackage_Success()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto consoleManager = std::make_shared<IO::MockConsoleManager>();
			auto scopedConsoleManager = IO::ScopedConsoleManagerRegister(consoleManager);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("C:/TestLocation/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testAuthHttpClient = std::make_shared<Network::MockHttpClient>(
				"auth.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testAuthHttpClient);

			auto testApiHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testApiHttpClient);

			// Setup the expected http requests
			auto openIdConfigResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"issuer":"https://auth.soupbuild.com","jwks_uri":"https://auth.soupbuild.com/.well-known/openid-configuration/jwks","authorization_endpoint":"https://auth.soupbuild.com/connect/authorize","token_endpoint":"https://auth.soupbuild.com/connect/token","userinfo_endpoint":"https://auth.soupbuild.com/connect/userinfo","end_session_endpoint":"https://auth.soupbuild.com/connect/endsession","check_session_iframe":"https://auth.soupbuild.com/connect/checksession","revocation_endpoint":"https://auth.soupbuild.com/connect/revocation","introspection_endpoint":"https://auth.soupbuild.com/connect/introspect","device_authorization_endpoint":"https://auth.soupbuild.com/connect/deviceauthorization","frontchannel_logout_supported":true,"frontchannel_logout_session_supported":true,"backchannel_logout_supported":true,"backchannel_logout_session_supported":true,"scopes_supported":["openid","profile","soup_api","offline_access"],"claims_supported":["sub","name","family_name","given_name","middle_name","nickname","preferred_username","profile","picture","website","gender","birthdate","zoneinfo","locale","updated_at"],"grant_types_supported":["authorization_code","client_credentials","refresh_token","implicit","password","urn:ietf:params:oauth:grant-type:device_code"],"response_types_supported":["code","token","id_token","id_token token","code id_token","code token","code id_token token"],"response_modes_supported":["form_post","query","fragment"],"token_endpoint_auth_methods_supported":["client_secret_basic","client_secret_post"],"id_token_signing_alg_values_supported":["RS256"],"subject_types_supported":["public"],"code_challenge_methods_supported":["plain","S256"],"request_parameter_supported":true})");
			testAuthHttpClient->AddGetResponse("/.well-known/openid-configuration", openIdConfigResult);
			
			auto connectTokenResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"access_token":"SUPER_TOKEN","expires_in":3600,"token_type":"Bearer","scope":"soup_api"})");
			testAuthHttpClient->AddPostResponse("/connect/token", connectTokenResult);

			auto packageResult = Network::HttpResponse(
				Network::HttpStatusCode::Created,
				"");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage/v1.2.3", packageResult);

			auto workingDirectory = Path("C:/TestLocation");
			PackageManager::PublishPackage(workingDirectory);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Publish Project: {recipe.Name}@{recipe.Version}",
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"INFO: UpdateStart: 0",
					"INFO: UpdateProgress: 0.000000",
					"INFO: UpdateDone",
					"INFO: Request Authentication Token",
					"HIGH: UserName:",
					"HIGH: Password:",
					"DIAG: /.well-known/openid-configuration",
					"DIAG: /connect/token",
					"INFO: Publish package",
					"DIAG: /v1/packages/MyPackage/v1.2.3",
					"INFO: Package version created",
					"INFO: Cleanup staging directory",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetStandardInput",
					"ReadLine: ",
					"ReadPassword: ",
				}),
				consoleManager->GetRequests(),
				"Verify console manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestLocation/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"GetDirectoryChildren: C:/TestLocation",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /.well-known/openid-configuration",
					"Post: /connect/token [application/x-www-form-urlencoded]",
				}),
				testAuthHttpClient->GetRequests(),
				"Verify http requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage/v1.2.3 [application/x-7z-compressed]",
				}),
				testApiHttpClient->GetRequests(),
				"Verify http requests match expected.");
		}

		[[Fact]]
		void PublishPackage_PackageMissing_Success()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto consoleManager = std::make_shared<IO::MockConsoleManager>();
			auto scopedConsoleManager = IO::ScopedConsoleManagerRegister(consoleManager);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testNetworkManager = std::make_shared<Network::MockNetworkManager>();
			auto scopedNetworkManager = Network::ScopedNetworkManagerRegister(testNetworkManager);

			// Create the Recipe
			fileSystem->CreateMockFile(
				Path("C:/TestLocation/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++"
					Version = "1.2.3"
				)")));

			// Create the required http client
			auto testAuthHttpClient = std::make_shared<Network::MockHttpClient>(
				"auth.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testAuthHttpClient);

			auto testApiHttpClient = std::make_shared<Network::MockHttpClient>(
				"api.soupbuild.com",
				443);
			testNetworkManager->RegisterClient(testApiHttpClient);

			// Setup the expected http requests
			auto openIdConfigResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"issuer":"https://auth.soupbuild.com","jwks_uri":"https://auth.soupbuild.com/.well-known/openid-configuration/jwks","authorization_endpoint":"https://auth.soupbuild.com/connect/authorize","token_endpoint":"https://auth.soupbuild.com/connect/token","userinfo_endpoint":"https://auth.soupbuild.com/connect/userinfo","end_session_endpoint":"https://auth.soupbuild.com/connect/endsession","check_session_iframe":"https://auth.soupbuild.com/connect/checksession","revocation_endpoint":"https://auth.soupbuild.com/connect/revocation","introspection_endpoint":"https://auth.soupbuild.com/connect/introspect","device_authorization_endpoint":"https://auth.soupbuild.com/connect/deviceauthorization","frontchannel_logout_supported":true,"frontchannel_logout_session_supported":true,"backchannel_logout_supported":true,"backchannel_logout_session_supported":true,"scopes_supported":["openid","profile","soup_api","offline_access"],"claims_supported":["sub","name","family_name","given_name","middle_name","nickname","preferred_username","profile","picture","website","gender","birthdate","zoneinfo","locale","updated_at"],"grant_types_supported":["authorization_code","client_credentials","refresh_token","implicit","password","urn:ietf:params:oauth:grant-type:device_code"],"response_types_supported":["code","token","id_token","id_token token","code id_token","code token","code id_token token"],"response_modes_supported":["form_post","query","fragment"],"token_endpoint_auth_methods_supported":["client_secret_basic","client_secret_post"],"id_token_signing_alg_values_supported":["RS256"],"subject_types_supported":["public"],"code_challenge_methods_supported":["plain","S256"],"request_parameter_supported":true})");
			testAuthHttpClient->AddGetResponse("/.well-known/openid-configuration", openIdConfigResult);
			
			auto connectTokenResult = Network::HttpResponse(
				Network::HttpStatusCode::Ok,
				R"({"access_token":"SUPER_TOKEN","expires_in":3600,"token_type":"Bearer","scope":"soup_api"})");
			testAuthHttpClient->AddPostResponse("/connect/token", connectTokenResult);

			auto publishPackageResultMissing = Network::HttpResponse(
				Network::HttpStatusCode::NotFound,
				"");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage/v1.2.3", publishPackageResultMissing);
			
			auto createPackageResult = Network::HttpResponse(
				Network::HttpStatusCode::Created,
				R"({"name":"MyPackage"})");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage", createPackageResult);

			auto publishPackageResultSuccess = Network::HttpResponse(
				Network::HttpStatusCode::Created,
				"");
			testApiHttpClient->AddPutResponse("/v1/packages/MyPackage/v1.2.3", publishPackageResultSuccess);

			auto workingDirectory = Path("C:/TestLocation");
			PackageManager::PublishPackage(workingDirectory);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Publish Project: {recipe.Name}@{recipe.Version}",
					"DIAG: Load Recipe: C:/TestLocation/Recipe.toml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"INFO: UpdateStart: 0",
					"INFO: UpdateProgress: 0.000000",
					"INFO: UpdateDone",
					"INFO: Request Authentication Token",
					"HIGH: UserName:",
					"HIGH: Password:",
					"DIAG: /.well-known/openid-configuration",
					"DIAG: /connect/token",
					"INFO: Publish package",
					"DIAG: /v1/packages/MyPackage/v1.2.3",
					"HIGH: The provided package name does not exist",
					"HIGH: Create package",
					"DIAG: /v1/packages/MyPackage",
					"INFO: Package version created",
					"HIGH: Retry publish package",
					"DIAG: /v1/packages/MyPackage/v1.2.3",
					"INFO: Package version created",
					"INFO: Cleanup staging directory",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetStandardInput",
					"ReadLine: ",
					"ReadPassword: ",
				}),
				consoleManager->GetRequests(),
				"Verify console manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestLocation/Recipe.toml",
					"OpenReadBinary: C:/TestLocation/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging",
					"GetDirectoryChildren: C:/TestLocation",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"OpenReadBinary: C:/Users/Me/.soup/packages/.staging/MyPackage.7z",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: auth.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
					"CreateClient: api.soupbuild.com:443",
				}),
				testNetworkManager->GetRequests(),
				"Verify network manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"Get: /.well-known/openid-configuration",
					"Post: /connect/token [application/x-www-form-urlencoded]",
				}),
				testAuthHttpClient->GetRequests(),
				"Verify http requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage/v1.2.3 [application/x-7z-compressed]",
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage [application/json]",
					"SetAuthenticationToken: Bearer:SUPER_TOKEN",
					"Put: /v1/packages/MyPackage/v1.2.3 [application/x-7z-compressed]",
				}),
				testApiHttpClient->GetRequests(),
				"Verify http requests match expected.");
		}

	private:
		std::string GetTheirPackageArchive()
		{
			// Simple recipe file with a single dependency
			// Name = "TheirPackage"
			// Language = "C++"
			// Version = "2.2.2"
			auto archive = std::vector<unsigned char>(
			{
				0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C, 0x00, 0x04, 0xE8, 0x9C, 0x1E, 0xE8, 0x3E, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7B, 0x52, 0x8E, 0x3F,
				0x01, 0x00, 0x39, 0x4E, 0x61, 0x6D, 0x65, 0x20, 0x3D, 0x20, 0x22, 0x54, 0x68, 0x65, 0x69, 0x72,
				0x50, 0x61, 0x63, 0x6B, 0x61, 0x67, 0x65, 0x22, 0x0D, 0x0A, 0x4C, 0x61, 0x6E, 0x67, 0x75, 0x61,
				0x67, 0x65, 0x20, 0x3D, 0x20, 0x22, 0x43, 0x2B, 0x2B, 0x22, 0x0D, 0x0A, 0x56, 0x65, 0x72, 0x73,
				0x69, 0x6F, 0x6E, 0x20, 0x3D, 0x20, 0x22, 0x32, 0x2E, 0x32, 0x2E, 0x32, 0x22, 0x00, 0x01, 0x04,
				0x06, 0x00, 0x01, 0x09, 0x3E, 0x00, 0x07, 0x0B, 0x01, 0x00, 0x01, 0x21, 0x21, 0x01, 0x00, 0x0C,
				0x3A, 0x00, 0x08, 0x0A, 0x01, 0xA4, 0x4F, 0xC0, 0xE4, 0x00, 0x00, 0x05, 0x01, 0x19, 0x0C, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x19, 0x00, 0x52, 0x00,
				0x65, 0x00, 0x63, 0x00, 0x69, 0x00, 0x70, 0x00, 0x65, 0x00, 0x2E, 0x00, 0x74, 0x00, 0x6F, 0x00,
				0x6D, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x19, 0x02, 0x00, 0x00, 0x14, 0x0A, 0x01, 0x00, 0x37, 0x66,
				0xF9, 0x60, 0xCC, 0x9A, 0xD6, 0x01, 0x15, 0x06, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
			});

			return std::string(archive.begin(), archive.end());
		}

		std::string GetTheirPackageWithDependencyArchive()
		{
			// Simple recipe file with a single dependency
			// Name = "TheirPackage"
			// Language = "C++"
			// Version = "2.2.2"
			// Dependencies = [
			//   "MyDependency@1.0.0",
			// ]
			auto archive = std::vector<unsigned char>(
			{
				0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C, 0x00, 0x04, 0xE2, 0xAB, 0x02, 0xE7, 0x62, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0xAE, 0x14, 0xD2, 
				0xE0, 0x00, 0x67, 0x00, 0x5A, 0x5D, 0x00, 0x27, 0x18, 0x49, 0xA6, 0x67, 0x41, 0x77, 0x81, 0x9A, 
				0x64, 0x61, 0x6F, 0xCB, 0x99, 0x37, 0x25, 0xF7, 0xD0, 0x58, 0xFA, 0x0C, 0xBC, 0xF1, 0x8B, 0x49, 
				0x2A, 0x4D, 0x15, 0x24, 0xD1, 0xB2, 0x55, 0x95, 0x8B, 0x37, 0x64, 0x3E, 0x54, 0xA1, 0x90, 0xC9, 
				0xB8, 0x42, 0xA9, 0x7F, 0xBA, 0x6F, 0x64, 0x14, 0xD5, 0x77, 0xBA, 0x3A, 0xB5, 0xFD, 0xC1, 0xE7, 
				0x2F, 0xC5, 0xED, 0x29, 0x4E, 0x42, 0xF1, 0xE2, 0xB9, 0xA2, 0xCF, 0x5C, 0x5F, 0x0E, 0x9D, 0x5F, 
				0x5C, 0x81, 0x97, 0x87, 0x25, 0xAA, 0xAC, 0xEA, 0xA6, 0x3E, 0x82, 0x77, 0x3A, 0x75, 0x99, 0xD8, 
				0x3A, 0x00, 0x01, 0x04, 0x06, 0x00, 0x01, 0x09, 0x62, 0x00, 0x07, 0x0B, 0x01, 0x00, 0x01, 0x21, 
				0x21, 0x01, 0x00, 0x0C, 0x68, 0x00, 0x08, 0x0A, 0x01, 0x90, 0xF6, 0xF7, 0x2E, 0x00, 0x00, 0x05, 
				0x01, 0x19, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 
				0x19, 0x00, 0x52, 0x00, 0x65, 0x00, 0x63, 0x00, 0x69, 0x00, 0x70, 0x00, 0x65, 0x00, 0x2E, 0x00, 
				0x74, 0x00, 0x6F, 0x00, 0x6D, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x19, 0x02, 0x00, 0x00, 0x14, 0x0A, 
				0x01, 0x00, 0x0E, 0xDB, 0xEE, 0x1C, 0xCD, 0x9A, 0xD6, 0x01, 0x15, 0x06, 0x01, 0x00, 0x20, 0x00, 
				0x00, 0x00, 0x00, 0x00, 
			});

			return std::string(archive.begin(), archive.end());
		}
		
		std::string GetMyDependencyArchive()
		{
			// MyDependency recipe file with a single dependency
			// Name = "MyDependency"
			// Language = "C++"
			// Version = "1.0.0"
			auto archive = std::vector<unsigned char>(
			{
				0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C, 0x00, 0x04, 0xF1, 0xC4, 0x4B, 0x22, 0x3E, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF5, 0xF5, 0xA5, 
				0x01, 0x00, 0x39, 0x4E, 0x61, 0x6D, 0x65, 0x20, 0x3D, 0x20, 0x22, 0x4D, 0x79, 0x44, 0x65, 0x70, 
				0x65, 0x6E, 0x64, 0x65, 0x6E, 0x63, 0x79, 0x22, 0x0D, 0x0A, 0x4C, 0x61, 0x6E, 0x67, 0x75, 0x61, 
				0x67, 0x65, 0x20, 0x3D, 0x20, 0x22, 0x43, 0x2B, 0x2B, 0x22, 0x0D, 0x0A, 0x56, 0x65, 0x72, 0x73, 
				0x69, 0x6F, 0x6E, 0x20, 0x3D, 0x20, 0x22, 0x31, 0x2E, 0x30, 0x2E, 0x30, 0x22, 0x00, 0x01, 0x04, 
				0x06, 0x00, 0x01, 0x09, 0x3E, 0x00, 0x07, 0x0B, 0x01, 0x00, 0x01, 0x21, 0x21, 0x01, 0x00, 0x0C, 
				0x3A, 0x00, 0x08, 0x0A, 0x01, 0x22, 0x28, 0x69, 0xC5, 0x00, 0x00, 0x05, 0x01, 0x19, 0x0C, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x19, 0x00, 0x52, 0x00, 
				0x65, 0x00, 0x63, 0x00, 0x69, 0x00, 0x70, 0x00, 0x65, 0x00, 0x2E, 0x00, 0x74, 0x00, 0x6F, 0x00, 
				0x6D, 0x00, 0x6C, 0x00, 0x00, 0x00, 0x19, 0x02, 0x00, 0x00, 0x14, 0x0A, 0x01, 0x00, 0x5B, 0x45, 
				0x42, 0xEE, 0xCC, 0x9A, 0xD6, 0x01, 0x15, 0x06, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 

			});

			return std::string(archive.begin(), archive.end());
		}
	};
}
