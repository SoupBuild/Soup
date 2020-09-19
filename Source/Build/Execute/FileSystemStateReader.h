// <copyright file="FileSystemStateReader.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The file system state reader
	/// </summary>
	export class FileSystemStateReader
	{
	private:
		// File System State file format
		static constexpr uint32_t FileVersion = 1;

	public:
		static FileSystemState Deserialize(std::istream& stream)
		{
			// Read the File Header with version
			auto headerBuffer = std::array<char, 4>();
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'B' ||
				headerBuffer[1] != 'F' ||
				headerBuffer[2] != 'S' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid file system state file header");
			}

			auto fileVersion = ReadUInt32(stream);
			if (fileVersion != FileVersion)
			{
				throw std::runtime_error("File System state file version does not match expected");
			}

			// Read the id of this file system state
			auto id = ReadUInt32(stream);

			// Read the maximum file id that has been used
			auto maxFileId = ReadUInt32(stream);

			// Read the set of files
			stream.read(headerBuffer.data(), 4);
			if (headerBuffer[0] != 'F' ||
				headerBuffer[1] != 'I' ||
				headerBuffer[2] != 'S' ||
				headerBuffer[3] != '\0')
			{
				throw std::runtime_error("Invalid file system state files header");
			}

			auto fileCount = ReadUInt32(stream);
			auto files = std::unordered_map<FileId, Path>();
			for (auto i = 0; i < fileCount; i++)
			{
				// Read the command working directory
				auto fileId = ReadUInt32(stream);
				auto file = Path(ReadString(stream));

				auto insertResult = files.emplace(fileId, std::move(file));
				if (!insertResult.second)
					throw std::runtime_error("File System State file corrupted - The file id was not unique");
			}

			if (stream.peek() != std::char_traits<char>::eof())
			{
				throw std::runtime_error("File System State file corrupted - Did not read the entire file");
			}

			return FileSystemState(
				id,
				maxFileId,
				std::move(files));
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
	};
}
