// <copyright file="OperationGraphWriterTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class OperationGraphWriterTests
	{
	public:
		// [[Fact]]
		void Serialize_Empty()
		{
			auto fileSystemState = FileSystemState();
			auto files = std::set<FileId>();
			auto operationGraph = OperationGraph({}, {});
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, files, fileSystemState, content);

			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'G', '\0', 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x00, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string((char*)binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleSimple()
		{
			auto fileSystemState = FileSystemState();
			auto files = std::set<FileId>();
			auto operationGraph = OperationGraph(
				std::vector<OperationId>({ 5, }),
				std::vector<OperationInfo>({
					OperationInfo(
						5,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						{ },
						{ },
						{ },
						{ },
						{ },
						1),
				}));
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, files, fileSystemState, content);

			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'G', '\0', 0x05, 0x00, 0x00, 0x00,
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
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
			});

			Assert::AreEqual(
				std::string((char*)binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleComplex()
		{
			auto fileSystemState = FileSystemState();
			auto files = std::set<FileId>();
			auto operationGraph = OperationGraph(
				std::vector<OperationId>({ 5, }),
				std::vector<OperationInfo>({
					OperationInfo(
						5,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1),
				}));
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, files, fileSystemState, content);

			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'G', '\0', 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0D, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string((char*)binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_Multiple()
		{
			auto fileSystemState = FileSystemState();
			auto files = std::set<FileId>();
			auto operationGraph = OperationGraph(
				std::vector<OperationId>({ 6, }),
				std::vector<OperationInfo>({
					OperationInfo(
						5,
						"TestOperation1",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff1.exe"),
							"arg1 arg2"),
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						2),
					OperationInfo(
						6,
						"TestOperation2",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff2.exe"),
							"arg3 arg4"),
						{ 5, },
						{ 6, },
						{ 5, },
						{ },
						{ },
						1),
				}));
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, files, fileSystemState, content);

			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'G', '\0', 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x06, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0E, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', '1',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0E, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '1', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
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
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '3', ' ', 'a', 'r', 'g', '4',
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
			});

			Assert::AreEqual(
				std::string((char*)binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}
	};
}
