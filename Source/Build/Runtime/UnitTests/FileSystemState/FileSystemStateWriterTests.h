// <copyright file="FileSystemStateWriterTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class FileSystemStateWriterTests
	{
	public:
		// [[Fact]]
		void Serialize_Empty()
		{
			auto fileSystemState = FileSystemState(
				12345,
				5,
				std::unordered_map<FileId, Path>({}));
			auto content = std::stringstream();

			FileSystemStateWriter::Serialize(fileSystemState, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_Single()
		{
			auto fileSystemState = FileSystemState(
				12345,
				5,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					},
				}));
			auto content = std::stringstream();

			FileSystemStateWriter::Serialize(fileSystemState, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '.', 'e', 'x', 'e',
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_Multiple()
		{
			auto fileSystemState = FileSystemState(
				12345,
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff1.exe"),
					},
					{
						9,
						Path("C:/Root/DoStuff2.exe"),
					},
				}));
			auto content = std::stringstream();

			FileSystemStateWriter::Serialize(fileSystemState, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'F', 'S', '\0', 0x01, 0x00, 0x00, 0x00, 0x39, 0x30, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x08, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '1', '.', 'e', 'x', 'e',
				0x09, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 'C', ':', '/', 'R', 'o', 'o', 't', '/', 'D', 'o', 'S', 't', 'u', 'f', 'f', '2', '.', 'e', 'x', 'e',
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}
	};
}
