// <copyright file="OperationResultsWriter.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"

namespace Soup::Core
{
	/// <summary>
	/// The operation results state writer
	/// </summary>
	export class OperationResultsWriter
	{
	private:
		// Binary Operation results file format
		static constexpr uint32_t FileVersion = 1;

		// The offset from January 1, 1970 at 00:00:00.000 to January 1, 0001 at 00:00:00.000 in the Gregorian calendar
		static constexpr long long UnixEpochOffset = 62135596800000;

	public:
		static void Serialize(
			const OperationResults& state,
			const std::set<FileId>& files,
			const FileSystemState& fileSystemState,
			std::ostream& stream)
		{
			// Write the File Header with version
			stream.write("BOR\0", 4);
			WriteValue(stream, FileVersion);

			// Write out the set of files
			stream.write("FIS\0", 4);
			WriteValue(stream, static_cast<uint32_t>(files.size()));
			for (auto fileId : files)
			{
				// Write the file id + path length + path
				WriteValue(stream, fileId);
				WriteValue(stream, fileSystemState.GetFilePath(fileId).ToString());
			}

			// Write out the set of results
			auto& results = state.GetResults();
			stream.write("RTS\0", 4);
			WriteValue(stream, static_cast<uint32_t>(results.size()));
			for (auto& resultValue : results)
			{
				WriteOperationResult(stream, resultValue.first, resultValue.second);
			}
		}

	private:
		static void WriteOperationResult(std::ostream& stream, OperationId operationId, const OperationResult& result)
		{
			// Write out the operation id
			WriteValue(stream, operationId);

			// Write out the value indicating if there was a successful run
			WriteValue(stream, result.WasSuccessfulRun);

			// Write out the utc milliseconds since January 1, 0001 at 00:00:00.000 in the Gregorian calendar
			auto unixEvaluateTimeMilliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(result.EvaluateTime).time_since_epoch().count();
			auto evaluateTimeMilliseconds = unixEvaluateTimeMilliseconds + UnixEpochOffset;
			WriteValue(stream, evaluateTimeMilliseconds);

			// Write out the observed input files
			WriteValues(stream, result.ObservedInput);

			// Write out the observed output files
			WriteValues(stream, result.ObservedOutput);
		}

		static void WriteValue(std::ostream& stream, uint32_t value)
		{
			stream.write(reinterpret_cast<char*>(&value), sizeof(uint32_t));
		}

		static void WriteValue(std::ostream& stream, int64_t value)
		{
			stream.write(reinterpret_cast<char*>(&value), sizeof(int64_t));
		}

		static void WriteValue(std::ostream& stream, bool value)
		{
			uint32_t integerValue = value ? 1u : 0u;
			stream.write(reinterpret_cast<char*>(&integerValue), sizeof(uint32_t));
		}

		static void WriteValue(std::ostream& stream, std::string_view value)
		{
			WriteValue(stream, static_cast<uint32_t>(value.size()));
			stream.write(value.data(), value.size());
		}

		static void WriteValues(std::ostream& stream, const std::vector<uint32_t>& values)
		{
			WriteValue(stream, static_cast<uint32_t>(values.size()));
			for (auto& value : values)
			{
				WriteValue(stream, value);
			}
		}
	};
}
