// <copyright file="OperationGraphReader.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationGraph.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The operation graph state reader
	/// </summary>
	export class OperationGraphReader
	{
	private:
		// Binary Operation Graph file format
		static constexpr uint32_t FileVersion = 1;

	public:
		static OperationGraph Deserialize(std::istream& stream)
		{
			// Read the File Header with version
			auto headerBuffer = std::array<char, 4>();
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'B' ||
				headerBuffer[1] != 'O' ||
				headerBuffer[2] != 'G' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation graph file header");
			}

			auto fileVersion = ReadUInt32(stream);
			if (fileVersion != FileVersion)
			{
				throw std::runtime_error("Operation graph file version does not match expected");
			}

			// Read in the file system state that was used for this operation graph
			auto stateId = ReadUInt32(stream);

			// Read the set of operations
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'R' ||
				headerBuffer[1] != 'O' ||
				headerBuffer[2] != 'P' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation graph root operations header");
			}

			// Read the root operation ids
			auto rootOperationIds = ReadOperationIdList(stream);

			// Read the set of operations
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'O' ||
				headerBuffer[1] != 'P' ||
				headerBuffer[2] != 'S' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation graph operations header");
			}

			auto operationCount = ReadUInt32(stream);
			auto operations = std::vector<OperationInfo>(operationCount);
			for (auto i = 0u; i < operationCount; i++)
			{
				operations[i] = ReadOperationInfo(stream);
			}

			if (stream.peek() != std::char_traits<char>::eof())
			{
				throw std::runtime_error("Operation graph file corrupted - Did not read the entire file");
			}

			return OperationGraph(
				stateId,
				std::move(rootOperationIds),
				std::move(operations));
		}

	private:
		static OperationInfo ReadOperationInfo(std::istream& stream)
		{
			// Write out the operation id
			auto id = ReadUInt32(stream);

			// Write the operation title
			auto title = ReadString(stream);

			// Write the command working directory
			auto workingDirectory = ReadString(stream);

			// Write the command executable
			auto executable = ReadString(stream);

			// Write the command arguments
			auto arguments = ReadString(stream);

			// Write out the declared input files
			auto declaredInput = ReadFileIdList(stream);

			// Write out the declared output files
			auto declaredOutput = ReadFileIdList(stream);

			// Write out the child operation ids
			auto children = ReadOperationIdList(stream);

			// Write out the dependency count
			auto dependecyCount = ReadInt32(stream);

			// Write out the value indicating if there was a successful run
			auto wasSuccessfulRun = ReadBoolean(stream);

			// Write out the observed input files
			auto observedInput = ReadFileIdList(stream);

			// Write out the observed output files
			auto observedOutput = ReadFileIdList(stream);

			return OperationInfo(
				id,
				std::move(title),
				CommandInfo(
					Path(workingDirectory),
					Path(executable),
					std::move(arguments)),
				std::move(declaredInput),
				std::move(declaredOutput),
				std::move(children),
				dependecyCount,
				wasSuccessfulRun,
				std::move(observedInput),
				std::move(observedOutput));
		}

		static int32_t ReadInt32(std::istream& stream)
		{
			int32_t result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(int32_t));
			if (stream.fail())
			{
				throw std::runtime_error("OperationGraphReader Failed to read integer value");
			}

			return result;
		}

		static uint32_t ReadUInt32(std::istream& stream)
		{
			uint32_t result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
			if (stream.fail())
			{
				throw std::runtime_error("OperationGraphReader Failed to read unsigned integer value");
			}

			return result;
		}

		static boolean ReadBoolean(std::istream& stream)
		{
			uint32_t result = 0;
			stream.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
			if (stream.fail())
			{
				throw std::runtime_error("OperationGraphReader Failed to read boolean value");
			}

			return result != 0;
		}

		static std::string ReadString(std::istream& stream)
		{
			auto size = ReadUInt32(stream);
			auto result = std::string(size, '\0');
			stream.read(result.data(), size);
			if (stream.fail())
			{
				throw std::runtime_error("OperationGraphReader Failed to read string value");
			}

			return result;
		}

		static std::vector<FileId> ReadFileIdList(std::istream& stream)
		{
			auto size = ReadUInt32(stream);
			auto result = std::vector<FileId>(size);
			for (auto i = 0u; i < size; i++)
			{
				result[i] = ReadUInt32(stream);
			}

			return result;
		}

		static std::vector<OperationId> ReadOperationIdList(std::istream& stream)
		{
			auto size = ReadUInt32(stream);
			auto result = std::vector<OperationId>(size);
			for (auto i = 0u; i < size; i++)
			{
				result[i] = ReadUInt32(stream);
			}

			return result;
		}
	};
}
