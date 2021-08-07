﻿// <copyright file="LocalUserConfig.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe/RecipeValue.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The local user config container
	/// </summary>
	export class LocalUserConfig
	{
	private:
		static constexpr const char* Property_SDKs = "SDKs";

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LocalUserConfig"/> class.
		/// </summary>
		LocalUserConfig() :
			_table()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="LocalUserConfig"/> class.
		/// </summary>
		LocalUserConfig(RecipeTable table) :
			_table(std::move(table))
		{
		}

		/// <summary>
		/// Gets or sets the list of SDKs
		/// </summary>
		bool HasSDKs()
		{
			return HasValue(Property_SDKs);
		}

		std::vector<std::string> GetSDKs()
		{
			if (!HasSDKs())
				throw std::runtime_error("No SDKs.");

			auto& values = GetValue(Property_SDKs).AsList();
			auto result = std::vector<std::string>();
			for (auto& value : values)
			{
				result.push_back(value.AsString());
			}

			return result;
		}

		/// <summary>
		/// Raw access
		/// </summary>
		RecipeTable& GetTable()
		{
			return _table;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const LocalUserConfig& rhs) const
		{
			return _table == rhs._table;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const LocalUserConfig& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		bool HasValue(std::string_view key)
		{
			return _table.contains(key.data());
		}

		Runtime::RecipeValue& GetValue(std::string_view key)
		{
			auto findItr = _table.find(key.data());
			if (findItr != _table.end())
			{
				return findItr->second;
			}
			else
			{
				throw std::runtime_error("Requested recipe value does not exist in the root table.");
			}
		}

	private:
		RecipeTable _table;
	};
}
