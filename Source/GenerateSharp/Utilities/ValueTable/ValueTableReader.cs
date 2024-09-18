// <copyright file="ValueTableReader.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

/// <summary>
/// The value table state reader
/// </summary>
public sealed class ValueTableReader
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
			var remaining = reader.BaseStream.Length - reader.BaseStream.Position;
			throw new InvalidOperationException($"Value Table file corrupted - Did not read the entire file {remaining}");
		}

		return rootTable;
	}

	private static Value ReadValue(System.IO.BinaryReader reader)
	{
		// Read the value type
		var valueType = (ValueType)reader.ReadUInt32();

		return valueType switch
		{
			ValueType.Table => new Value(ReadValueTable(reader)),
			ValueType.List => new Value(ReadValueList(reader)),
			ValueType.String => new Value(ReadString(reader)),
			ValueType.Integer => new Value(reader.ReadInt64()),
			ValueType.Float => new Value(reader.ReadDouble()),
			ValueType.Boolean => new Value(ReadBoolean(reader)),
			ValueType.Empty => throw new NotImplementedException(),
			_ => throw new InvalidOperationException("Unknown ValueType"),
		};
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

	private static bool ReadBoolean(System.IO.BinaryReader reader)
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