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
		static constexpr const char* Property_Name = "Name";
		static constexpr const char* Property_Version = "Version";
		static constexpr const char* Property_Type = "Type";
		static constexpr const char* Property_Language = "Language";
		static constexpr const char* Property_Dependencies = "Dependencies";
		static constexpr const char* Property_Extensions = "Extensions";
		static constexpr const char* Property_Public = "Public";
		static constexpr const char* Property_Source = "Source";
		static constexpr const char* Property_IncludePaths = "IncludePaths";
		static constexpr const char* Property_Defines = "Defines";

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
			SemanticVersion version) :
			_table()
		{
			SetName(name);
			SetVersion(version);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(
			std::string_view name,
			SemanticVersion version,
			std::optional<Build::Extensions::RecipeType> type,
			std::optional<Build::Extensions::RecipeLanguageVersion> languageVersion,
			std::optional<std::vector<PackageReference>> dependencies,
			std::optional<std::vector<PackageReference>> extensions,
			std::optional<std::string> publicFile,
			std::optional<std::vector<std::string>> source,
			std::optional<std::vector<std::string>> includePaths,
			std::optional<std::vector<std::string>> defines) :
			_table()
		{
			SetName(name);
			SetVersion(version);

			if (type.has_value())
				SetType(type.value());

			if (languageVersion.has_value())
				SetLanguageVersion(languageVersion.value());

			if (dependencies.has_value())
				SetDependencies(dependencies.value());

			if (extensions.has_value())
				SetExtensions(extensions.value());

			if (publicFile.has_value())
				SetPublic(publicFile.value());

			if (source.has_value())
				SetSource(source.value());

			if (includePaths.has_value())
				SetIncludePaths(includePaths.value());

			if (defines.has_value())
				SetDefines(defines.value());
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(RecipeTable table) :
			_table(std::move(table))
		{
			if (!HasValue(Property_Name))
				throw std::runtime_error("Missing required property Name");
			if (!HasValue(Property_Version))
				throw std::runtime_error("Missing required property Version");
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
		/// Gets or sets the package version
		/// </summary>
		SemanticVersion GetVersion()
		{
			return SemanticVersion::Parse(
				GetValue(Property_Version).AsString());
		}

		void SetVersion(SemanticVersion value)
		{
			return EnsureValue(Property_Version).SetValueString(value.ToString());
		}

		/// <summary>
		/// Gets or sets the package type
		/// </summary>
		bool HasType()
		{
			return HasValue(Property_Type);
		}

		Build::Extensions::RecipeType GetType()
		{
			if (!HasType())
				throw std::runtime_error("No type.");

			return Build::Extensions::ParseRecipeType(GetValue(Property_Type).AsString());
		}

		void SetType(Build::Extensions::RecipeType value)
		{
			return EnsureValue(Property_Type).SetValueString(ToString(value));
		}

		/// <summary>
		/// Gets or sets the language version
		/// </summary>
		bool HasLanguageVersion()
		{
			return HasValue(Property_Language);
		}

		Build::Extensions::RecipeLanguageVersion GetLanguageVersion()
		{
			if (!HasLanguageVersion())
				throw std::runtime_error("No language version.");

			return Build::Extensions::ParseRecipeLanguageVersion(
				GetValue(Property_Language).AsString());
		}

		void SetLanguageVersion(Build::Extensions::RecipeLanguageVersion value)
		{
			return EnsureValue(Property_Language).SetValueString(ToString(value));
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
		/// Gets or sets the list of dextension packages
		/// TODO: Observable?
		/// </summary>
		bool HasExtensions()
		{
			return HasValue(Property_Extensions);
		}

		std::vector<PackageReference> GetExtensions()
		{
			if (!HasExtensions())
				throw std::runtime_error("No extensions.");

			auto& values = GetValue(Property_Extensions).AsList();
			auto result = std::vector<PackageReference>();
			for (auto& value : values)
			{
				result.push_back(PackageReference::Parse(value.AsString()));
			}

			return result;
		}

		void SetExtensions(const std::vector<PackageReference>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value.ToString()));
			}

			EnsureValue(Property_Extensions).SetValueList(std::move(stringValues));
		}

		/// <summary>
		/// Gets or sets the public file
		/// </summary>
		bool HasPublic()
		{
			return HasValue(Property_Public);
		}

		const std::string& GetPublic()
		{
			if (!HasPublic())
				throw std::runtime_error("No public.");

			return GetValue(Property_Public).AsString();
		}

		void SetPublic(std::string_view value)
		{
			EnsureValue(Property_Public).SetValueString(std::string(value));
		}

		/// <summary>
		/// Gets or sets the source values
		/// TODO: Observable?
		/// </summary>
		bool HasSource()
		{
			return HasValue(Property_Source);
		}

		std::vector<std::string> GetSource()
		{
			if (!HasSource())
				throw std::runtime_error("No source.");

			auto stringValues = std::vector<std::string>();
			for (auto& value : GetValue(Property_Source).AsList())
			{
				stringValues.push_back(value.AsString());
			}

			return stringValues;
		}

		void SetSource(const std::vector<std::string>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value));
			}

			EnsureValue(Property_Source).SetValueList(std::move(stringValues));
		}

		/// <summary>
		/// Gets or sets the include paths values
		/// </summary>
		bool HasIncludePaths()
		{
			return HasValue(Property_IncludePaths);
		}

		std::vector<std::string> GetIncludePaths()
		{
			if (!HasIncludePaths())
				throw std::runtime_error("No includePaths.");

			auto stringValues = std::vector<std::string>();
			for (auto& value : GetValue(Property_IncludePaths).AsList())
			{
				stringValues.push_back(value.AsString());
			}

			return stringValues;
		}

		void SetIncludePaths(const std::vector<std::string>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value));
			}

			EnsureValue(Property_IncludePaths).SetValueList(std::move(stringValues));
		}

		/// <summary>
		/// Gets or sets the define values
		/// </summary>
		bool HasDefines()
		{
			return HasValue(Property_Defines);
		}

		std::vector<std::string> GetDefines()
		{
			if (!HasDefines())
				throw std::runtime_error("No defines.");

			auto stringValues = std::vector<std::string>();
			for (auto& value : GetValue(Property_Defines).AsList())
			{
				stringValues.push_back(value.AsString());
			}

			return stringValues;
		}

		void SetDefines(const std::vector<std::string>& values)
		{
			auto stringValues = RecipeList();
			for (auto& value : values)
			{
				stringValues.push_back(RecipeValue(value));
			}

			EnsureValue(Property_Defines).SetValueList(std::move(stringValues));
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
