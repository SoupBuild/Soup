// <copyright file="LocalUserConfigExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Constants.h"
#include "LocalUserConfigJson.h"

namespace Soup
{
	/// <summary>
	/// The LocalUserConfig extensions
	/// </summary>
	export class LocalUserConfigExtensions
	{
	public:
		/// <summary>
		/// Build up the LocalUserConfig file location from the active process location
		/// </summary>
		static Path GetLocalUserConfigFilePath()
		{
			auto result = System::IProcessManager::Current().GetProcessFileName();
			result.SetFilename(Constants::LocalUserConfigFileName);
			return result;
		}

		/// <summary>
		/// Attempt to load from file
		/// </summary>
		static LocalUserConfig LoadFromFile()
		{
			// Verify the requested file exists
			auto localUserConfigFile = GetLocalUserConfigFilePath();
			Log::Info("LocalConfig: " + localUserConfigFile.ToString());
			if (!System::IFileSystem::Current().Exists(localUserConfigFile))
			{
				throw std::runtime_error("LocalUserConfig file does not exist.");
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(localUserConfigFile);

			// Read the contents of the LocalUserConfig file
			try
			{
				auto result = LocalUserConfigJson::Deserialize(*file);
				return result;
			}
			catch (std::exception& ex)
			{
				Log::Diag(std::string("Deserialze Threw: ") + ex.what());
				throw std::runtime_error("Failed to parse LocalUserConfig file.");
			}
		}
	};
}
