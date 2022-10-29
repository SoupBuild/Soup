// <copyright file="OperationResultsReader.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"

namespace Soup::Core
{
	/// <summary>
	/// The operation results state reader
	/// </summary>
	export class OperationResultsReader
	{
	private:
		// Binary Operation Results file format
		static constexpr uint32_t FileVersion = 1;

		// The offset from January 1, 1970 at 00:00:00.000 to January 1, 0001 at 00:00:00.000 in the Gregorian calendar
		static constexpr long long UnixEpochOffset = 62135596800000;

	public:
		static OperationResults Deserialize(std::istream& stream, FileSystemState& fileSystemState)
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
				headerBuffer[2] != 'R' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation results file header");
			}

			auto fileVersion = ReadUInt32(content);
			if (fileVersion != FileVersion)
			{
				throw std::runtime_error("Operation results file version does not match expected");
			}

			// Read the set of files
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'F' ||
				headerBuffer[1] != 'I' ||
				headerBuffer[2] != 'S' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation results files header");
			}

			// Map up the incoming file ids to the active file system state ids
			auto fileCount = ReadUInt32(content);
			auto activeFileIdMap = std::unordered_map<FileId, FileId>();
			for (auto i = 0u; i < fileCount; i++)
			{
				// Read the command working directory
				auto fileId = ReadUInt32(content);

				auto fileString = ReadString(content);
				auto file = Path::Load(std::move(fileString));

				auto activeFileId = fileSystemState.ToFileId(file);
				auto insertMapResult = activeFileIdMap.emplace(fileId, activeFileId);
				if (!insertMapResult.second)
					throw std::runtime_error("Failed to insert file id lookup");
			}

			// Read the set of operations
			Read(content, headerBuffer.data(), 4);
			if (headerBuffer[0] != 'R' ||
				headerBuffer[1] != 'T' ||
				headerBuffer[2] != 'S' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid operation results results header");
			}

			auto resultCount = ReadUInt32(content);
			auto results = OperationResults();
			for (auto i = 0u; i < resultCount; i++)
			{
				ReadOperationResult(content, activeFileIdMap, results);
			}

			if (stream.peek() != std::char_traits<char>::eof())
			{
				throw std::runtime_error("Operation results file corrupted - Did not read the entire file");
			}

			return results;
		}

	private:
		static void ReadOperationResult(
			char*& content,
			const std::unordered_map<FileId, FileId>& activeFileIdMap,
			OperationResults& results)
		{
			// Read the operation id
			auto operationId = ReadUInt32(content);

			// Read the value indicating if there was a successful run
			auto wasSuccessfulRun = ReadBoolean(content);

			// Read the utc tick since January 1, 0001 at 00:00:00.000 in the Gregorian calendar
			auto evaluateTimeMilliseconds = ReadInt64(content);
			auto unixEvaluateTimeMilliseconds = std::chrono::milliseconds(evaluateTimeMilliseconds - UnixEpochOffset);
			auto evaluateTime = std::chrono::time_point<std::chrono::system_clock>(unixEvaluateTimeMilliseconds);

			// Read the observed input files
			auto observedInput = ReadFileIdList(content, activeFileIdMap);

			// Read the observed output files
			auto observedOutput = ReadFileIdList(content, activeFileIdMap);

			auto result = OperationResult(
				wasSuccessfulRun,
				evaluateTime,
				std::move(observedInput),
				std::move(observedOutput));

			results.AddOrUpdateOperationResult(operationId, std::move(result));
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

		static std::vector<FileId> ReadFileIdList(char*& content, const std::unordered_map<FileId, FileId>& activeFileIdMap)
		{
			auto size = ReadUInt32(content);
			auto result = std::vector<FileId>(size);
			for (auto i = 0u; i < size; i++)
			{
				auto fileId = ReadUInt32(content);

				// Find the active file id that maps to the cached file id
				auto findActiveFileId = activeFileIdMap.find(fileId);
				if (findActiveFileId == activeFileIdMap.end())
					throw std::runtime_error("Could not find file id in active map");

				result[i] = findActiveFileId->second;
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
