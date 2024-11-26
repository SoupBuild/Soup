// <copyright file="OperationGraphReaderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class OperationGraphReaderTests
	{
	public:
		// [[Fact]]
		void Deserialize_InvalidFileHeaderThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationGraphReader::Deserialize(content, fileSystemState);
			});
			
			Assert::AreEqual("Invalid operation graph file header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidFileVersionThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x00, 0x00, 0x00, 0x02,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationGraphReader::Deserialize(content, fileSystemState);
			});

			Assert::AreEqual("Operation graph file version does not match expected", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidFilesHeaderThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x06, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationGraphReader::Deserialize(content, fileSystemState);
			});
			
			Assert::AreEqual("Invalid operation graph files header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidRootOperationsHeaderThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x06, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationGraphReader::Deserialize(content, fileSystemState);
			});
			
			Assert::AreEqual("Invalid operation graph operations header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidOperationsHeaderThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x06, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x00, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationGraphReader::Deserialize(content, fileSystemState);
			});

			Assert::AreEqual("Invalid operation graph operations header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_Empty()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x06, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x00, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content, fileSystemState);

			Assert::AreEqual(
				std::vector<OperationId>(),
				actual.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::map<OperationId, OperationInfo>(),
				actual.GetOperations(),
				"Verify operations match expected.");
		}

		// [[Fact]]
		void Deserialize_SingleSimple()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'G', '\0', 0x06, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0D, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x02, 0x00, 0x00, 0x00,
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1',
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '2',
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content, fileSystemState);

			Assert::AreEqual(
				std::vector<OperationId>({ 5, }),
				actual.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::map<OperationId, OperationInfo>({
					{
						5,
						OperationInfo(
							5,
							"TestOperation",
							CommandInfo(
								Path("C:/Root/"),
								Path("./DoStuff.exe"),
								{ "arg1", "arg2" }),
							{ },
							{ },
							{ },
							{ },
							{ },
							1),
					}
				}),
				actual.GetOperations(),
				"Verify operations match expected.");
		}

		// [[Fact]]
		void Deserialize_SingleComplex()
		{
			auto fileSystemState = FileSystemState(
				13,
				{
					{ 11, Path("C:/File1") },
					{ 12, Path("C:/File2") },
				});
			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'G', '\0', 0x06, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '1',
				0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '2',
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0D, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x02, 0x00, 0x00, 0x00,
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1',
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content, fileSystemState);

			Assert::AreEqual(
				std::vector<OperationId>({ 5, }),
				actual.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::map<OperationId, OperationInfo>({
					{
						5,
						OperationInfo(
							5,
							"TestOperation",
							CommandInfo(
								Path("C:/Root/"),
								Path("./DoStuff.exe"),
								{ "arg1", "arg2" }),
							{ 11, },
							{ 12, },
							{ },
							{ },
							{ },
							1),
					}
				}),
				actual.GetOperations(),
				"Verify operations match expected.");
		}

		// [[Fact]]
		void Deserialize_Multiple()
		{
			auto fileSystemState = FileSystemState(
				20,
				{
					{ 11, Path("C:/File1") },
					{ 12, Path("C:/File2") },
					{ 15, Path("C:/File5") },
					{ 16, Path("C:/File6") },
				});
			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'G', '\0', 0x06, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x04, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '1',
				0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '2',
				0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '5',
				0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '6',
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x06, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0E, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', '1',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0E, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '1', '.', 'e', 'x', 'e',
				0x02, 0x00, 0x00, 0x00,
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1',
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00,
				0x06, 0x00, 0x00, 0x00,
				0x0E, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', '2',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0E, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '2', '.', 'e', 'x', 'e',
				0x02, 0x00, 0x00, 0x00,
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '3',
				0x04, 0x00, 0x00, 0x00, 'a', 'r', 'g', '4',
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content, fileSystemState);

			auto expected = std::map<OperationId, OperationInfo>(
			{
				{
					5,
					OperationInfo(
						5,
						"TestOperation1",
						CommandInfo(
							Path("C:/Root/"),
							Path("./DoStuff1.exe"),
							{ "arg1", "arg2" }),
						{ 11, },
						{ 12, },
						{ },
						{ },
						{ },
						2),
				},
				{
					6,
					OperationInfo(
						6,
						"TestOperation2",
						CommandInfo(
							Path("C:/Root/"),
							Path("./DoStuff2.exe"),
							{ "arg3", "arg4" }),
						{ 15, },
						{ 16, },
						{ 15, },
						{ },
						{ },
						1),
				},
			});

			Assert::AreEqual(
				std::vector<OperationId>({ 6, }),
				actual.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				expected,
				actual.GetOperations(),
				"Verify operations match expected.");
		}
	};
}
