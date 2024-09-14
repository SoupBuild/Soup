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

			Assert::AreEqual("Read Unknown ValueType", exception.what(), "Verify Exception message");
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
				{
					{ "TestValue", Value(ValueTable()) },
				}),
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
				{
					{ "TestValue", Value(ValueList()) },
				}),
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
				{
					{ "TestValue", Value(std::string("Value")) },
				}),
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
				{
					{ "TestValue", Value(static_cast<int64_t>(-123)) },
				}),
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
				{
					{ "TestValue", Value(1.23) },
				}),
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
				{
					{ "TestValue", Value(true) },
				}),
				actual,
				"Verify value table matches expected.");
		}

		// [[Fact]]
		void Deserialize_Complex()
		{
			auto binaryFileContent = std::vector<unsigned char>(
			{
				'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
				'T', 'B', 'L', '\0', 0x08, 0x00, 0x00, 0x00,
				0x0b, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'B', 'o', 'o', 'l', 'e', 'a', 'n', 0x6, 0x0, 0x0, 0x0, 0x1, 0x00, 0x00, 0x00,
				0x0d, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'D', 'e', 'e', 'p', 'T', 'a', 'b', 'l', 'e', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '1', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '2', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '3', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '4', 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				0x0d, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'E', 'm', 'p', 't', 'y', 'L', 'i', 's', 't', 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				0x0e, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'E', 'm', 'p', 't', 'y', 'T', 'a', 'b', 'l', 'e', 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'F', 'l', 'o', 'a', 't', 0x5, 0x0, 0x0, 0x0, 0xae, 0x47, 0xe1, 0x7a, 0x14, 0xae, 0xf3, 0x3f,
				0x0b, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'I', 'n', 't', 'e', 'g', 'e', 'r', 0x4, 0x0, 0x0, 0x0, 0x85, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0x0f, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'I', 'n', 't', 'e', 'g', 'e', 'r', 'L', 'i', 's', 't', 0x2, 0x0, 0x0, 0x0, 0xa, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
				0x0a, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'S', 't', 'r', 'i', 'n', 'g', 0x3, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e',
			});
			auto content = std::stringstream(
				std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()));

			auto actual = ValueTableReader::Deserialize(content);

			Assert::AreEqual(
				ValueTable(
				{
					{ "TestEmptyTable", Value(ValueTable()) },
					{ "TestEmptyList", Value(ValueList()) },
					{ "TestString", Value(std::string("Value")) },
					{ "TestInteger", Value(static_cast<int64_t>(-123)) },
					{ "TestFloat", Value(1.23) },
					{ "TestBoolean", Value(true) },
					{
						"TestIntegerList",
						Value(ValueList({
							Value(static_cast<int64_t>(1)),
							Value(static_cast<int64_t>(2)),
							Value(static_cast<int64_t>(3)),
							Value(static_cast<int64_t>(4)),
							Value(static_cast<int64_t>(5)),
							Value(static_cast<int64_t>(6)),
							Value(static_cast<int64_t>(7)),
							Value(static_cast<int64_t>(8)),
							Value(static_cast<int64_t>(9)),
							Value(static_cast<int64_t>(10)),
						}))
					},
					{
						"TestDeepTable",
						Value(ValueTable({
							{
								"Value1",
								Value(ValueTable({
									{
										"Value2",
										Value(ValueTable({
											{
												"Value3",
												Value(ValueTable({
													{ "Value4", Value(ValueTable()) },
												}))
											},
										}))
									},
								}))
							},
						}))
					},
				}),
				actual,
				"Verify value table matches expected.");
		}
	};
}
