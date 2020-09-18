// <copyright file="OperationHistoryManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class OperationHistoryManagerTests
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
			auto actual = OperationHistory(0);
			auto result = OperationHistoryManager::TryLoadState(directory, actual, 1234);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/NoFile/.soup/OperationHistory.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"HIGH: Operation History file does not exist",
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
				Path("TestFiles/GarbageOperationHistory/.soup/OperationHistory.bin"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto directory = Path("TestFiles/GarbageOperationHistory");
			auto actual = OperationHistory(0);
			auto result = OperationHistoryManager::TryLoadState(directory, actual, 1234);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/GarbageOperationHistory/.soup/OperationHistory.bin",
					"OpenReadBinary: ./TestFiles/GarbageOperationHistory/.soup/OperationHistory.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"ERRO: Invalid operation history file header",
				}),
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void TryLoadFromFile_WrongStateId()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0B, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			});
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleOperationHistory/.soup/OperationHistory.bin"),
				std::make_shared<MockFile>(std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()))));

			auto directory = Path("TestFiles/SimpleOperationHistory");
			auto actual = OperationHistory(0);
			auto result = OperationHistoryManager::TryLoadState(directory, actual, 22);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/SimpleOperationHistory/.soup/OperationHistory.bin",
					"OpenReadBinary: ./TestFiles/SimpleOperationHistory/.soup/OperationHistory.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"WARN: Operation History uses an out of date state Id",
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
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0B, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			});
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleOperationHistory/.soup/OperationHistory.bin"),
				std::make_shared<MockFile>(std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()))));

			auto directory = Path("TestFiles/SimpleOperationHistory");
			auto actual = OperationHistory(0);
			auto result = OperationHistoryManager::TryLoadState(directory, actual, 12345);

			Assert::IsTrue(result, "Verify result is true.");

			auto expected = std::unordered_map<CommandInfo, OperationInfo>({
				{
					CommandInfo(
						Path("C:/Root/"),
						Path("DoStuff.exe"),
						"arg1 arg2"),
					OperationInfo(
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({
							1,
						}),
						std::vector<FileId>({
							2,
						})),
				}
			});
			Assert::AreEqual(expected, actual.GetOperations(), "Verify operations matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/SimpleOperationHistory/.soup/OperationHistory.bin",
					"OpenReadBinary: ./TestFiles/SimpleOperationHistory/.soup/OperationHistory.bin",
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
			auto operationHistory = OperationHistory(
				12345,
				std::vector<OperationInfo>({
					OperationInfo(
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({
							1,
						}),
						std::vector<FileId>({
							2,
						})),
				}));
			OperationHistoryManager::SaveState(directory, operationHistory);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/.soup/",
					"CreateDirectory: ./TestFiles/.soup/",
					"OpenWriteBinary: ./TestFiles/.soup/OperationHistory.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Create Directory: ./TestFiles/.soup/",
				}),
				testListener->GetMessages(),
				"Verify messages match expected.");

			// Verify the file content
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			});
			auto& mockFile = fileSystem->GetMockFile(Path("./TestFiles/.soup/OperationHistory.bin"));
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				mockFile->Content.str(),
				"Verify file content match expected.");
		}
	};
}
