// <copyright file="OperationHistoryReader.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationHistory.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The operation history state reader
	/// </summary>
	export class OperationHistoryReader
	{
	private:
		// Binary Operation History file format
		static constexpr uint32_t FileVersion = 1;

	public:
		static OperationHistory Deserialize(std::istream& stream)
		{
			// Read the File Header with version
			auto headerBuffer = std::array<char, 4>();
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'B' ||
				headerBuffer[1] != 'O' ||
				headerBuffer[2] != 'H' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation history file header");
			}

			auto fileVersion = ReadUInt32(stream);
			if (fileVersion != FileVersion)
			{
				throw std::runtime_error("Operation History file version does not match expected");
			}

			// Read the set of operations
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'O' ||
				headerBuffer[1] != 'P' ||
				headerBuffer[2] != 'E' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation history operations header");
			}

			auto operationCount = ReadUInt32(stream);
			auto operations = std::vector<OperationInfo>(operationCount);
			for (auto i = 0; i < operationCount; i++)
			{
				// Read the command working directory
				auto workingDirectroy = Path(ReadString(stream));

				// Read the command executable
				auto executable = Path(ReadString(stream));

				// Read the command arguments
				auto arguments = ReadString(stream);

				// Write out the input files
				auto inputFiles = ReadFileIdList(stream);

				// Write out the output files
				auto outputFiles = ReadFileIdList(stream);

				operations[i] = OperationInfo(
					CommandInfo(
						std::move(workingDirectroy),
						std::move(executable),
						std::move(arguments)),
					std::move(inputFiles),
					std::move(outputFiles));
			}

			if (stream.peek() != std::char_traits<char>::eof())
			{
				throw std::runtime_error("Operation History file corrupted - Did not read the entire file");
			}

			return OperationHistory(std::move(operations));
		}

	private:
		static uint32_t ReadUInt32(std::istream& stream)
		{
			uint32_t result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
			if (stream.fail())
			{
				throw std::runtime_error("Failed to read unsigned integer value");
			}

			return result;
		}

		static std::string ReadString(std::istream& stream)
		{
			auto size = ReadUInt32(stream);
			auto result = std::string(size, '\0');
			stream.read(result.data(), size);
			if (stream.fail())
			{
				throw std::runtime_error("Failed to read string value");
			}

			return result;
		}

		static std::vector<FileId> ReadFileIdList(std::istream& stream)
		{
			auto size = ReadUInt32(stream);
			auto result = std::vector<FileId>(size);
			for (auto i = 0; i < size; i++)
			{
				result[i] = ReadUInt32(stream);
			}

			return result;
		}
	};
}
