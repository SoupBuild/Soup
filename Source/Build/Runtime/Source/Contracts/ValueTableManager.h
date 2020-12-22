// <copyright file="ValueTableManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ValueTable.h"
#include "ValueTableReader.h"
#include "ValueTableWriter.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The Value Table state manager
	/// </summary>
	export class ValueTableManager
	{
	private:
		// Binary Build Value Table file format
		static constexpr std::string_view FileName = "ValueTable.bin";
		static constexpr std::string_view FolderName = ".soup/";

	public:
		/// <summary>
		/// Load the value table from the provided directory
		/// </summary>
		static bool TryLoadState(
			const Path& directory,
			ValueTable& result)
		{
			// Verify the requested file exists
			auto ValueTableFile = directory +
				Path(FolderName) +
				Path(FileName);
			if (!System::IFileSystem::Current().Exists(ValueTableFile))
			{
				Log::Info("Value Table file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(ValueTableFile, true);

			// Read the contents of the build state file
			try
			{
				result = ValueTableReader::Deserialize(file->GetInStream());
				return true;
			}
			catch(std::runtime_error& ex)
			{
				Log::Error(ex.what());
				return false;
			}
			catch(...)
			{
				Log::Error("Failed to parse value table");
				return false;
			}
		}

		/// <summary>
		/// Save the value table for the provided directory
		/// </summary>
		static void SaveState(
			const Path& directory,
			ValueTable& state)
		{
			auto targetFolder = directory +
				Path(FolderName);
			auto ValueTableFile = targetFolder +
				Path(FileName);

			// Ensure the target directories exists
			if (!System::IFileSystem::Current().Exists(targetFolder))
			{
				Log::Info("Create Directory: " + targetFolder.ToString());
				System::IFileSystem::Current().CreateDirectory2(targetFolder);
			}

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(ValueTableFile, true);

			// Write the build state to the file stream
			ValueTableWriter::Serialize(state, file->GetOutStream());
		}
	};
}
