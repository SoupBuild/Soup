// <copyright file="BuildStateManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class BuildStateManagerTests
	{
	public:
		[[Fact]]
		void TryLoadFromFile_MissingFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			auto directory = Path("TestFiles/NoFile");
			BuildState actual;
			auto result = BuildStateManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/NoFile/.soup/BuildState.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: BuildState file does not exist",
				}),
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void TryLoadFromFile_GarbageFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);
			fileSystem->CreateMockFile(
				Path("TestFiles/GarbageBuildState/.soup/BuildState.json"),
				MockFileState(std::stringstream("garbage")));

			auto directory = Path("TestFiles/GarbageBuildState");
			BuildState actual;
			auto result = BuildStateManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/GarbageBuildState/.soup/BuildState.json",
					"OpenRead: TestFiles/GarbageBuildState/.soup/BuildState.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Failed to parse the build state json: expected value, got 'g' (103)",
				}),
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void TryLoadFromFile_SimpleFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleBuildState/.soup/BuildState.json"),
				MockFileState(std::stringstream(R"({
					"knownFiles": [
						{
							"file": "File.h",
							"includes": [ "Other.h" ]
						}
					]
				})")));

			auto directory = Path("TestFiles/SimpleBuildState");
			BuildState actual;
			auto result = BuildStateManager::TryLoadState(directory, actual);

			Assert::IsTrue(result, "Verify result is false.");

			auto expected = BuildState({
					FileInfo(Path("File.h"), { Path("Other.h") }),
				});

			Assert::AreEqual(expected, actual, "Verify matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/SimpleBuildState/.soup/BuildState.json",
					"OpenRead: TestFiles/SimpleBuildState/.soup/BuildState.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
				}),
				testListener->GetMessages(),
				"Verify messages match expected.");
		}
	};
}
