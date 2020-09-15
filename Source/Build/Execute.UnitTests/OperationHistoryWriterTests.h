// <copyright file="OperationHistoryWriterTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class OperationHistoryWriterTests
	{
	public:
		[[Fact]]
		void Serialize_Empty()
		{
			auto operationHistory = OperationHistory(
				std::vector<OperationInfo>({}));
			auto content = std::stringstream();

			OperationHistoryWriter::Serialize(operationHistory, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		[[Fact]]
		void Serialize_Single()
		{
			auto operationHistory = OperationHistory(
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
			auto content = std::stringstream();

			OperationHistoryWriter::Serialize(operationHistory, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0D, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		[[Fact]]
		void Serialize_Multiple()
		{
			auto operationHistory = OperationHistory(
				std::vector<OperationInfo>({
					OperationInfo(
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff1.exe"),
							"arg1 arg2"),
						std::vector<FileId>({
							1,
						}),
						std::vector<FileId>({
							2,
						})),
					OperationInfo(
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff2.exe"),
							"arg1 arg2"),
						std::vector<FileId>({
							3,
						}),
						std::vector<FileId>({
							4,
						})),
				}));
			auto content = std::stringstream();

			OperationHistoryWriter::Serialize(operationHistory, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 'O', 'P', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0E, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '1', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0E, 0x00, 0x00, 0x00, '.', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '2', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}
	};
}
