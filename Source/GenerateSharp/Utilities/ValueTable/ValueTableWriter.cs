// <copyright file="ValueTableWriter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.IO;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The value table state writer
	/// </summary>
	internal class ValueTableWriter
	{
		// Binary Value Table file format
		private static uint FileVersion => 2;

		public static void Serialize(IValueTable state, BinaryWriter writer)
		{
			// Write the File Header with version
			writer.Write(new char[] { 'B', 'V', 'T', '\0' });
			writer.Write(FileVersion);

			// Write out the root table
			writer.Write(new char[] { 'T', 'B', 'L', '\0' });
			WriteValue(writer, state);
		}

		private static void WriteValue(BinaryWriter writer, IValue value)
		{
			// Write the value type
			writer.Write((uint)value.Type);

			switch (value.Type)
			{
				case ValueType.Table:
					WriteValue(writer, value.AsTable());
					break;
				case ValueType.List:
					WriteValue(writer, value.AsList());
					break;
				case ValueType.String:
					WriteValue(writer, value.AsString());
					break;
				case ValueType.Integer:
					writer.Write(value.AsInteger());
					break;
				case ValueType.Float:
					writer.Write(value.AsFloat());
					break;
				case ValueType.Boolean:
					WriteValue(writer, value.AsBoolean());
					break;
				default:
					throw new InvalidOperationException("Unknown ValueType");
			}
		}

		static void WriteValue(BinaryWriter writer, IValueTable value)
		{
			// Write the count of values
			writer.Write((uint)value.Count);

			foreach (var tableValue in value)
			{
				// Write the key
				WriteValue(writer, tableValue.Key);

				// Write the value
				WriteValue(writer, tableValue.Value);
			}
		}

		static void WriteValue(BinaryWriter writer, IValueList value)
		{
			// Write the count of values
			writer.Write((uint)value.Count);

			foreach (var listValue in value)
			{
				WriteValue(writer, listValue);
			}
		}

		public static void WriteValue(BinaryWriter writer, bool value)
		{
			uint integerValue = value ? 1u : 0u;
			writer.Write(integerValue);
		}

		public static void WriteValue(BinaryWriter writer, string value)
		{
			writer.Write((uint)value.Length);
			writer.Write(value.ToCharArray());
		}
	}
}
