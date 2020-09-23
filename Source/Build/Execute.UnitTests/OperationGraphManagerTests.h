// <copyright file="OperationGraphManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class OperationGraphManagerTests
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
			auto actual = OperationGraph(0);
			auto result = OperationGraphManager::TryLoadState(directory, actual, 1234);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/NoFile/.soup/OperationGraph.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"HIGH: Operation graph file does not exist",
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
				Path("TestFiles/GarbageOperationGraph/.soup/OperationGraph.bin"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto directory = Path("TestFiles/GarbageOperationGraph");
			auto actual = OperationGraph(0);
			auto result = OperationGraphManager::TryLoadState(directory, actual, 1234);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/GarbageOperationGraph/.soup/OperationGraph.bin",
					"OpenReadBinary: ./TestFiles/GarbageOperationGraph/.soup/OperationGraph.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"ERRO: Invalid operation graph file header",
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
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0B, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			});
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin"),
				std::make_shared<MockFile>(std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()))));

			auto directory = Path("TestFiles/SimpleOperationGraph");
			auto actual = OperationGraph(0);
			auto result = OperationGraphManager::TryLoadState(directory, actual, 22);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin",
					"OpenReadBinary: ./TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"WARN: Operation graph uses an out of date state Id",
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
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0B, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			});
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin"),
				std::make_shared<MockFile>(std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()))));

			auto directory = Path("TestFiles/SimpleOperationGraph");
			auto actual = OperationGraph(0);
			auto result = OperationGraphManager::TryLoadState(directory, actual, 12345);

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
					"Exists: ./TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin",
					"OpenReadBinary: ./TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin",
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
			auto operationGraph = OperationGraph(
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
			OperationGraphManager::SaveState(directory, operationGraph);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/.soup/",
					"CreateDirectory: ./TestFiles/.soup/",
					"OpenWriteBinary: ./TestFiles/.soup/OperationGraph.bin",
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
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			});
			auto& mockFile = fileSystem->GetMockFile(Path("./TestFiles/.soup/OperationGraph.bin"));
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				mockFile->Content.str(),
				"Verify file content match expected.");
		}
	};
}
