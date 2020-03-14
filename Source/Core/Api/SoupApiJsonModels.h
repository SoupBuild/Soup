// <copyright file="SoupApiJsonModels.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Models/PackageCreateModel.h"
#include "Models/PackageResultModel.h"

namespace Soup::Api
{
	/// <summary>
	/// The soup api models json serialize manager
	/// </summary>
	export class SoupApiJsonModels
	{
	private:
		static constexpr const char* Property_Name = "name";
		static constexpr const char* Property_Description = "description";
		static constexpr const char* Property_Latest = "latest";

	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static PackageResultModel ParsePackageResult(const std::string& content)
		{
			// Read the contents of the recipe file
			std::string error = "";
			auto jsonRoot = json11::Json::parse(content, error);
			if (jsonRoot.is_null())
			{
				auto message = "Failed to parse the package result json: " + error;
				throw std::runtime_error(std::move(message));
			}
			else
			{
				if (!jsonRoot.is_object())
					throw std::runtime_error("Root json was not an object.");

				return ParsePackageResult(jsonRoot);
			}
		}

		/// <summary>
		/// Serialize the package create model
		/// </summary>
		static void SerializePackageCreate(const PackageCreateModel& model, std::ostream& stream)
		{
			json11::Json::object result = {};

			// Add required fields
			result[Property_Name] = model.GetName();

			if (model.HasDescription())
			{
				result[Property_Description] = model.GetDescription();
			}

			stream << json11::Json(result).dump();
		}

	private:
		static PackageResultModel ParsePackageResult(const json11::Json& value)
		{
			std::string name;
			std::optional<SemanticVersion> latest;

			if (!value[Property_Name].is_null())
			{
				name = value[Property_Name].string_value();
			}
			else
			{
				throw std::runtime_error("Missing Required field: name.");
			}

			if (!value[Property_Latest].is_null())
			{
				latest = SemanticVersion::Parse(value[Property_Latest].string_value());
			}

			return PackageResultModel(
				std::move(name),
				latest);
		}
	};
}
