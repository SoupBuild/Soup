// <copyright file="FileSystemStateWriter.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The file system state writer
	/// </summary>
	export class FileSystemStateWriter
	{
	private:
		// File System State file format
		static constexpr uint32_t FileVersion = 1;

	public:
		static void Serialize(const FileSystemState& state, std::ostream& stream)
		{
			// Write the File Header with version
			stream.write("BFS\0", 4);
			WriteValue(stream, FileVersion);

			// Write out the set of files
			auto& files = state.GetFiles();
			stream.write("FIL\0", 4);
			WriteValue(stream, static_cast<uint32_t>(files.size()));
			for (auto& file : files)
			{
				// Write the file id + path length + path
				WriteValue(stream, file.first);
				WriteValue(stream, file.second.ToString());
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
	};
}
