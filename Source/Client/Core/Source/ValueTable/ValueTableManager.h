﻿// <copyright file="ValueTableManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Value.h"
#include "ValueTableReader.h"
#include "ValueTableWriter.h"

namespace Soup::Core
{
	/// <summary>
	/// The Value Table state manager
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class ValueTableManager
	{
	public:
		/// <summary>
		/// Load the value table from the target file
		/// </summary>
		static bool TryLoadState(
			const Path& valueTableFile,
			ValueTable& result)
		{
			// Verify the requested file exists
			if (!System::IFileSystem::Current().Exists(valueTableFile))
			{
				Log::Info("Value Table file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(valueTableFile, true);

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
		/// Save the value table for the target file
		/// </summary>
		static void SaveState(
			const Path& valueTableFile,
			ValueTable& state)
		{
			auto targetFolder = valueTableFile.GetParent();

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(valueTableFile, true);

			// Write the build state to the file stream
			ValueTableWriter::Serialize(state, file->GetOutStream());
		}
	};
}
