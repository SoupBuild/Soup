// <copyright file="LocalUserConfigExtensionsTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class LocalUserConfigExtensionsTests
	{
	public:
		// [[Fact]]
		void TryLoadLocalUserConfigFromFile_MissingFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto directory = Path("TestFiles/NoFile/LocalUserConfig.sml");
			LocalUserConfig actual;
			auto result = LocalUserConfigExtensions::TryLoadLocalUserConfigFromFile(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/NoFile/LocalUserConfig.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: ./TestFiles/NoFile/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}
		
		// [[Fact]]
		void TryLoadLocalUserConfigFromFile_GarbageFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("TestFiles/GarbageLocalUserConfig/LocalUserConfig.sml"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto directory = Path("TestFiles/GarbageLocalUserConfig/LocalUserConfig.sml");
			LocalUserConfig actual;
			auto result = LocalUserConfigExtensions::TryLoadLocalUserConfigFromFile(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/GarbageLocalUserConfig/LocalUserConfig.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: ./TestFiles/GarbageLocalUserConfig/LocalUserConfig.sml",
					"ERRO: Deserialize Threw: Parsing the Recipe SML failed: Failed to parse at 1:7  ./TestFiles/GarbageLocalUserConfig/LocalUserConfig.sml",
					"INFO: Failed to parse local user config.",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		// [[Fact]]
		void TryLoadLocalUserConfigFromFile_SimpleFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleLocalUserConfig/LocalUserConfig.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					)")));

			auto directory = Path("TestFiles/SimpleLocalUserConfig/LocalUserConfig.sml");
			LocalUserConfig actual;
			auto result = LocalUserConfigExtensions::TryLoadLocalUserConfigFromFile(directory, actual);

			Assert::IsTrue(result, "Verify result is false.");

			auto expected = LocalUserConfig();

			Assert::AreEqual(expected, actual, "Verify matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/SimpleLocalUserConfig/LocalUserConfig.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: ./TestFiles/SimpleLocalUserConfig/LocalUserConfig.sml",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}
	};
}
