// <copyright file="ValueTableWriterTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class ValueTableWriterTests
	{
	public:
		// [[Fact]]
		void Serialize_Empty()
		{
			auto valueTable = ValueTable();
			auto content = std::stringstream();

			ValueTableWriter::Serialize(valueTable, content);

			auto binaryFileContent = std::vector<char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x00, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(binaryFileContent.data(), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleTable()
		{
			auto valueTable = ValueTable(
				std::map<std::string, Value>({
					{
						"TestValue",
						Value(ValueTable()),
					},
				}));
			auto content = std::stringstream();

			ValueTableWriter::Serialize(valueTable, content);

			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleList()
		{
			auto valueTable = ValueTable(
				std::map<std::string, Value>({
					{
						"TestValue",
						Value(ValueList()),
					},
				}));
			auto content = std::stringstream();

			ValueTableWriter::Serialize(valueTable, content);

			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			});
			Assert::AreEqual(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleString()
		{
			auto valueTable = ValueTable(
				std::map<std::string, Value>({
					{
						"TestValue",
						Value(std::string("Value")),
					},
				}));
			auto content = std::stringstream();

			ValueTableWriter::Serialize(valueTable, content);

			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x03, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 'V', 'a', 'l', 'u', 'e',
			});
			Assert::AreEqual(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleInteger()
		{
			auto valueTable = ValueTable(
				std::map<std::string, Value>({
					{
						"TestValue",
						Value(static_cast<int64_t>(-123)),
					},
				}));
			auto content = std::stringstream();

			ValueTableWriter::Serialize(valueTable, content);

			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x04, 0x00, 0x00, 0x00, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			});
			Assert::AreEqual(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleFloat()
		{
			auto valueTable = ValueTable(
				std::map<std::string, Value>({
					{
						"TestValue",
						Value(1.23),
					},
				}));
			auto content = std::stringstream();

			ValueTableWriter::Serialize(valueTable, content);

			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x05, 0x00, 0x00, 0x00, 0xAE, 0x47, 0xE1, 0x7A, 0x14, 0xAE, 0xF3, 0x3F,
			});
			Assert::AreEqual(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Serialize_SingleBoolean()
		{
			auto valueTable = ValueTable(
				std::map<std::string, Value>({
					{
						"TestValue",
						Value(true),
					},
				}));
			auto content = std::stringstream();

			ValueTableWriter::Serialize(valueTable, content);

			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x01, 0x00, 0x00, 0x00,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'V', 'a', 'l', 'u', 'e',
				0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
			});

			Assert::AreEqual(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()),
				content.str(),
				"Verify file content match expected.");
		}
	};
}
