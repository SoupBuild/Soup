// <copyright file="LocalUserConfigJson.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "LocalUserConfig.h"

namespace Soup
{
	/// <summary>
	/// The local user config json serialize manager
	/// </summary>
	export class LocalUserConfigJson
	{
	private:
		static constexpr const char* Property_RuntimeCompiler = "runtimeCompiler";
		static constexpr const char* Property_MSVC = "msvc";
		static constexpr const char* Property_Clang = "clang";
		static constexpr const char* Property_WindowsSDKIncludes = "windowsSDKIncludes";
		static constexpr const char* Property_WindowsSDKLibraries = "windowsSDKLibraries";

	public:
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		static bool TryLoadFromFile(
			const Path& filePath,
			LocalUserConfig& result)
		{
			// Verify the requested file exists
			if (!IFileSystem::Current().Exists(filePath))
			{
				Log::Info("LocalUserConfig file does not exist.");
				return false;
			}

			// Open the file to read from
			auto file = IFileSystem::Current().OpenRead(filePath);

			// Read the contents of the file
			try
			{
				result = Deserialize(*file);
				return true;
			}
			catch (std::exception& ex)
			{
				Log::Trace(std::string("Deserialze Threw: ") + ex.what());
				Log::Info("Failed to parse LocalUserConfig.");
				return false;
			}
		}

		/// <summary>
		/// Load from local user config
		/// </summary>
		static LocalUserConfig Deserialize(std::istream& stream)
		{
			// Read the entire file into a string
			std::string content(
				(std::istreambuf_iterator<char>(stream)),
				std::istreambuf_iterator<char>());

			// Read the contents of the local user config file
			std::string error = "";
			auto jsonRoot = json11::Json::parse(content, error);
			if (jsonRoot.is_null())
			{
				auto message = "Failed to parse the local user config json: " + error;
				throw std::runtime_error(std::move(message));
			}
			else
			{
				return LoadJsonLocalUserConfig(jsonRoot);
			}
		}

	private:
		static LocalUserConfig LoadJsonLocalUserConfig(const json11::Json& value)
		{
			std::string runtimeCompiler;
			std::optional<std::string> msvcRootPath;
			std::optional<std::string> clangToolPath;
			std::optional<std::vector<std::string>> windowsSDKIncludePaths;
			std::optional<std::vector<std::string>> windowsSDKLibraryPaths;

			if (!value[Property_RuntimeCompiler].is_null())
			{
				runtimeCompiler = value[Property_RuntimeCompiler].string_value();
			}
			else
			{
				throw std::runtime_error("Missing Required field: runtimeCompiler.");
			}

			if (!value[Property_MSVC].is_null())
			{
				msvcRootPath = value[Property_MSVC].string_value();
			}

			if (!value[Property_Clang].is_null())
			{
				clangToolPath = value[Property_Clang].string_value();
			}

			if (!value[Property_WindowsSDKIncludes].is_null())
			{
				auto values = std::vector<std::string>();
				for (auto& value : value[Property_WindowsSDKIncludes].array_items())
				{
					values.push_back(value.string_value());
				}

				windowsSDKIncludePaths = std::move(values);
			}

			if (!value[Property_WindowsSDKLibraries].is_null())
			{
				auto values = std::vector<std::string>();
				for (auto& value : value[Property_WindowsSDKLibraries].array_items())
				{
					values.push_back(value.string_value());
				}

				windowsSDKLibraryPaths = std::move(values);
			}

			return LocalUserConfig(
				std::move(runtimeCompiler),
				std::move(msvcRootPath),
				std::move(clangToolPath),
				std::move(windowsSDKIncludePaths),
				std::move(windowsSDKLibraryPaths));
		}
	};
}
