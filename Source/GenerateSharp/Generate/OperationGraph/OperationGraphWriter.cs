// <copyright file="OperationGraphWriter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System.Collections.Generic;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The operation graph state writer
	/// </summary>
	internal static class OperationGraphWriter
	{
		// Binary Operation graph file format
		private static uint FileVersion => 3;

		public static void Serialize(OperationGraph state, System.IO.BinaryWriter writer)
		{
			// Write the File Header with version
			writer.Write(new char[] { 'B', 'O', 'G', '\0' });
			writer.Write(FileVersion);

			// Write out the set of files
			var files = state.GetReferencedFiles();
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
			WriteValues(writer, state.GetRootOperationIds());

			// Write out the set of operations
			var operations = state.GetOperations();
			writer.Write(new char[] { 'O', 'P', 'S', '\0' });
			writer.Write((uint)operations.Count);
			foreach (var operationValue in state.GetOperations())
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
			WriteValue(writer, operation.Command.Arguments);

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

			// Write out the value indicating if there was a successful run
			WriteValue(writer, operation.WasSuccessfulRun);

			// Write out the observed input files
			WriteValues(writer, operation.ObservedInput);

			// Write out the observed output files
			WriteValues(writer, operation.ObservedOutput);
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
