// <copyright file="OperationGraphReaderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class OperationGraphReaderTests
	{
	public:
		[[Fact]]
		void Deserialize_InvalidFileHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationGraphReader::Deserialize(content);
			},
			"Invalid operation graph file header");
		}

		[[Fact]]
		void Deserialize_InvalidFileVersionThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x00, 0x00, 0x00, 0x02,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationGraphReader::Deserialize(content);
			},
			"Operation graph file version does not match expected");
		}

		[[Fact]]
		void Deserialize_InvalidRootOperationsHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationGraphReader::Deserialize(content);
			},
			"Invalid operation graph operations header");
		}

		[[Fact]]
		void Deserialize_InvalidOperationsHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x00, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationGraphReader::Deserialize(content);
			},
			"Invalid operation graph operations header");
		}

		[[Fact]]
		void Deserialize_ExtraContentThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x00, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00, 'E',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = OperationGraphReader::Deserialize(content);
			},
			"Operation graph file corrupted - Did not read the entire file");
		}

		[[Fact]]
		void Deserialize_Empty()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x00, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content);

			Assert::AreEqual(
				12345u,
				actual.GetStateId(),
				"Verify state id match expected.");
			Assert::AreEqual(
				std::vector<OperationId>(),
				actual.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationInfo>(),
				actual.GetOperations(),
				"Verify operations match expected.");
		}

		[[Fact]]
		void Deserialize_SingleSimple()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0D, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0B, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content);

			Assert::AreEqual(
				12345u,
				actual.GetStateId(),
				"Verify state id match expected.");
			Assert::AreEqual(
				std::vector<OperationId>({ 5, }),
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
		}

		[[Fact]]
		void Deserialize_SingleComplex()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0D, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0B, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '1', ' ', 'a', 'r', 'g', '2',
				0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content);

			Assert::AreEqual(
				12345u,
				actual.GetStateId(),
				"Verify state id match expected.");
			Assert::AreEqual(
				std::vector<OperationId>({ 5, }),
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
							std::vector<FileId>({ 1, }),
							std::vector<FileId>({ 2, }),
							std::vector<OperationId>({}),
							1,
							true,
							std::vector<FileId>({ 1, 3, }),
							std::vector<FileId>({ 2, 4, })),
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
				'B', 'O', 'G', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00,
				'R', 'O', 'P', '\0', 0x01, 0x00, 0x00, 0x00,
				0x06, 0x00, 0x00, 0x00,
				'O', 'P', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x0E, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', '1',
				0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/',
				0x0C, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '1', '.', 'e', 'x', 'e',
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
				0x0C, 0x00, 0x00, 0x00, 'D', 'o', 'S', 't', 'u', 'f', 'f', '2', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 'a', 'r', 'g', '3', ' ', 'a', 'r', 'g', '4',
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationGraphReader::Deserialize(content);

			Assert::AreEqual(
				12345u,
				actual.GetStateId(),
				"Verify state id match expected.");
			Assert::AreEqual(
				std::vector<OperationId>({ 6, }),
				actual.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationInfo>({
					{
						5,
						OperationInfo(
							5,
							"TestOperation1",
							CommandInfo(
								Path("C:/Root/"),
								Path("DoStuff1.exe"),
								"arg1 arg2"),
							std::vector<FileId>({ 1, }),
							std::vector<FileId>({ 2, }),
							std::vector<OperationId>({}),
							2,
							true,
							std::vector<FileId>({ 1, 3, }),
							std::vector<FileId>({ 2, 4, })),
					},
					{
						6,
						OperationInfo(
							6,
							"TestOperation2",
							CommandInfo(
								Path("C:/Root/"),
								Path("DoStuff2.exe"),
								"arg3 arg4"),
							std::vector<FileId>({ 5, }),
							std::vector<FileId>({ 6, }),
							std::vector<OperationId>({ 5, }),
							1,
							true,
							std::vector<FileId>({ 5, 7, }),
							std::vector<FileId>({ 6, 8, })),
					},
				}),
				actual.GetOperations(),
				"Verify operations match expected.");
		}
	};
}
