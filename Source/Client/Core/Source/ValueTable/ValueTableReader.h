// <copyright file="ValueTableReader.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Value.h"

namespace Soup::Core
{
	/// <summary>
	/// The value table state reader
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class ValueTableReader
	{
	private:
		// Binary Value Table file format
		static constexpr uint32_t FileVersion = 2;

	public:
		static ValueTable Deserialize(std::istream& stream)
		{
			// Read the entire file for fastest read operation
			stream.seekg(0, std::ios_base::end);
			auto size = stream.tellg();
			stream.seekg(0, std::ios_base::beg);

			auto contentBuffer = std::vector<char>(size);
			stream.read(contentBuffer.data(), size);
			auto content = contentBuffer.data();

			// Read the File Header with version
			auto headerBuffer = std::array<char, 4>();
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'B' ||
				headerBuffer[1] != 'V' ||
				headerBuffer[2] != 'T' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid Value Table file header");
			}

			auto fileVersion = ReadUInt32(content);
			if (fileVersion != FileVersion)
			{
				throw std::runtime_error("Value Table file version does not match expected");
			}

			// Read the root table
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'T' ||
				headerBuffer[1] != 'B' ||
				headerBuffer[2] != 'L' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid Value Table table header");
			}

			auto rootTable = ReadValueTable(content);

			if (content != (contentBuffer.data() + size))
			{
				throw std::runtime_error("Value Table file corrupted - Did not read the entire file");
			}

			return rootTable;
		}

	private:
		static Value ReadValue(char*& content)
		{
			// Read the value type
			auto valueType = static_cast<ValueType>(ReadUInt32(content));

			switch (valueType)
			{
				case ValueType::Table:
					return Value(ReadValueTable(content));
				case ValueType::List:
					return Value(ReadValueList(content));
				case ValueType::String:
					return Value(ReadString(content));
				case ValueType::Integer:
					return Value(ReadInt64(content));
				case ValueType::Float:
					return Value(ReadDouble(content));
				case ValueType::Boolean:
					return Value(ReadBoolean(content));
				default:
					throw std::runtime_error("Unknown ValueType");
			}
		}

		static ValueTable ReadValueTable(char*& content)
		{
			// Write out the table size
			auto size = ReadUInt32(content);

			auto table = ValueTable();
			for (auto i = 0u; i < size; i++)
			{
				// Read the key
				auto key = ReadString(content);

				// Read the value
				auto value = ReadValue(content);

				table.emplace(std::move(key), std::move(value));
			}

			return table;
		}

		static ValueList ReadValueList(char*& content)
		{
			// Write out the list size
			auto size = ReadUInt32(content);

			auto list = ValueList();
			for (auto i = 0u; i < size; i++)
			{
				// Read the value
				auto value = ReadValue(content);

				list.push_back(std::move(value));
			}

			return list;
		}

		static int64_t ReadInt64(char*& content)
		{
			int64_t result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(int64_t));

			return result;
		}

		static uint32_t ReadUInt32(char*& content)
		{
			uint32_t result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(uint32_t));

			return result;
		}

		static double ReadDouble(char*& content)
		{
			double result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(double));

			return result;
		}

		static bool ReadBoolean(char*& content)
		{
			uint32_t result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(uint32_t));

			return result != 0;
		}

		static std::string ReadString(char*& content)
		{
			auto size = ReadUInt32(content);
			auto result = std::string(size, '\0');
			Read(content, result.data(), size);

			return result;
		}

		static void Read(char*& data, char* buffer, size_t count)
		{
			memcpy(buffer, data, count);
			data += count;
		}
	};
}
