// <copyright file="OperationHistoryManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationHistory.h"
#include "OperationHistoryReader.h"
#include "OperationHistoryWriter.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The operation state manager
	/// </summary>
	export class OperationHistoryManager
	{
	private:
		// Binary Build History file format
		static constexpr std::string_view FileName = "OperationHistory.bin";
		static constexpr std::string_view FolderName = ".soup/";

	public:
		/// <summary>
		/// Load the operation state from the provided directory
		/// </summary>
		static bool TryLoadState(
			const Path& directory,
			OperationHistory& result)
		{
			// Verify the requested file exists
			auto operationHistoryFile = directory +
				Path(FolderName) +
				Path(FileName);
			if (!System::IFileSystem::Current().Exists(operationHistoryFile))
			{
				Log::HighPriority("Operation History file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(operationHistoryFile, true);

			// Read the contents of the build state file
			try
			{
				result = OperationHistoryReader::Deserialize(file->GetInStream());
				return true;
			}
			catch(std::runtime_error& ex)
			{
				Log::Error(ex.what());
				return false;
			}
			catch(...)
			{
				Log::Error("Failed to parse Operation History.");
				return false;
			}
		}

		/// <summary>
		/// Save the operation state for the provided directory
		/// </summary>
		static void SaveState(const Path& directory, const OperationHistory& state)
		{
			auto targetFolder = directory +
				Path(FolderName);
			auto operationHistoryFile = targetFolder +
				Path(FileName);

			// Ensure the target directories exists
			if (!System::IFileSystem::Current().Exists(targetFolder))
			{
				Log::Info("Create Directory: " + targetFolder.ToString());
				System::IFileSystem::Current().CreateDirectory2(targetFolder);
			}

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(operationHistoryFile, true);

			// Write the build state to the file stream
			OperationHistoryWriter::Serialize(state, file->GetOutStream());
		}
	};
}
