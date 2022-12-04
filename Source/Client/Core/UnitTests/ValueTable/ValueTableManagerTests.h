// <copyright file="ValueTableManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class ValueTableManagerTests
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

			auto directory = Path("TestFiles/NoFile/.soup/ValueTable.bin");
			auto actual = ValueTable();
			auto result = ValueTableManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/NoFile/.soup/ValueTable.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Value Table file does not exist",
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
				Path("TestFiles/GarbageValueTable/.soup/ValueTable.bin"),
				std::make_shared<MockFile>(std::stringstream("garbage")));

			auto directory = Path("TestFiles/GarbageValueTable/.soup/ValueTable.bin");
			auto actual = ValueTable();
			auto result = ValueTableManager::TryLoadState(directory, actual);

			Assert::IsFalse(result, "Verify result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/GarbageValueTable/.soup/ValueTable.bin",
					"OpenReadBinary: ./TestFiles/GarbageValueTable/.soup/ValueTable.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"ERRO: Invalid Value Table file header",
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
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			});
			fileSystem->CreateMockFile(
				Path("TestFiles/SimpleValueTable/.soup/ValueTable.bin"),
				std::make_shared<MockFile>(std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()))));

			auto directory = Path("TestFiles/SimpleValueTable/.soup/ValueTable.bin");
			auto actual = ValueTable();
			auto result = ValueTableManager::TryLoadState(directory, actual);

			Assert::IsTrue(result, "Verify result is true.");

			// Verify value table matches expected
			Assert::AreEqual(
				ValueTable(
				{
					{ "TestValue", Value(false) },
				}),
				actual,
				"Verify value table match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/SimpleValueTable/.soup/ValueTable.bin",
					"OpenReadBinary: ./TestFiles/SimpleValueTable/.soup/ValueTable.bin",
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

			auto valueTableFile = Path("TestFiles/.soup/ValueTable.bin");
			auto valueTable = ValueTable(
			{
				{ "TestValue", Value(false) },
			});
			ValueTableManager::SaveState(valueTableFile, valueTable);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: ./TestFiles/.soup/",
					"CreateDirectory: ./TestFiles/.soup/",
					"OpenWriteBinary: ./TestFiles/.soup/ValueTable.bin",
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
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			});
			auto mockFile = fileSystem->GetMockFile(Path("./TestFiles/.soup/ValueTable.bin"));
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				mockFile->Content.str(),
				"Verify file content match expected.");
		}
	};
}
