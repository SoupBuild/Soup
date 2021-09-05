// <copyright file="ValueTableReader.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The value table state reader
	/// </summary>
	internal class ValueTableReader
	{
		// Binary Value Table file format
		private static uint FileVersion => 2;

		public static ValueTable Deserialize(System.IO.BinaryReader reader)
		{
			// Read the File Header with version
			var headerBuffer = reader.ReadChars(4);
			if (headerBuffer[0] != 'B' ||
				headerBuffer[1] != 'V' ||
				headerBuffer[2] != 'T' ||
				headerBuffer[3] != '\0')
			{
				throw new InvalidOperationException("Invalid Value Table file header");
			}

			var fileVersion = reader.ReadUInt32();
			if (fileVersion != FileVersion)
			{
				throw new InvalidOperationException("Value Table file version does not match expected");
			}

			// Read the root table
			headerBuffer = reader.ReadChars(4);
			if (headerBuffer[0] != 'T' ||
				headerBuffer[1] != 'B' ||
				headerBuffer[2] != 'L' ||
				headerBuffer[3] != '\0')
			{
				throw new InvalidOperationException("Invalid Value Table table header");
			}

			var rootTable = ReadValueTable(reader);

			if (reader.BaseStream.Position != reader.BaseStream.Length)
			{
				throw new InvalidOperationException("Value Table file corrupted - Did not read the entire file");
			}

			return rootTable;
		}

		private static Value ReadValue(System.IO.BinaryReader reader)
		{
			// Read the value type
			var valueType = (ValueType)reader.ReadUInt32();

			switch (valueType)
			{
				case ValueType.Table:
					return new Value(ReadValueTable(reader));
				case ValueType.List:
					return new Value(ReadValueList(reader));
				case ValueType.String:
					return new Value(ReadString(reader));
				case ValueType.Integer:
					return new Value(reader.ReadInt64());
				case ValueType.Float:
					return new Value(reader.ReadDouble());
				case ValueType.Boolean:
					return new Value(ReadBoolean(reader));
				default:
					throw new InvalidOperationException("Unknown ValueType");
			}
		}

		private static ValueTable ReadValueTable(System.IO.BinaryReader reader)
		{
			// Write out the table size
			var size = reader.ReadUInt32();

			var table = new ValueTable();
			for (var i = 0; i < size; i++)
			{
				// Read the key
				var key = ReadString(reader);

				// Read the value
				var value = ReadValue(reader);

				table.Add(key, value);
			}

			return table;
		}

		private static ValueList ReadValueList(System.IO.BinaryReader reader)
		{
			// Write out the list size
			var size = reader.ReadUInt32();

			var list = new ValueList();
			for (var i = 0; i < size; i++)
			{
				// Read the value
				var value = ReadValue(reader);

				list.Add(value);
			}

			return list;
		}

		static bool ReadBoolean(System.IO.BinaryReader reader)
		{
			var result = reader.ReadUInt32();
			return result != 0;
		}

		private static string ReadString(System.IO.BinaryReader reader)
		{
			var size = reader.ReadUInt32();
			var result = reader.ReadChars((int)size);

			return new string(result);
		}
	}
}
