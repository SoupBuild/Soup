// <copyright file="ValueTableReader.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Runtime
{
	/// <summary>
	/// The value table state reader
	/// </summary>
	internal class ValueTableReader
	{
	private:
		// Binary Value Table file format
		static constexpr uint32_t FileVersion = 1;

	public:
		static ValueTable Deserialize(std::istream& stream)
		{
			// Read the File Header with version
			auto headerBuffer = std::array<char, 4>();
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'B' ||
				headerBuffer[1] != 'V' ||
				headerBuffer[2] != 'T' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid Value Table file header");
			}

			auto fileVersion = ReadUInt32(stream);
			if (fileVersion != FileVersion)
			{
				throw std::runtime_error("Value Table file version does not match expected");
			}

			// Read the root table
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'T' ||
				headerBuffer[1] != 'B' ||
				headerBuffer[2] != 'L' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid Value Table table header");
			}

			auto rootTable = ReadValueTable(stream);

			if (stream.peek() != std::char_traits<char>::eof())
			{
				throw std::runtime_error("Value Table file corrupted - Did not read the entire file");
			}

			return rootTable;
		}

	private:
		static Value ReadValue(std::istream& stream)
		{
			// Read the value type
			auto valueType = static_cast<ValueType>(ReadUInt32(stream));

			switch (valueType)
			{
				case ValueType::Empty:
					return Value();
				case ValueType::Table:
					return Value(ReadValueTable(stream));
				case ValueType::List:
					return Value(ReadValueList(stream));
				case ValueType::String:
					return Value(ReadString(stream));
				case ValueType::Integer:
					return Value(ReadInt64(stream));
				case ValueType::Float:
					return Value(ReadDouble(stream));
				case ValueType::Boolean:
					return Value(ReadBoolean(stream));
				default:
					throw std::runtime_error("Unknown ValueType");
			}
		}

		static ValueTable ReadValueTable(std::istream& stream)
		{
			// Write out the table size
			auto size = ReadUInt32(stream);

			auto table = std::map<string, Value>();
			for (auto i = 0u; i < size; i++)
			{
				// Read the key
				auto key = ReadString(stream);

				// Read the value
				auto value = ReadValue(stream);

				table.emplace(std::move(key), std::move(value));
			}

			return ValueTable(
				std::move(table));
		}

		static ValueList ReadValueList(std::istream& stream)
		{
			// Write out the list size
			auto size = ReadUInt32(stream);

			auto list = IList<Value>();
			for (auto i = 0u; i < size; i++)
			{
				// Read the value
				auto value = ReadValue(stream);

				list.push_back(std::move(value));
			}

			return ValueList(
				std::move(list));
		}

		static int64_t ReadInt64(std::istream& stream)
		{
			int64_t result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(int64_t));
			if (stream.fail())
			{
				throw std::runtime_error("ValueTableReader Failed to read integer value");
			}

			return result;
		}

		static uint32_t ReadUInt32(std::istream& stream)
		{
			uint32_t result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
			if (stream.fail())
			{
				throw std::runtime_error("ValueTableReader Failed to read unsigned integer value");
			}

			return result;
		}

		static double ReadDouble(std::istream& stream)
		{
			double result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(double));
			if (stream.fail())
			{
				throw std::runtime_error("ValueTableReader Failed to read double value");
			}

			return result;
		}

		static bool ReadBoolean(std::istream& stream)
		{
			uint32_t result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
			if (stream.fail())
			{
				throw std::runtime_error("ValueTableReader Failed to read boolean value");
			}

			return result != 0;
		}

		static string ReadString(std::istream& stream)
		{
			auto size = ReadUInt32(stream);
			auto result = string(size, '\0');
			stream.read(result.data(), size);
			if (stream.fail())
			{
				throw std::runtime_error("ValueTableReader Failed to read string value");
			}

			return result;
		}
	};
}
