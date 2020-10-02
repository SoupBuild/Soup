// <copyright file="FilESystemStateManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"
#include "FileSystemStateReader.h"
#include "FileSystemStateWriter.h"

namespace Soup::Build::Evaluate
{
	/// <summary>
	/// The file system state manager
	/// </summary>
	export class FileSystemStateManager
	{
	private:
		// Binary Build History file format
		static constexpr std::string_view FileName = "FileSystemState.bin";
		static constexpr std::string_view FolderName = "State/";

	public:
		/// <summary>
		/// Load the file system state from the provided directory
		/// </summary>
		static bool TryLoadState(
			const Path& directory,
			FileSystemState& result)
		{
			// Verify the requested file exists
			auto fileSystemStateFile = directory +
				Path(FolderName) +
				Path(FileName);
			if (!System::IFileSystem::Current().Exists(fileSystemStateFile))
			{
				Log::HighPriority("File system state file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(fileSystemStateFile, true);

			// Read the contents of the build state file
			try
			{
				result = FileSystemStateReader::Deserialize(file->GetInStream());
				return true;
			}
			catch(std::runtime_error& ex)
			{
				Log::Warning(ex.what());
				return false;
			}
			catch(...)
			{
				Log::Warning("Failed to parse file system state.");
				return false;
			}
		}

		/// <summary>
		/// Save the file system state for the provided directory
		/// </summary>
		static void SaveState(const Path& directory, const FileSystemState& state)
		{
			auto targetFolder = directory +
				Path(FolderName);
			auto fileSystemStateFile = targetFolder +
				Path(FileName);

			// Ensure the target directories exists
			if (!System::IFileSystem::Current().Exists(targetFolder))
			{
				Log::Info("Create Directory: " + targetFolder.ToString());
				System::IFileSystem::Current().CreateDirectory2(targetFolder);
			}

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(fileSystemStateFile, true);

			// Write the build state to the file stream
			FileSystemStateWriter::Serialize(state, file->GetOutStream());
		}
	};
}
