// <copyright file="ValueTableReaderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class ValueTableReaderTests
	{
	public:
		// [[Fact]]
		void Deserialize_InvalidFileHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content]() {
				auto actual = ValueTableReader::Deserialize(content);
			});

			Assert::AreEqual("Invalid Value Table file header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidFileVersionThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x00, 0x00, 0x00, 0x02,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content]() {
				auto actual = ValueTableReader::Deserialize(content);
			});
			
			Assert::AreEqual("Value Table file version does not match expected", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidTableHeaderThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '2',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content]() {
				auto actual = ValueTableReader::Deserialize(content);
			});
			
			Assert::AreEqual("Invalid Value Table table header", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_InvalidValueTypeThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content]() {
				auto actual = ValueTableReader::Deserialize(content);
			});

			Assert::AreEqual("Unknown ValueType", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_ExtraContentThrows()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto exception = Assert::Throws<std::runtime_error>([&content]() {
				auto actual = ValueTableReader::Deserialize(content);
			});
			
			Assert::AreEqual("Value Table file corrupted - Did not read the entire file", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void Deserialize_Empty()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(),
				actual,
				"Verify value table matches expected.");
		}

		// [[Fact]]
		void Deserialize_SingleTable()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(
					std::map<std::string, Value>({
						{
							"TestValue",
							Value(ValueTable()),
						},
					})),
				actual,
				"Verify value table matches expected.");
		}

		// [[Fact]]
		void Deserialize_SingleList()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(
					std::map<std::string, Value>({
						{
							"TestValue",
							Value(ValueList()),
						},
					})),
				actual,
				"Verify value table matches expected.");
		}

		// [[Fact]]
		void Deserialize_SingleString()
		{
			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 'V', 'a', 'l', 'u', 'e',
			});
			auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(
					std::map<std::string, Value>({
						{
							"TestValue",
							Value(std::string("Value")),
						},
					})),
				actual,
				"Verify value table matches expected.");
		}

		// [[Fact]]
		void Deserialize_SingleInteger()
		{
			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x04, 0x00, 0x00, 0x00, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			});
			auto content = std::stringstream(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(
					std::map<std::string, Value>({
						{
							"TestValue",
							Value(static_cast<int64_t>(-123)),
						},
					})),
				actual,
				"Verify value table matches expected.");
		}

		// [[Fact]]
		void Deserialize_SingleFloat()
		{
			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x05, 0x00, 0x00, 0x00, 0xAE, 0x47, 0xE1, 0x7A, 0x14, 0xAE, 0xF3, 0x3F,
			});
			auto content = std::stringstream(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(
					std::map<std::string, Value>({
						{
							"TestValue",
							Value(1.23),
						},
					})),
				actual,
				"Verify value table matches expected.");
		}

		// [[Fact]]
		void Deserialize_SingleBoolean()
		{
			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
			});
			auto content = std::stringstream(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(
					std::map<std::string, Value>({
						{
							"TestValue",
							Value(true),
						},
					})),
				actual,
				"Verify value table matches expected.");
		}
	};
}
