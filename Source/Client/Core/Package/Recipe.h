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
		static constexpr const char* Property_DevDependencies = "DevDependencies";
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
			SetVersion(SemanticVersion());
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
			std::optional<std::vector<PackageReference>> dependencies,
			std::optional<std::vector<PackageReference>> devDependencies) :
			_table()
		{
			SetName(name);
			SetLanguage(language);

			if (version.has_value())
				SetVersion(version.value());

			if (dependencies.has_value())
				SetDependencies(dependencies.value());

			if (devDependencies.has_value())
				SetDevDependencies(devDependencies.value());
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(RecipeTable table) :
			_table(std::move(table))
		{
			if (!HasValue(Property_Name))
				throw std::runtime_error("Missing required property Name");
			if (!HasValue(Property_Language))
				throw std::runtime_error("Missing required property Language");
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		RecipeValue& GetNameValue()
		{
			return GetValue(Property_Name);
		}

		const std::string& GetName()
		{
			return GetNameValue().AsString();
		}

		void SetName(std::string_view value)
		{
			EnsureValue(Property_Name).SetValueString(std::string(value));
		}

		/// <summary>
		/// Gets or sets the package language
		/// </summary>
		RecipeValue& GetLanguageValue()
		{
			return GetValue(Property_Language);
		}

		const std::string& GetLanguage()
		{
			return GetLanguageValue().AsString();
		}

		void SetLanguage(std::string_view value)
		{
			EnsureValue(Property_Language).SetValueString(std::string(value));
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		bool HasVersion()
		{
			return HasValue(Property_Version);
		}

		SemanticVersion GetVersion()
		{
			if (!HasVersion())
				throw std::runtime_error("No version.");

			return SemanticVersion::Parse(
				GetValue(Property_Version).AsString());
		}

		void SetVersion(SemanticVersion value)
		{
			return EnsureValue(Property_Version).SetValueString(value.ToString());
		}

		/// <summary>
		/// Gets or sets the list of dependency packages
		/// TODO: Observable?
		/// </summary>
		bool HasDependencies()
		{
			return HasValue(Property_Dependencies);
		}

		std::vector<PackageReference> GetDependencies()
		{
			if (!HasDependencies())
				throw std::runtime_error("No dependencies.");

			auto& values = GetValue(Property_Dependencies).AsList();
			auto result = std::vector<PackageReference>();
			for (auto& value : values)
			{
				result.push_back(PackageReference::Parse(value.AsString()));
			}

			return result;
		}

		void SetDependencies(const std::vector<PackageReference>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value.ToString()));
			}

			EnsureValue(Property_Dependencies).SetValueList(std::move(stringValues));
		}

		/// <summary>
		/// Gets or sets the list of dev dependency packages
		/// TODO: Observable?
		/// </summary>
		bool HasDevDependencies()
		{
			return HasValue(Property_DevDependencies);
		}

		std::vector<PackageReference> GetDevDependencies()
		{
			if (!HasDevDependencies())
				throw std::runtime_error("No dev dependencies.");

			auto& values = GetValue(Property_DevDependencies).AsList();
			auto result = std::vector<PackageReference>();
			for (auto& value : values)
			{
				result.push_back(PackageReference::Parse(value.AsString()));
			}

			return result;
		}

		void SetDevDependencies(const std::vector<PackageReference>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value.ToString()));
			}

			EnsureValue(Property_DevDependencies).SetValueList(std::move(stringValues));
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
		bool HasValue(std::string_view key)
		{
			return _table.contains(key.data());
		}

		RecipeValue& GetValue(std::string_view key)
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

		RecipeValue& EnsureValue(std::string_view key)
		{
			auto findItr = _table.find(key.data());
			if (findItr != _table.end())
			{
				return findItr->second;
			}
			else
			{
				auto insertResult = _table.emplace(key.data(), RecipeValue());
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
