// <copyright file="OperationResultsReader.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using Path = Opal.Path;

namespace Soup.Build.Utilities;

/// <summary>
/// The operation results state reader
/// </summary>
internal static class OperationResultsReader
{
	// Binary Operation Results file format
	private static uint FileVersion => 2;

	public static OperationResults Deserialize(System.IO.BinaryReader reader)
	{
		// Read the File Header with version
		var headerBuffer = reader.ReadBytes(4);
		if (headerBuffer[0] != 'B' ||
			headerBuffer[1] != 'O' ||
			headerBuffer[2] != 'R' ||
			headerBuffer[3] != '\0')
		{
			throw new InvalidOperationException("Invalid operation results file header");
		}

		var fileVersion = reader.ReadUInt32();
		if (fileVersion != FileVersion)
		{
			throw new InvalidOperationException("Operation results file version does not match expected");
		}

		// Read the set of files
		headerBuffer = reader.ReadBytes(4);
		if (headerBuffer[0] != 'F' ||
			headerBuffer[1] != 'I' ||
			headerBuffer[2] != 'S' ||
			headerBuffer[3] != '\0')
		{
			throw new InvalidOperationException("Invalid operation results files header");
		}

		var fileCount = reader.ReadUInt32();
		var files = new List<(FileId FileId, Path Path)>();
		for (var i = 0; i < fileCount; i++)
		{
			// Read the command working directory
			var fileId = new FileId(reader.ReadUInt32());
			var file = new Path(ReadString(reader));

			files.Add((fileId, file));
		}

		// Read the set of operation results
		headerBuffer = reader.ReadBytes(4);
		if (headerBuffer[0] != 'R' ||
			headerBuffer[1] != 'T' ||
			headerBuffer[2] != 'S' ||
			headerBuffer[3] != '\0')
		{
			throw new InvalidOperationException("Invalid operation results operation results header");
		}

		var operationResultsCount = reader.ReadUInt32();
		var operationResults = new Dictionary<OperationId, OperationResult>();
		for (var i = 0; i < operationResultsCount; i++)
		{
			var (operationId, operationResult) = ReadOperationInfo(reader);
			operationResults.Add(operationId, operationResult);
		}

		if (reader.BaseStream.Position != reader.BaseStream.Length)
		{
			var remaining = reader.BaseStream.Length - reader.BaseStream.Position;
			throw new InvalidOperationException($"Operation results file corrupted - Did not read the entire file {remaining}");
		}

		return new OperationResults(
			files,
			operationResults);
	}

	private static (OperationId, OperationResult) ReadOperationInfo(System.IO.BinaryReader reader)
	{
		// Read the operation id
		var id = new OperationId(reader.ReadUInt32());

		// Read the value indicating if there was a successful run
		var wasSuccessfulRun = ReadBoolean(reader);

		// Read the utc tick since January 1, 0001 at 00:00:00.000 in the Gregorian calendar
		var evaluateTime = new DateTime(reader.ReadInt64(), DateTimeKind.Utc);

		// Read the observed input files
		var observedInput = ReadFileIdList(reader);

		// Read the observed output files
		var observedOutput = ReadFileIdList(reader);

		return (id, new OperationResult(
			wasSuccessfulRun,
			evaluateTime,
			observedInput,
			observedOutput));
	}

	private static bool ReadBoolean(System.IO.BinaryReader reader)
	{
		uint result = reader.ReadUInt32();
		return result != 0;
	}

	private static string ReadString(System.IO.BinaryReader reader)
	{
		var size = reader.ReadUInt32();
		var result = reader.ReadChars((int)size);

		return new string(result);
	}

	private static List<FileId> ReadFileIdList(System.IO.BinaryReader reader)
	{
		var size = reader.ReadUInt32();
		var result = new List<FileId>((int)size);
		for (var i = 0; i < size; i++)
		{
			result.Add(new FileId(reader.ReadUInt32()));
		}

		return result;
	}
}