// <copyright file="OperationGraphManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationGraph.h"
#include "OperationGraphReader.h"
#include "OperationGraphWriter.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The operation state manager
	/// </summary>
	export class OperationGraphManager
	{
	private:
		// Binary Build Operation Graph file format
		static constexpr std::string_view FileName = "OperationGraph.bin";
		static constexpr std::string_view FolderName = ".soup/";

	public:
		/// <summary>
		/// Load the operation state from the provided directory
		/// </summary>
		static bool TryLoadState(
			const Path& directory,
			OperationGraph& result)
		{
			// Verify the requested file exists
			auto OperationGraphFile = directory +
				Path(FolderName) +
				Path(FileName);
			if (!System::IFileSystem::Current().Exists(OperationGraphFile))
			{
				Log::Info("Operation graph file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(OperationGraphFile, true);

			// Read the contents of the build state file
			try
			{
				auto loadedResult = OperationGraphReader::Deserialize(file->GetInStream());
				result = loadedResult;
				return true;
			}
			catch(std::runtime_error& ex)
			{
				Log::Error(ex.what());
				return false;
			}
			catch(...)
			{
				Log::Error("Failed to parse operation graph");
				return false;
			}
		}

		/// <summary>
		/// Save the operation state for the provided directory
		/// </summary>
		static void SaveState(const Path& directory, const OperationGraph& state)
		{
			auto targetFolder = directory +
				Path(FolderName);
			auto OperationGraphFile = targetFolder +
				Path(FileName);

			// Ensure the target directories exists
			if (!System::IFileSystem::Current().Exists(targetFolder))
			{
				Log::Info("Create Directory: " + targetFolder.ToString());
				System::IFileSystem::Current().CreateDirectory2(targetFolder);
			}

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(OperationGraphFile, true);

			// Write the build state to the file stream
			OperationGraphWriter::Serialize(state, file->GetOutStream());
		}
	};
}
