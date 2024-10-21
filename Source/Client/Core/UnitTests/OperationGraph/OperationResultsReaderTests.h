// <copyright file="OperationResultsReaderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
using namespace std::chrono;

namespace Soup::Core::UnitTests
{
	class OperationResultsReaderTests
	{
	public:
		// [[Fact]]
		void Deserialize_InvalidFileHeaderThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'R', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationResultsReader::Deserialize(content, fileSystemState);
			});
			
			Assert::AreEqual("Invalid operation results file header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidFileVersionThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'R', '\0', 0x00, 0x00, 0x00, 0x02,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationResultsReader::Deserialize(content, fileSystemState);
			});

			Assert::AreEqual("Operation results file version does not match expected", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidFilesHeaderThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationResultsReader::Deserialize(content, fileSystemState);
			});
			
			Assert::AreEqual("Invalid operation results files header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidResultsHeaderThrows()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'T', 'S', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content, &fileSystemState]() {
				auto actual = OperationResultsReader::Deserialize(content, fileSystemState);
			});

			Assert::AreEqual("Invalid operation results results header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_Empty()
		{
			auto fileSystemState = FileSystemState();
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
				'R', 'T', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationResultsReader::Deserialize(content, fileSystemState);

			Assert::AreEqual(
				std::map<OperationId, OperationResult>(),
				actual.GetResults(),
				"Verify results match expected.");
		}

		// [[Fact]]
		void Deserialize_SingleSimple()
		{
			auto fileSystemState = FileSystemState();
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
			auto content = std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationResultsReader::Deserialize(content, fileSystemState);

			Assert::AreEqual(
				std::map<OperationId, OperationResult>({
					{
						5,
						OperationResult(
							false,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::time_point<std::chrono::system_clock>()),
							{ },
							{ })
					},
				}),
				actual.GetResults(),
				"Verify results match expected.");
		}

		// [[Fact]]
		void Deserialize_SingleComplex()
		{
			auto fileSystemState = FileSystemState(
				20,
				{
					{ 11, Path("C:/File1") },
					{ 12, Path("C:/File2") },
					{ 13, Path("C:/File3") },
					{ 14, Path("C:/File4") },
				});
			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x04, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '1',
				0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '2',
				0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '3',
				0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '4',
				'R', 'T', 'S', '\0', 0x01, 0x00, 0x00, 0x00,
				0x05, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00,
				0x10, 0x16, 0x62, 0xbb, 0x0b, 0x41, 0x38, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationResultsReader::Deserialize(content, fileSystemState);

			Assert::AreEqual(
				std::map<OperationId, OperationResult>({
					{
						5,
						OperationResult(
							true,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::sys_days(March/5/2020) + 12h + 35min + 34s + 1ms),
							{ 11, 13, },
							{ 12, 14, }),
					}
				}),
				actual.GetResults(),
				"Verify results match expected.");
		}

		// [[Fact]]
		void Deserialize_Multiple()
		{
			auto fileSystemState = FileSystemState(
				20,
				{
					{ 11, Path("C:/File1") },
					{ 12, Path("C:/File2") },
					{ 13, Path("C:/File3") },
					{ 14, Path("C:/File4") },
					{ 15, Path("C:/File5") },
					{ 16, Path("C:/File6") },
					{ 17, Path("C:/File7") },
					{ 18, Path("C:/File8") },
				});
			auto binaryFileContent = std::vector<uint8_t>(
			{
				'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
				'F', 'I', 'S', '\0', 0x08, 0x00, 0x00, 0x00,
				0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '1',
				0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '2',
				0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '3',
				0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '4',
				0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '5',
				0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '6',
				0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '7',
				0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '8',
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
			auto content = std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()));

			auto actual = OperationResultsReader::Deserialize(content, fileSystemState);

			auto expected = std::map<OperationId, OperationResult>(
			{
				{
					5,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::sys_days(March / 5 / 2020) + 12h + 35min + 34s + 1ms),
						{ 11, 13, },
						{ 12, 14, }),
				},
				{
					6,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::sys_days(March / 5 / 2020) + 12h + 36min + 55s),
						{ 15, 17, },
						{ 16, 18, }),
				},
			});

			Assert::AreEqual(
				expected,
				actual.GetResults(),
				"Verify results match expected.");
		}
	};
}
