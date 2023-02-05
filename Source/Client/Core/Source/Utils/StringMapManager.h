// <copyright file="StringMapManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// The string map manager
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class StringMapManager
	{
	public:
		/// <summary>
		/// Load the string map from the provided directory
		/// </summary>
		static bool TryLoad(
			const Path& filePath,
			std::map<std::string, std::string>& stringMap)
		{
			// Verify the requested file exists
			if (!System::IFileSystem::Current().Exists(filePath))
			{
				Log::Info("String Map file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(filePath, true);

			// Read the contents of the file
			try
			{
				std::string key;
				std::string value;
				while (std::getline(file->GetInStream(), key) && std::getline(file->GetInStream(), value))
				{
					stringMap.emplace(std::move(key), std::move(value));
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
				Log::Error("Failed to parse string map");
				return false;
			}
		}

		/// <summary>
		/// Save the path list for the provided directory
		/// </summary>
		static void Save(
			const Path& filePath,
			const std::map<std::string, std::string>& stringMap)
		{
			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(filePath, true);

			// Write each key and value on a line
			for (auto& [key, value] : stringMap)
			{
				file->GetOutStream() << key << '\n';
				file->GetOutStream() << value << '\n';
			}
		}
	};
}
