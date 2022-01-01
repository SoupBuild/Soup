// <copyright file="OperationGraphManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class OperationGraphManagerTests
	{
	public:
		// [[Fact]]
		void TryLoadFromFile_MissingFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto filePath = Path("TestFiles/NoFile/.soup/OperationGraph.bin");
			auto fileSystemState = std::make_shared<FileSystemState>();
			auto actual = OperationGraph();
			auto result = OperationGraphManager::TryLoadState(filePath, actual, *fileSystemState);

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
					"INFO: Operation graph file does not exist",
				}),
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		// [[Fact]]
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

			auto filePath = Path("TestFiles/GarbageOperationGraph/.soup/OperationGraph.bin");
			auto fileSystemState = std::make_shared<FileSystemState>();
			auto actual = OperationGraph();
			auto result = OperationGraphManager::TryLoadState(filePath, actual, *fileSystemState);

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

		// [[Fact]]
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
				'B', 'O', 'G', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0D, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
			});
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin"),
				std::make_shared<MockFile>(std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()))));

			auto filePath = Path("TestFiles/SimpleOperationGraph/.soup/OperationGraph.bin");
			auto fileSystemState = std::make_shared<FileSystemState>();
			auto actual = OperationGraph();
			auto result = OperationGraphManager::TryLoadState(filePath, actual, *fileSystemState);

			Assert::IsTrue(result, "Verify result is true.");

			// Verify operation graph matches expected
			Assert::AreEqual(
				std::vector<OperationId>({
					5,
				}),
				actual.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationInfo>({
					{
						5,
						OperationInfo(
							5,
							"TestOperation",
							CommandInfo(
								Path("C:/Root/"),
								Path("DoStuff.exe"),
								"arg1 arg2"),
							std::vector<FileId>({}),
							std::vector<FileId>({}),
							std::vector<OperationId>({}),
							1,
							false,
							std::vector<FileId>({}),
							std::vector<FileId>({})),
					}
				}),
				actual.GetOperations(),
				"Verify operations match expected.");

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

		// [[Fact]]
		void SaveState()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto fileSystemState = std::make_shared<FileSystemState>();
			auto filePath = Path("TestFiles/.soup/OperationGraph.bin");
			auto operationGraph = OperationGraph(
				{},
				std::vector<OperationId>({
					5,
				}),
				std::vector<OperationInfo>({
					OperationInfo(
						5,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({}),
						std::vector<FileId>({}),
						std::vector<OperationId>({}),
						1,
						false,
						std::vector<FileId>({}),
						std::vector<FileId>({})),
				}));
			OperationGraphManager::SaveState(filePath, operationGraph, *fileSystemState);

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
				'B', 'O', 'G', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0D, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
			});
			auto mockFile = fileSystem->GetMockFile(Path("./TestFiles/.soup/OperationGraph.bin"));
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				mockFile->Content.str(),
				"Verify file content match expected.");
		}
	};
}
