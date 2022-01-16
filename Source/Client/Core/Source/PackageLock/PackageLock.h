// <copyright file="PackageLock.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe/RecipeValue.h"

namespace Soup::Core
{
	/// <summary>
	/// The package lock container
	/// </summary>
	export class PackageLock
	{
	private:
		static constexpr const char* Property_Projects = "Projects";
		static constexpr const char* Property_Name = "Name";
		static constexpr const char* Property_Version = "Version";

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PackageLock"/> class.
		/// </summary>
		PackageLock() :
			_table()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageLock"/> class.
		/// </summary>
		PackageLock(RecipeTable table) :
			_table(std::move(table))
		{
		}

		/// <summary>
		/// Gets or sets the table of projects
		/// </summary>
		bool HasProjects()
		{
			return HasValue(_table, Property_Projects);
		}

		RecipeTable GetProjects()
		{
			if (!HasProjects())
				throw std::runtime_error("No projects.");

			auto values = GetValue(_table, Property_Projects).AsTable();
			return values;
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
		bool operator ==(const PackageLock& rhs) const
		{
			return _table == rhs._table;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const PackageLock& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		bool HasValue(const RecipeTable& table, std::string_view key) const
		{
			return table.contains(key.data());
		}

		const RecipeValue& GetValue(const RecipeTable& table, std::string_view key) const
		{
			auto findItr = table.find(key.data());
			if (findItr != table.end())
			{
				return findItr->second;
			}
			else
			{
				throw std::runtime_error("Requested recipe value does not exist in the table.");
			}
		}

		RecipeValue& GetValue(RecipeTable& table, std::string_view key)
		{
			auto findItr = table.find(key.data());
			if (findItr != table.end())
			{
				return findItr->second;
			}
			else
			{
				throw std::runtime_error("Requested recipe value does not exist in the table.");
			}
		}

	private:
		RecipeTable _table;
	};
}
