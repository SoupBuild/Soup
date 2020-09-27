// <copyright file="OperationGraphWriterTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class OperationGraphWriterTests
	{
	public:
		[[Fact]]
		void Serialize_Empty()
		{
			auto operationGraph = OperationGraph(
				12345,
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({}));
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x00, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		[[Fact]]
		void Serialize_SingleSimple()
		{
			auto operationGraph = OperationGraph(
				12345,
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
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
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
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		[[Fact]]
		void Serialize_SingleComplex()
		{
			auto operationGraph = OperationGraph(
				12345,
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
						std::vector<FileId>({
							1,
						}),
						std::vector<FileId>({
							2,
						}),
						std::vector<OperationId>({}),
						1,
						true,
						std::vector<FileId>({
							1,
							3,
						}),
						std::vector<FileId>({
							2,
							4,
						})),
				}));
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
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
				0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		[[Fact]]
		void Serialize_Multiple()
		{
			auto operationGraph = OperationGraph(
				12345,
				std::vector<OperationId>({
					6,
				}),
				std::vector<OperationInfo>({
					OperationInfo(
						5,
						"TestOperation1",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff1.exe"),
							"arg1 arg2"),
						std::vector<FileId>({
							1,
						}),
						std::vector<FileId>({
							2,
						}),
						std::vector<OperationId>({}),
						2,
						true,
						std::vector<FileId>({
							1,
							3,
						}),
						std::vector<FileId>({
							2,
							4,
						})),
					OperationInfo(
						6,
						"TestOperation2",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff2.exe"),
							"arg3 arg4"),
						std::vector<FileId>({
							5,
						}),
						std::vector<FileId>({
							6,
						}),
						std::vector<OperationId>({
							5,
						}),
						1,
						true,
						std::vector<FileId>({
							5,
							7,
						}),
						std::vector<FileId>({
							6,
							8,
						})),
				}));
			auto content = std::stringstream();

			OperationGraphWriter::Serialize(operationGraph, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
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
				0x02, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
				0x06, 0x00, 0x00, 0x00,
				0x0E, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', '2',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0E, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '2', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '3', ' ', 'a', 'r', 'g', '4',
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}
	};
}
