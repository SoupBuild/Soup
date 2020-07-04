// <copyright file="BuildHistoryJson.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildHistory.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The build state json serializer
	/// </summary>
	export class BuildHistoryJson
	{
	private:
		static constexpr const char* Property_File = "file";
		static constexpr const char* Property_KnownFiles = "knownFiles";
		static constexpr const char* Property_Includes = "includes";

	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static BuildHistory Deserialize(std::istream& stream)
		{
			// Read the entire file into a string
			std::string content(
				(std::istreambuf_iterator<char>(stream)),
				std::istreambuf_iterator<char>());

			// Read the contents of the build state file
			std::string error = "";
			auto jsonRoot = json11::Json::parse(content, error);
			if (jsonRoot.is_null())
			{
				auto message = "Failed to parse the build state json: " + error;
				throw std::runtime_error(std::move(message));
			}
			else
			{
				return LoadJsonBuildHistory(jsonRoot);
			}
		}

		/// <summary>
		/// Save the BuildHistory to the root file
		/// </summary>
		static void Serialize(const BuildHistory& state, std::ostream& stream)
		{
			// Serialize the contents of the build state
			json11::Json json = BuildJsonBuildHistory(state);

			stream << json.dump();
		}

	private:
		static BuildHistory LoadJsonBuildHistory(const json11::Json& value)
		{
			std::vector<FileInfo> knownFiles;

			if (!value[Property_KnownFiles].is_null())
			{
				auto values = std::vector<FileInfo>();
				for (auto& value : value[Property_KnownFiles].array_items())
				{
					auto fileInfo = LoadJsonFileInfo(value);
					values.push_back(std::move(fileInfo));
				}

				knownFiles = std::move(values);
			}
			else
			{
				throw std::runtime_error("Missing Required field: knownFiles.");
			}

			return BuildHistory(
				std::move(knownFiles));
		}

		static FileInfo LoadJsonFileInfo(const json11::Json& value)
		{
			Path file;
			std::vector<Path> includes;

			if (!value[Property_File].is_null())
			{
				file = Path(value[Property_File].string_value());
			}
			else
			{
				throw std::runtime_error("Missing Required field: file.");
			}

			if (!value[Property_Includes].is_null())
			{
				auto values = std::vector<Path>();
				for (auto& value : value[Property_Includes].array_items())
				{
					values.push_back(Path(value.string_value()));
				}

				includes = std::move(values);
			}
			else
			{
				throw std::runtime_error("Missing Required field: includes.");
			}

			return FileInfo(
				std::move(file),
				std::move(includes));
		}

		static json11::Json BuildJsonBuildHistory(const BuildHistory& state)
		{
			json11::Json::object result = {};

			// Add required fields
			json11::Json::array knownFiles;
			for (auto& value : state.GetKnownFiles())
			{
				knownFiles.push_back(BuildJsonFileInfo(value));
			}

			result[Property_KnownFiles] = std::move(knownFiles);

			return result;
		}

		static json11::Json BuildJsonFileInfo(const FileInfo& info)
		{
			json11::Json::object result = {};

			// Add required fields
			result[Property_File] = info.File.ToString();

			json11::Json::array includes;
			for (auto& value : info.Includes)
			{
				includes.push_back(value.ToString());
			}

			result[Property_Includes] = std::move(includes);

			return result;
		}
	};
}
