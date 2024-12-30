// <copyright file="OperationResultsWriterTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
using namespace std::chrono;

namespace Soup::Core::UnitTests
{
	class OperationResultsWriterTests
	{
	public:
		// [[Fact]]
		void Serialize_Empty()
		{
			auto fileSystemState = FileSystemState();
			auto files = std::set<FileId>();
			auto operationResults = OperationResults();
			auto content = std::stringstream();

			OperationResultsWriter::Serialize(operationResults, files, fileSystemState, content);

			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'T', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
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
			auto content = std::stringstream();

			OperationResultsWriter::Serialize(operationResults, files, fileSystemState, content);

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
			auto operationResults = OperationResults({
				{
					5,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::sys_days(March/5/2020) + 12h + 35min + 34s + 1ms),
						{ 1, 3, },
						{ 2, 4, })
				},
			});
			auto content = std::stringstream();

			OperationResultsWriter::Serialize(operationResults, files, fileSystemState, content);

			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'T', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x10, 0x16, 0x62, 0xbb, 0x0b, 0x41, 0x38, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
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
			auto operationResults = OperationResults({
				{
					5,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::sys_days(March/5/2020) + 12h + 35min + 34s + 1ms),
						{ 1, 3, },
						{ 2, 4, })
				},
				{
					6,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::sys_days(March/5/2020) + 12h + 36min + 55s),
						{ 5, 7, },
						{ 6, 8, })
				},
			});
			auto content = std::stringstream();

			OperationResultsWriter::Serialize(operationResults, files, fileSystemState, content);

auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'T', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x10, 0x16, 0x62, 0xbb, 0x0b, 0x41, 0x38, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
				0x06, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x80, 0x8d, 0xa9, 0xeb, 0x0b, 0x41, 0x38, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
			});

			Assert::AreEqual(
				std::string((char*)binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}
	};
}
