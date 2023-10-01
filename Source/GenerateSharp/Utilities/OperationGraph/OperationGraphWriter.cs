// <copyright file="OperationGraphWriter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The operation graph state writer
	/// </summary>
	internal static class OperationGraphWriter
	{
		// Binary Operation graph file format
		private static uint FileVersion => 6;

		public static void Serialize(OperationGraph state, System.IO.BinaryWriter writer)
		{
			// Write the File Header with version
			writer.Write(new char[] { 'B', 'O', 'G', '\0' });
			writer.Write(FileVersion);

			// Write out the set of files
			var files = state.ReferencedFiles;
			writer.Write(new char[] { 'F', 'I', 'S', '\0' });
			writer.Write((uint)files.Count);
			foreach (var file in files)
			{
				// Write the file id + path length + path
				writer.Write(file.FileId.value);
				WriteValue(writer, file.Path.ToString());
			}

			// Write out the root operation ids
			writer.Write(new char[] { 'R', 'O', 'P', '\0' });
			WriteValues(writer, state.RootOperationIds);

			// Write out the set of operations
			writer.Write(new char[] { 'O', 'P', 'S', '\0' });
			writer.Write((uint)state.Operations.Count);
			foreach (var operationValue in state.Operations)
			{
				WriteOperationInfo(writer, operationValue.Value);
			}
		}

		private static void WriteOperationInfo(System.IO.BinaryWriter writer, OperationInfo operation)
		{
			// Write out the operation id
			writer.Write(operation.Id.value);

			// Write out the operation title
			WriteValue(writer, operation.Title);

			// Write the command working directory
			WriteValue(writer, operation.Command.WorkingDirectory.ToString());

			// Write the command executable
			WriteValue(writer, operation.Command.Executable.ToString());

			// Write the command arguments
			WriteValues(writer, operation.Command.Arguments);

			// Write out the declared input files
			WriteValues(writer, operation.DeclaredInput);

			// Write out the declared output files
			WriteValues(writer, operation.DeclaredOutput);

			// Write out the read access list
			WriteValues(writer, operation.ReadAccess);

			// Write out the write access list
			WriteValues(writer, operation.WriteAccess);

			// Write out the child operation ids
			WriteValues(writer, operation.Children);

			// Write out the dependency count
			writer.Write(operation.DependencyCount);
		}

		private static void WriteValue(System.IO.BinaryWriter writer, bool value)
		{
			uint integerValue = value ? 1u : 0u;
			writer.Write(integerValue);
		}

		private static void WriteValue(System.IO.BinaryWriter writer, string value)
		{
			writer.Write((uint)value.Length);
			writer.Write(value.ToCharArray());
		}

		private static void WriteValues(System.IO.BinaryWriter writer, IReadOnlyList<string> values)
		{
			writer.Write((uint)values.Count);
			foreach (var value in values)
			{
				writer.Write(value);
			}
		}

		private static void WriteValues(System.IO.BinaryWriter writer, IList<FileId> values)
		{
			writer.Write((uint)values.Count);
			foreach (var value in values)
			{
				writer.Write(value.value);
			}
		}

		private static void WriteValues(System.IO.BinaryWriter writer, IList<OperationId> values)
		{
			writer.Write((uint)values.Count);
			foreach (var value in values)
			{
				writer.Write(value.value);
			}
		}
	}
}
