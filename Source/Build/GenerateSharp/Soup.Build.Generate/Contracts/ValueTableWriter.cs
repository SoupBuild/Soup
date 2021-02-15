// <copyright file="ValueTableWriter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Generate
{
	/// <summary>
	/// The value table state writer
	/// </summary>
	internal class ValueTableWriter
	{
	private:
		// Binary Value Table file format
		static constexpr uint32_t FileVersion = 1;

	public:
		static void Serialize(const ValueTable& state, std::ostream& stream)
		{
			// Write the File Header with version
			stream.write("BVT\0", 4);
			WriteValue(stream, FileVersion);

			// Write out the root table
			stream.write("TBL\0", 4);
			WriteValue(stream, state);
		}

	private:
		static void WriteValue(std::ostream& stream, const Value& value)
		{
			// Write the value type
			auto valueType = value.GetType();
			WriteValue(stream, static_cast<uint32_t>(valueType));

			switch (valueType)
			{
				case ValueType::Empty:
					// Nothing to write
					break;
				case ValueType::Table:
					WriteValue(stream, value.AsTable());
					break;
				case ValueType::List:
					WriteValue(stream, value.AsList());
					break;
				case ValueType::String:
					WriteValue(stream, string_view(value.AsString().ToString()));
					break;
				case ValueType::Integer:
					WriteValue(stream, value.AsInteger().GetValue());
					break;
				case ValueType::Float:
					WriteValue(stream, value.AsFloat().GetValue());
					break;
				case ValueType::Boolean:
					WriteValue(stream, value.AsBoolean().GetValue());
					break;
				default:
					throw std::runtime_error("Unknown ValueType");
			}
		}

		static void WriteValue(std::ostream& stream, const ValueTable& value)
		{
			// Write the count of values
			auto& tableValues = value.GetValues();
			WriteValue(stream, static_cast<uint32_t>(tableValues.size()));

			for (auto& tableValue : tableValues)
			{
				// Write the key
				WriteValue(stream, string_view(tableValue.first));

				// Write the value
				WriteValue(stream, tableValue.second);
			}
		}

		static void WriteValue(std::ostream& stream, const ValueList& value)
		{
			// Write the count of values
			auto& listValues = value.GetValues();
			WriteValue(stream, static_cast<uint32_t>(listValues.size()));

			for (auto& listValue : listValues)
			{
				WriteValue(stream, listValue);
			}
		}

		static void WriteValue(std::ostream& stream, uint32_t value)
		{
			stream.write(reinterpret_cast<char*>(&value), sizeof(uint32_t));
		}

		static void WriteValue(std::ostream& stream, int64_t value)
		{
			stream.write(reinterpret_cast<char*>(&value), sizeof(int64_t));
		}

		static void WriteValue(std::ostream& stream, double value)
		{
			stream.write(reinterpret_cast<char*>(&value), sizeof(double));
		}

		static void WriteValue(std::ostream& stream, bool value)
		{
			uint32_t integerValue = value ? 1u : 0u;
			stream.write(reinterpret_cast<char*>(&integerValue), sizeof(uint32_t));
		}

		static void WriteValue(std::ostream& stream, string_view value)
		{
			WriteValue(stream, static_cast<uint32_t>(value.size()));
			stream.write(value.data(), value.size());
		}
	};
}
