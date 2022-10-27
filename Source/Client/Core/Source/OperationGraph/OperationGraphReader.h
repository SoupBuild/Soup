// <copyright file="OperationGraphReader.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationGraph.h"

namespace Soup::Core
{
	/// <summary>
	/// The operation graph state reader
	/// </summary>
	export class OperationGraphReader
	{
	private:
		// Binary Operation Graph file format
		static constexpr uint32_t FileVersion = 4;

		// The offset from January 1, 1970 at 00:00:00.000 to January 1, 0001 at 00:00:00.000 in the Gregorian calendar
		static constexpr long long UnixEpochOffset = 62135596800000;

	public:
		static OperationGraph Deserialize(std::istream& stream)
		{
			// Read the entire file for fastest read operation
			stream.seekg(0, std::ios_base::end);
			auto size = stream.tellg();
			stream.seekg(0, std::ios_base::beg);

			auto contentBuffer = std::vector<char>(size);
			stream.read(contentBuffer.data(), size);
			auto content = contentBuffer.data();

			// Read the File Header with version
			auto headerBuffer = std::array<char, 4>();
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'B' ||
				headerBuffer[1] != 'O' ||
				headerBuffer[2] != 'G' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation graph file header");
			}

			auto fileVersion = ReadUInt32(content);
			if (fileVersion != FileVersion)
			{
				throw std::runtime_error("Operation graph file version does not match expected");
			}

			// Read the set of files
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'F' ||
				headerBuffer[1] != 'I' ||
				headerBuffer[2] != 'S' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation graph files header");
			}

			auto fileCount = ReadUInt32(content);
			auto files = std::vector<std::pair<FileId, Path>>();
			for (auto i = 0u; i < fileCount; i++)
			{
				// Read the command working directory
				auto fileId = ReadUInt32(content);

				auto fileString = ReadString(content);
				auto file = Path::Load(std::move(fileString));

				files.push_back({ fileId, std::move(file) });
			}

			// Read the set of operations
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'R' ||
				headerBuffer[1] != 'O' ||
				headerBuffer[2] != 'P' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation graph root operations header");
			}

			// Read the root operation ids
			auto rootOperationIds = ReadOperationIdList(content);

			// Read the set of operations
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'O' ||
				headerBuffer[1] != 'P' ||
				headerBuffer[2] != 'S' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation graph operations header");
			}

			auto operationCount = ReadUInt32(content);
			auto operations = std::vector<OperationInfo>(operationCount);
			for (auto i = 0u; i < operationCount; i++)
			{
				operations[i] = ReadOperationInfo(content);
			}

			if (stream.peek() != std::char_traits<char>::eof())
			{
				throw std::runtime_error("Operation graph file corrupted - Did not read the entire file");
			}

			return OperationGraph(
				std::move(files),
				std::move(rootOperationIds),
				std::move(operations));
		}

	private:
		static OperationInfo ReadOperationInfo(char*& content)
		{
			// Write out the operation id
			auto id = ReadUInt32(content);

			// Write the operation title
			auto title = ReadString(content);

			// Write the command working directory
			auto workingDirectory = ReadString(content);

			// Write the command executable
			auto executable = ReadString(content);

			// Write the command arguments
			auto arguments = ReadString(content);

			// Write out the declared input files
			auto declaredInput = ReadFileIdList(content);

			// Write out the declared output files
			auto declaredOutput = ReadFileIdList(content);

			// Write out the read access list
			auto readAccess = ReadFileIdList(content);

			// Write out the write access list
			auto writeAccess = ReadFileIdList(content);

			// Write out the child operation ids
			auto children = ReadOperationIdList(content);

			// Write out the dependency count
			auto dependencyCount = ReadUInt32(content);

			// Write out the value indicating if there was a successful run
			auto wasSuccessfulRun = ReadBoolean(content);

			// Read the utc tick since January 1, 0001 at 00:00:00.000 in the Gregorian calendar
			auto evaluateTimeMilliseconds = ReadInt64(content);
			auto unixEvaluateTimeMilliseconds = std::chrono::milliseconds(evaluateTimeMilliseconds - UnixEpochOffset);
			auto evaluateTime = std::chrono::time_point<std::chrono::system_clock>(unixEvaluateTimeMilliseconds);

			// Write out the observed input files
			auto observedInput = ReadFileIdList(content);

			// Write out the observed output files
			auto observedOutput = ReadFileIdList(content);

			return OperationInfo(
				id,
				std::move(title),
				CommandInfo(
					Path(workingDirectory),
					Path(executable),
					std::move(arguments)),
				std::move(declaredInput),
				std::move(declaredOutput),
				std::move(readAccess),
				std::move(writeAccess),
				std::move(children),
				dependencyCount,
				wasSuccessfulRun,
				evaluateTime,
				std::move(observedInput),
				std::move(observedOutput));
		}

		static uint32_t ReadUInt32(char*& content)
		{
			uint32_t result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(uint32_t));

			return result;
		}

		static int64_t ReadInt64(char*& content)
		{
			int64_t result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(int64_t));

			return result;
		}

		static boolean ReadBoolean(char*& content)
		{
			uint32_t result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(uint32_t));

			return result != 0;
		}

		static std::string ReadString(char*& content)
		{
			auto size = ReadUInt32(content);
			auto result = std::string(size, '\0');
			Read(content, result.data(), size);

			return result;
		}

		static std::vector<FileId> ReadFileIdList(char*& content)
		{
			auto size = ReadUInt32(content);
			auto result = std::vector<FileId>(size);
			for (auto i = 0u; i < size; i++)
			{
				result[i] = ReadUInt32(content);
			}

			return result;
		}

		static std::vector<OperationId> ReadOperationIdList(char*& content)
		{
			auto size = ReadUInt32(content);
			auto result = std::vector<OperationId>(size);
			for (auto i = 0u; i < size; i++)
			{
				result[i] = ReadUInt32(content);
			}

			return result;
		}

		static void Read(char*& data, char* buffer, size_t count)
		{
			memcpy(buffer, data, count);
			data += count;
		}
	};
}
