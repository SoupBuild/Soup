// <copyright file="BuildHistoryManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildHistory.h"
#include "BuildHistoryJson.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The build state manager
	/// </summary>
	export class BuildHistoryManager
	{
	private:
		static constexpr std::string_view BuildHistoryFileName = "BuildHistory.json";
		static constexpr std::string_view ProjectGenerateFolderName = ".soup";

	public:
		/// <summary>
		/// Load the build state from the provided directory
		/// </summary>
		static bool TryLoadState(
			const Path& directory, BuildHistory& result)
		{
			// Verify the requested file exists
			auto BuildHistoryFile = directory +
				Path(ProjectGenerateFolderName) +
				Path(BuildHistoryFileName);
			if (!System::IFileSystem::Current().Exists(BuildHistoryFile))
			{
				Log::Info("BuildHistory file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(BuildHistoryFile, false);

			// Read the contents of the build state file
			try
			{
				result = BuildHistoryJson::Deserialize(file->GetInStream());
				return true;
			}
			catch(std::runtime_error& ex)
			{
				Log::Info(ex.what());
				return false;
			}
			catch(...)
			{
				Log::Info("Failed to parse BuildHistory.");
				return false;
			}
		}

		/// <summary>
		/// Save the build state for the provided directory
		/// </summary>
		static void SaveState(const Path& directory, const BuildHistory& state)
		{
			auto buildProjectGenerateFolder = directory +
				Path(ProjectGenerateFolderName);
			auto BuildHistoryFile = buildProjectGenerateFolder +
				Path(BuildHistoryFileName);

			// Ensure the target directories exists
			if (!System::IFileSystem::Current().Exists(buildProjectGenerateFolder))
			{
				Log::Info("Create Directory: " + buildProjectGenerateFolder.ToString());
				System::IFileSystem::Current().CreateDirectory2(buildProjectGenerateFolder);
			}

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(BuildHistoryFile, false);

			// Write the build state to the file stream
			BuildHistoryJson::Serialize(state, file->GetOutStream());
		}
	};
}
