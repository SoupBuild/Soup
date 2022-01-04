// <copyright file="PathListManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// The path list manager
	/// </summary>
	export class PathListManager
	{
	public:
		/// <summary>
		/// Load the path list from the provided directory
		/// </summary>
		static bool TryLoad(
			const Path& pathListFile,
			std::vector<Path>& pathList)
		{
			// Verify the requested file exists
			if (!System::IFileSystem::Current().Exists(pathListFile))
			{
				Log::Info("Path list file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(pathListFile, true);

			// Read the contents of the file
			try
			{
				std::string value;
				while (std::getline(file->GetInStream(), value))
				{
					pathList.push_back(Path(value));
				}

				return true;
			}
			catch(std::runtime_error& ex)
			{
				Log::Error(ex.what());
				return false;
			}
			catch(...)
			{
				Log::Error("Failed to parse path list");
				return false;
			}
		}

		/// <summary>
		/// Save the path list for the provided directory
		/// </summary>
		static void Save(
			const Path& pathListFile,
			const std::vector<Path>& pathList)
		{
			auto targetFolder = pathListFile.GetParent();

			// Ensure the target directories exists
			if (!System::IFileSystem::Current().Exists(targetFolder))
			{
				Log::Info("Create Directory: " + targetFolder.ToString());
				System::IFileSystem::Current().CreateDirectory2(targetFolder);
			}

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(pathListFile, true);

			// Write each path on a line
			for (auto& value : pathList)
			{
				file->GetOutStream() << value.ToString() << '\n';
			}
		}
	};
}
