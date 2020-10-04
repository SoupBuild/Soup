// <copyright file="FileSystemStateManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class FileSystemStateManagerTests
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
			auto actual = FileSystemState(0);
			auto result = FileSystemStateManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/NoFile/State/FileSystemState.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"HIGH: File system state file does not exist",
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
				Path("TestFiles/GarbageFileSystemState/State/FileSystemState.bin"),
				std::make_shared<MockFile>());

			auto directory = Path("TestFiles/GarbageFileSystemState");
			auto actual = FileSystemState(0);
			auto result = FileSystemStateManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/GarbageFileSystemState/State/FileSystemState.bin",
					"OpenReadBinary: ./TestFiles/GarbageFileSystemState/State/FileSystemState.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"WARN: Invalid file system state file header",
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

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
			});
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleFileSystemState/State/FileSystemState.bin"),
				std::make_shared<MockFile>(std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()))));

			auto directory = Path("TestFiles/SimpleFileSystemState");
			auto actual = FileSystemState(0);
			auto result = FileSystemStateManager::TryLoadState(directory, actual);

			Assert::IsTrue(result, "Verify result is true.");

			Assert::AreEqual(12345u, actual.GetId(), "Verify id matches expected.");
			Assert::AreEqual(10u, actual.GetMaxFileId(), "Verify max file id matches expected.");
			auto expected = std::unordered_map<FileId, Path>({
				{
					8,
					Path("C:/Root/DoStuff.exe"),
				}
			});
			Assert::AreEqual(expected, actual.GetFiles(), "Verify files matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/SimpleFileSystemState/State/FileSystemState.bin",
					"OpenReadBinary: ./TestFiles/SimpleFileSystemState/State/FileSystemState.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void SaveState()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto directory = Path("TestFiles/");
			auto fileSystemState = FileSystemState(
				12345,
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					},
				}));
			FileSystemStateManager::SaveState(directory, fileSystemState);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/State/",
					"CreateDirectory: ./TestFiles/State/",
					"OpenWriteBinary: ./TestFiles/State/FileSystemState.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Create Directory: ./TestFiles/State/",
				}),
				testListener->GetMessages(),
				"Verify messages match expected.");

			// Verify the file content
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
			});
			auto& mockFile = fileSystem->GetMockFile(Path("./TestFiles/State/FileSystemState.bin"));
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				mockFile->Content.str(),
				"Verify file content match expected.");
		}
	};
}
