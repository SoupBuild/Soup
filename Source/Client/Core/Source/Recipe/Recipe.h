// <copyright file="Recipe.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "LanguageReference.h"
#include "PackageReference.h"
#include "RecipeValue.h"

namespace Soup::Core
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
		static constexpr const char* Property_Reference = "Reference";

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe() :
			_table()
		{
			SetName("");
			SetLanguage(LanguageReference());
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(
			std::string_view name,
			LanguageReference language) :
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
			LanguageReference language,
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
		/// Generic Helpers
		/// </summary>
		void SetRootValue(std::string_view key, std::string value)
		{
			SetValue(_table, key, RecipeValue(std::move(value)));
		}

		void SetRootValue(std::string_view key, const std::vector<std::string>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value));
			}

			SetValue(_table, key, RecipeValue(std::move(stringValues)));
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		const RecipeValue& GetNameValue() const
		{
			return GetValue(_table, Property_Name);
		}

		RecipeValue& GetNameValue()
		{
			return GetValue(_table, Property_Name);
		}

		const std::string& GetName() const
		{
			auto& nameValue = GetNameValue();
			if (nameValue.IsString())
				return nameValue.AsString();
			else
				throw std::runtime_error("The Recipe name must be of type String");
		}

		void SetName(std::string_view value)
		{
			SetRootValue(Property_Name, std::string(value));
		}

		/// <summary>
		/// Gets or sets the package language
		/// </summary>
		bool HasLanguage() const
		{
			return HasValue(_table, Property_Language);
		}

		const RecipeValue& GetLanguageValue() const
		{
			return GetValue(_table, Property_Language);
		}

		LanguageReference GetLanguage() const
		{
			auto& languageValue = GetLanguageValue();
			if (languageValue.IsString())
			{
				LanguageReference result;
				if (LanguageReference::TryParse(languageValue.AsString(), result))
					return result;
				else
					throw std::runtime_error("Invalid Language format in Recipe: " + languageValue.AsString());
			}
			else
			{
				throw std::runtime_error("The Recipe language must be of type String");
			}
		}

		void SetLanguage(const LanguageReference& value)
		{
			SetRootValue(Property_Language, value.ToString());
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		bool HasVersion() const
		{
			return HasValue(_table, Property_Version);
		}

		SemanticVersion GetVersion() const
		{
			if (!HasVersion())
				throw std::runtime_error("No version.");

			auto& versionValue = GetValue(_table, Property_Version);
			if (versionValue.IsString())
				return SemanticVersion::Parse(versionValue.AsString());
			else
				throw std::runtime_error("The Recipe version must be of type String");
		}

		void SetVersion(SemanticVersion value)
		{
			SetRootValue(Property_Version, value.ToString());
		}

		/// <summary>
		/// Gets or sets the list of named dependency packages
		/// </summary>
		std::vector<std::string> GetDependencyTypes() const
		{
			auto result = std::vector<std::string>();
			if (HasDependencies())
			{
				for (auto value : GetDependencies())
				{
					result.push_back(value.first);
				}
			}

			return result;
		}

		bool HasNamedDependencies(std::string_view name) const
		{
			return HasDependencies() && HasValue(GetDependencies(), name);
		}

		std::vector<PackageReference> GetNamedDependencies(std::string_view name) const
		{
			if (!HasNamedDependencies(name))
				throw std::runtime_error("No named dependencies.");

			auto& values = GetValue(GetDependencies(), name).AsList();
			auto result = std::vector<PackageReference>();
			for (auto& value : values)
			{
				// A dependency can either be a string or a table with reference key
				if (value.IsString())
				{
					result.push_back(PackageReference::Parse(value.AsString()));
				}
				else if (value.IsTable())
				{
					auto& valueTable = value.AsTable();
					if (!HasValue(valueTable, Property_Reference))
						throw std::runtime_error("Recipe dependency table missing required Reference value.");
					auto& referenceValue = GetValue(valueTable, Property_Reference);
					if (referenceValue.IsString())
					{
						result.push_back(PackageReference::Parse(referenceValue.AsString()));
					}
					else
					{
						throw std::runtime_error("Recipe dependency Reference must be type String.");
					}
				}
				else
				{
					throw std::runtime_error("Unknown Recipe dependency type.");
				}
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

			SetValue(EnsureDependencies(), name, RecipeValue(std::move(stringValues)));
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
		bool HasDependencies() const
		{
			return HasValue(_table, Property_Dependencies);
		}

		const RecipeTable& GetDependencies() const
		{
			if (!HasDependencies())
				throw std::runtime_error("No dependencies.");

			auto& values = GetValue(_table, Property_Dependencies).AsTable();
			return values;
		}

		RecipeTable& EnsureDependencies()
		{
			auto& value = EnsureTableValue(_table, Property_Dependencies);
			return value.AsTable();
		}

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

		RecipeValue& SetValue(RecipeTable& table, std::string_view key, RecipeValue&& value)
		{
			auto insertResult = table.insert_or_assign(key.data(), std::move(value));
			if (insertResult.second)
			{
				return insertResult.first->second;
			}
			else
			{
				throw std::runtime_error("Failed to insert a recipe value.");
			}
		}

		RecipeValue& EnsureTableValue(RecipeTable& table, std::string_view key)
		{
			auto findItr = table.find(key.data());
			if (findItr != table.end())
			{
				if (findItr->second.GetType() != RecipeValueType::Table)
				{
					throw std::runtime_error("The recipe already has a non-table dependencies property");
				}

				return findItr->second;
			}
			else
			{
				auto insertResult = table.emplace(key.data(), RecipeValue(RecipeTable()));
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
