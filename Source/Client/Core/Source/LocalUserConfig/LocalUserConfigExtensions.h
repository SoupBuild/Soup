// <copyright file="LocalUserConfigExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "LocalUserConfig.h"
#include "Recipe/RecipeToml.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The local user config extensions
	/// </summary>
	export class LocalUserConfigExtensions
	{
	public:
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		static bool TryLoadLocalUserConfigFromFile(
			const Path& localUserConfigFile,
			LocalUserConfig& result)
		{
			// Verify the requested file exists
			Log::Diag("Load Local User Config: " + localUserConfigFile.ToString());
			if (!System::IFileSystem::Current().Exists(localUserConfigFile))
			{
				Log::Warning("Local User Config file does not exist.");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(localUserConfigFile, true);

			// Read the contents of the local user config file
			try
			{
				result = LocalUserConfig(
					Runtime::RecipeToml::Deserialize(
						localUserConfigFile,
						file->GetInStream()));
				return true;
			}
			catch (std::exception& ex)
			{
				Log::Error(std::string("Deserialize Threw: ") + ex.what());
				Log::Info("Failed to parse local user config.");
				return false;
			}
		}
	};
}
