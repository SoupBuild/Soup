// <copyright file="OperationHistoryReaderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class OperationHistoryReaderTests
	{
	public:
		[[Fact]]
		void Deserialize_InvalidFileHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationHistoryReader::Deserialize(content);
			},
			"Invalid operation history file header");
		}

		[[Fact]]
		void Deserialize_InvalidFileVersionThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x00, 0x00, 0x00, 0x02,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationHistoryReader::Deserialize(content);
			},
			"Operation History file version does not match expected");
		}

		[[Fact]]
		void Deserialize_InvalidOperationsHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '2'
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationHistoryReader::Deserialize(content);
			},
			"Invalid operation history operations header");
		}

		[[Fact]]
		void Deserialize_ExtraContentThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00, 'E',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationHistoryReader::Deserialize(content);
			},
			"Operation History file corrupted - Did not read the entire file");
		}

		[[Fact]]
		void Deserialize_Empty()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationHistoryReader::Deserialize(content);

			Assert::AreEqual(
				std::unordered_map<CommandInfo, OperationInfo>(),
				actual.GetOperations(),
				"Verify operations match expected.");
		}

		[[Fact]]
		void Deserialize_Single()
		{
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
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationHistoryReader::Deserialize(content);

			Assert::AreEqual(
				std::unordered_map<CommandInfo, OperationInfo>({
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
				}),
				actual.GetOperations(),
				"Verify operations match expected.");
		}

		[[Fact]]
		void Deserialize_Multiple()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'H', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0C, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '1', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0C, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '2', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationHistoryReader::Deserialize(content);

			Assert::AreEqual(
				std::unordered_map<CommandInfo, OperationInfo>({
					{
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff1.exe"),
							"arg1 arg2"),
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
					},
					{
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff2.exe"),
							"arg1 arg2"),
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
					},
				}),
				actual.GetOperations(),
				"Verify operations match expected.");
		}
	};
}
