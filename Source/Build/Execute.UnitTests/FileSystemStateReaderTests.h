// <copyright file="FileSystemStateReaderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class FileSystemStateReaderTests
	{
	public:
		[[Fact]]
		void Deserialize_InvalidFileHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = FileSystemStateReader::Deserialize(content);
			},
			"Invalid file system state file header");
		}

		[[Fact]]
		void Deserialize_InvalidFileVersionThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x00, 0x00, 0x00, 0x02,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = FileSystemStateReader::Deserialize(content);
			},
			"File System state file version does not match expected");
		}

		[[Fact]]
		void Deserialize_InvalidOperationsHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '2'
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = FileSystemStateReader::Deserialize(content);
			},
			"Invalid file system state files header");
		}

		[[Fact]]
		void Deserialize_ExtraContentThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00, 'E',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = FileSystemStateReader::Deserialize(content);
			},
			"File System State file corrupted - Did not read the entire file");
		}

		[[Fact]]
		void Deserialize_Empty()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = FileSystemStateReader::Deserialize(content);

			Assert::AreEqual(12345u, actual.GetId(), "Verify id matches expected.");
			Assert::AreEqual(5u, actual.GetMaxFileId(), "Verify max file id matches expected.");
			Assert::AreEqual(
				std::unordered_map<FileId, Path>(),
				actual.GetFiles(),
				"Verify files match expected.");
		}

		[[Fact]]
		void Deserialize_Single()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = FileSystemStateReader::Deserialize(content);

			Assert::AreEqual(12345u, actual.GetId(), "Verify id matches expected.");
			Assert::AreEqual(5u, actual.GetMaxFileId(), "Verify max file id matches expected.");
			Assert::AreEqual(
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}
				}),
				actual.GetFiles(),
				"Verify files match expected.");
		}

		[[Fact]]
		void Deserialize_Multiple()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '1', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '2', '.', 'e', 'x', 'e',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = FileSystemStateReader::Deserialize(content);

			Assert::AreEqual(12345u, actual.GetId(), "Verify id matches expected.");
			Assert::AreEqual(5u, actual.GetMaxFileId(), "Verify max file id matches expected.");
			Assert::AreEqual(
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff1.exe"),
					},
					{
						9,
						Path("C:/Root/DoStuff2.exe"),
					},
				}),
				actual.GetFiles(),
				"Verify files match expected.");
		}
	};
}
