// <copyright file="OperationHistoryWriter.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationHistory.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The operation history state writer
	/// </summary>
	export class OperationHistoryWriter
	{
	private:
		// Binary Operation History file format
		static constexpr uint32_t FileVersion = 1;

	public:
		static void Serialize(const OperationHistory& state, std::ostream& stream)
		{
			// Write the File Header with version
			stream.write("BOH\0", 4);
			WriteValue(stream, FileVersion);

			// Write out the file system state that was used for this operation history
			WriteValue(stream, state.GetStateId());

			// Write out the set of operations
			auto& operations = state.GetOperations();
			stream.write("OPS\0", 4);
			WriteValue(stream, static_cast<uint32_t>(operations.size()));
			for (auto& operationValue : state.GetOperations())
			{
				auto& operation = operationValue.second;

				// Write the command working directory
				WriteValue(stream, operation.Command.WorkingDirectory.ToString());

				// Write the command executable
				WriteValue(stream, operation.Command.Executable.ToString());

				// Write the command arguments
				WriteValue(stream, operation.Command.Arguments);

				// Write out the input files
				WriteValues(stream, operation.Input);

				// Write out the output files
				WriteValues(stream, operation.Output);
			}
		}

		static void WriteValue(std::ostream& stream, uint32_t value)
		{
			stream.write(reinterpret_cast<char*>(&value), sizeof(uint32_t));
		}

		static void WriteValue(std::ostream& stream, std::string_view value)
		{
			WriteValue(stream, static_cast<uint32_t>(value.size()));
			stream.write(value.data(), value.size());
		}

		static void WriteValues(std::ostream& stream, const std::vector<FileId>& values)
		{
			WriteValue(stream, static_cast<uint32_t>(values.size()));
			for (auto& value : values)
			{
				WriteValue(stream, value);
			}
		}
	};
}
