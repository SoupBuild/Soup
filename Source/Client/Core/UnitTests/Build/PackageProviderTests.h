// <copyright file="PackageProviderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class PackageProviderTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto uut = PackageProvider();
		}

		// [[Fact]]
		void Initialize_NoDependencies()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
				)")));

			auto uut = PackageProvider();

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			uut.Initialize(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist.",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
		
		// [[Fact]]
		void Initialize_BuildDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"TestBuild@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "C#|1"
				)")));

			auto uut = PackageProvider();

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			uut.Initialize(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist.",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		// [[Fact]]
		void Initialize_TriangleDependency_NoRebuild()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Runtime: [
							"PackageA@1.2.3"
							"PackageB@1.1.1"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageA"
					Language: "C++|1"
					Dependencies: {
						Runtime: [
							"PackageB@1.1.1"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageB"
					Language: "C++|1"
				)")));

			auto uut = PackageProvider();

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			uut.Initialize(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist.",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"DIAG: Recipe closure already loaded: C++|PackageB",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"GetCurrentDirectory",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		// [[Fact]]
		void Initialize_PackageLock_OverrideBuildDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"TestBuild@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "C#|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 2
					Closures: {
						Root: {
							C#: [
								{ Name: "TestBuild", Version: "1.3.0", Build: "Build1" }
							]
							C++: [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
						}
						Build0: {
							C#: [
								{ Name: "C++", Version: "1.0.2" }
								{ Name: "TestBuild", Version: "1.3.0" }
							]
						}
						Build1: {
							C#: [
								{ Name: "C#", Version: "1.0.1" }
							]
						}
					}
				)")));

			auto uut = PackageProvider();

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			uut.Initialize(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
