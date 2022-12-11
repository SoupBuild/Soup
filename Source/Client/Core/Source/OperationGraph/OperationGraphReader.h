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
	#ifdef SOUP_BUILD
	export
	#endif
	class OperationGraphReader
	{
	private:
		// Binary Operation Graph file format
		static constexpr uint32_t FileVersion = 5;

	public:
		static OperationGraph Deserialize(std::istream& stream, FileSystemState& fileSystemState)
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
				auto insertResult = activeFileIdMap.emplace(fileId, activeFileId);
				if (!insertResult.second)
					throw std::runtime_error("Failed to insert file id lookup");
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
				operations[i] = ReadOperationInfo(content, activeFileIdMap);
			}

			if (stream.peek() != std::char_traits<char>::eof())
			{
				throw std::runtime_error("Operation graph file corrupted - Did not read the entire file");
			}

			return OperationGraph(
				std::move(rootOperationIds),
				std::move(operations));
		}

	private:
		static OperationInfo ReadOperationInfo(char*& content, const std::unordered_map<FileId, FileId>& activeFileIdMap)
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
			auto declaredInput = ReadFileIdList(content, activeFileIdMap);

			// Write out the declared output files
			auto declaredOutput = ReadFileIdList(content, activeFileIdMap);

			// Write out the read access list
			auto readAccess = ReadFileIdList(content, activeFileIdMap);

			// Write out the write access list
			auto writeAccess = ReadFileIdList(content, activeFileIdMap);

			// Write out the child operation ids
			auto children = ReadOperationIdList(content);

			// Write out the dependency count
			auto dependencyCount = ReadUInt32(content);

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
				dependencyCount);
		}

		static uint32_t ReadUInt32(char*& content)
		{
			uint32_t result = 0;
			Read(content, reinterpret_cast<char*>(&result), sizeof(uint32_t));

			return result;
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
