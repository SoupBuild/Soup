// <copyright file="OperationResultsManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class OperationResultsManagerTests
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

			auto filePath = Path("./TestFiles/NoFile/.soup/OperationResults.bor");
			auto fileSystemState = std::make_shared<FileSystemState>();
			auto actual = OperationResults();
			auto result = OperationResultsManager::TryLoadState(filePath, actual, *fileSystemState);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/NoFile/.soup/OperationResults.bor",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Operation results file does not exist",
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
				Path("./TestFiles/GarbageOperationResults/.soup/OperationResults.bor"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto filePath = Path("./TestFiles/GarbageOperationResults/.soup/OperationResults.bor");
			auto fileSystemState = std::make_shared<FileSystemState>();
			auto actual = OperationResults();
			auto result = OperationResultsManager::TryLoadState(filePath, actual, *fileSystemState);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/GarbageOperationResults/.soup/OperationResults.bor",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"ERRO: Invalid operation results file header",
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

			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'T', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
			});
			fileSystem->CreateMockFile(
				Path("./TestFiles/SimpleOperationResults/.soup/OperationResults.bor"),
				std::make_shared<MockFile>(std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()))));

			auto filePath = Path("./TestFiles/SimpleOperationResults/.soup/OperationResults.bor");
			auto fileSystemState = std::make_shared<FileSystemState>();
			auto actual = OperationResults();
			auto result = OperationResultsManager::TryLoadState(filePath, actual, *fileSystemState);

			Assert::IsTrue(result, "Verify result is true.");

			// Verify operation Results matches expected
			Assert::AreEqual(
				std::map<OperationId, OperationResult>({
					{
						5,
						OperationResult(
							false,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::time_point<std::chrono::system_clock>()),
							{ },
							{ })
					},
				}),
				actual.GetResults(),
				"Verify results match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"TryOpenReadBinary: ./TestFiles/SimpleOperationResults/.soup/OperationResults.bor",
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
			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto fileSystemState = std::make_shared<FileSystemState>();
			auto filePath = Path("./TestFiles/.soup/OperationResults.bor");
			auto operationResults = OperationResults({
				{
					5,
					OperationResult(
						false,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::time_point<std::chrono::system_clock>()),
						{ },
						{ })
				},
			});
			OperationResultsManager::SaveState(filePath, operationResults, *fileSystemState);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"OpenWriteBinary: ./TestFiles/.soup/OperationResults.bor",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify the file content
			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'T', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
			});
			auto mockFile = fileSystem->GetMockFile(Path("./TestFiles/.soup/OperationResults.bor"));
			Assert::AreEqual(
				std::string((char*)binaryFileContent.data(), binaryFileContent.size()),
				mockFile->Content.str(),
				"Verify file content match expected.");
		}
	};
}
