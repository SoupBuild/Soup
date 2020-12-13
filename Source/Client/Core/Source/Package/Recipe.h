// <copyright file="Recipe.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageReference.h"
#include "RecipeValue.h"

namespace Soup
{
	/// <summary>
	/// The recipe container
	/// </summary>
	export class Recipe
	{
	private:
		static constexpr const char* Property_Dependencies = "Dependencies";
		static constexpr const char* Property_Runtime = "Runtime";
		static constexpr const char* Property_Build = "Build";
		static constexpr const char* Property_Test = "Test";
		static constexpr const char* Property_Language = "Language";
		static constexpr const char* Property_Name = "Name";
		static constexpr const char* Property_Version = "Version";

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe() :
			_table()
		{
			SetName("");
			SetLanguage("");
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(
			std::string_view name,
			std::string_view language) :
			_table()
		{
			SetName(name);
			SetLanguage(language);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(
			std::string_view name,
			std::string_view language,
			std::optional<SemanticVersion> version,
			std::optional<std::vector<PackageReference>> runtimeDependencies,
			std::optional<std::vector<PackageReference>> buildDependencies,
			std::optional<std::vector<PackageReference>> testDependencies) :
			_table()
		{
			SetName(name);
			SetLanguage(language);

			if (version.has_value())
				SetVersion(version.value());

			if (runtimeDependencies.has_value())
				SetRuntimeDependencies(runtimeDependencies.value());

			if (buildDependencies.has_value())
				SetBuildDependencies(buildDependencies.value());

			if (testDependencies.has_value())
				SetTestDependencies(testDependencies.value());
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(RecipeTable table) :
			_table(std::move(table))
		{
			if (!HasValue(_table, Property_Name))
				throw std::runtime_error("Missing required property Name");
			if (!HasValue(_table, Property_Language))
				throw std::runtime_error("Missing required property Language");
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		RecipeValue& GetNameValue()
		{
			return GetValue(_table, Property_Name);
		}

		const std::string& GetName()
		{
			return GetNameValue().AsString();
		}

		void SetName(std::string_view value)
		{
			EnsureValue(_table, Property_Name).SetValueString(std::string(value));
		}

		/// <summary>
		/// Gets or sets the package language
		/// </summary>
		RecipeValue& GetLanguageValue()
		{
			return GetValue(_table, Property_Language);
		}

		const std::string& GetLanguage()
		{
			return GetLanguageValue().AsString();
		}

		void SetLanguage(std::string_view value)
		{
			EnsureValue(_table, Property_Language).SetValueString(std::string(value));
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		bool HasVersion()
		{
			return HasValue(_table, Property_Version);
		}

		SemanticVersion GetVersion()
		{
			if (!HasVersion())
				throw std::runtime_error("No version.");

			return SemanticVersion::Parse(
				GetValue(_table, Property_Version).AsString());
		}

		void SetVersion(SemanticVersion value)
		{
			return EnsureValue(_table, Property_Version).SetValueString(value.ToString());
		}

		/// <summary>
		/// Gets or sets the list of named dependency packages
		/// </summary>
		bool HasNamedDependencies(std::string_view name)
		{
			return HasDependencies() && HasValue(GetDependencies(), name);
		}

		std::vector<PackageReference> GetNamedDependencies(std::string_view name)
		{
			if (!HasNamedDependencies(name))
				throw std::runtime_error("No named dependencies.");

			auto& values = GetValue(GetDependencies(), name).AsList();
			auto result = std::vector<PackageReference>();
			for (auto& value : values)
			{
				result.push_back(PackageReference::Parse(value.AsString()));
			}

			return result;
		}

		void SetNamedDependencies(std::string_view name, const std::vector<PackageReference>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value.ToString()));
			}

			EnsureValue(EnsureDependencies(), name).SetValueList(std::move(stringValues));
		}

		/// <summary>
		/// Gets or sets the list of runtime dependency packages
		/// </summary>
		bool HasRuntimeDependencies()
		{
			return HasNamedDependencies(Property_Runtime);
		}

		std::vector<PackageReference> GetRuntimeDependencies()
		{
			return GetNamedDependencies(Property_Runtime);
		}

		void SetRuntimeDependencies(const std::vector<PackageReference>& values)
		{
			SetNamedDependencies(Property_Runtime, values);
		}

		/// <summary>
		/// Gets or sets the list of build dependency packages
		/// </summary>
		bool HasBuildDependencies()
		{
			return HasNamedDependencies(Property_Build);
		}

		std::vector<PackageReference> GetBuildDependencies()
		{
			return GetNamedDependencies(Property_Build);
		}

		void SetBuildDependencies(const std::vector<PackageReference>& values)
		{
			SetNamedDependencies(Property_Build, values);
		}
		
		/// <summary>
		/// Gets or sets the list of test dependency packages
		/// </summary>
		bool HasTestDependencies()
		{
			return HasNamedDependencies(Property_Test);
		}

		std::vector<PackageReference> GetTestDependencies()
		{
			return GetNamedDependencies(Property_Test);
		}

		void SetTestDependencies(const std::vector<PackageReference>& values)
		{
			SetNamedDependencies(Property_Test, values);
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
		bool operator ==(const Recipe& rhs) const
		{
			return _table == rhs._table;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const Recipe& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		/// <summary>
		/// Gets or sets the table of dependency packages
		/// </summary>
		bool HasDependencies()
		{
			return HasValue(_table, Property_Dependencies);
		}

		RecipeTable& GetDependencies()
		{
			if (!HasDependencies())
				throw std::runtime_error("No dependencies.");

			auto& values = GetValue(_table, Property_Dependencies).AsTable();
			return values;
		}

		RecipeTable& EnsureDependencies()
		{
			auto& value = EnsureValue(_table, Property_Dependencies);
			switch (value.GetType())
			{
				case RecipeValueType::Table:
					// All good.
					break;
				case RecipeValueType::Empty:
					value.SetValueTable(RecipeTable());
					break;
				default:
					throw std::runtime_error("The recipe already has a non-table dependencies property");
			}

			return value.AsTable();
		}

		bool HasValue(RecipeTable& table, std::string_view key)
		{
			return table.contains(key.data());
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

		RecipeValue& EnsureValue(RecipeTable& table, std::string_view key)
		{
			auto findItr = table.find(key.data());
			if (findItr != table.end())
			{
				return findItr->second;
			}
			else
			{
				auto insertResult = table.emplace(key.data(), RecipeValue());
				if (insertResult.second)
				{
					return insertResult.first->second;
				}
				else
				{
					throw std::runtime_error("Failed to insert a recipe value.");
				}
			}
		}

	private:
		RecipeTable _table;
	};
}
