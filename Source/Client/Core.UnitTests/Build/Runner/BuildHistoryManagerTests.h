// <copyright file="BuildHistoryManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class BuildHistoryManagerTests
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

			auto directory = Path("TestFiles/NoFile");
			BuildHistory actual;
			auto result = BuildHistoryManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/NoFile/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: BuildHistory file does not exist",
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
				Path("TestFiles/GarbageBuildHistory/.soup/BuildHistory.json"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto directory = Path("TestFiles/GarbageBuildHistory");
			BuildHistory actual;
			auto result = BuildHistoryManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/GarbageBuildHistory/.soup/BuildHistory.json",
					"OpenRead: TestFiles/GarbageBuildHistory/.soup/BuildHistory.json",
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
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleBuildHistory/.soup/BuildHistory.json"),
				std::make_shared<MockFile>(std::stringstream(R"({
					"knownFiles": [
						{
							"file": "File.h",
							"includes": [ "Other.h" ]
						}
					]
				})")));

			auto directory = Path("TestFiles/SimpleBuildHistory");
			BuildHistory actual;
			auto result = BuildHistoryManager::TryLoadState(directory, actual);

			Assert::IsTrue(result, "Verify result is false.");

			auto expected = BuildHistory({
					FileInfo(Path("File.h"), { Path("Other.h") }),
				});

			Assert::AreEqual(expected, actual, "Verify matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: TestFiles/SimpleBuildHistory/.soup/BuildHistory.json",
					"OpenRead: TestFiles/SimpleBuildHistory/.soup/BuildHistory.json",
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
