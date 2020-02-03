// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageReference.h"
#include "RecipeType.h"
#include "RecipeLanguageVersion.h"

namespace Soup::Build
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
			std::optional<RecipeType> type,
			std::optional<RecipeLanguageVersion> languageVersion,
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
		Recipe(ValueTable table) :
			_table(std::move(table))
		{
			if (!ValueTableWrapper(_table).HasValue(Property_Name))
				throw std::runtime_error("Missing required property Name");
			if (!ValueTableWrapper(_table).HasValue(Property_Version))
				throw std::runtime_error("Missing required property Version");
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		std::string_view GetName()
		{
			return ValueTableWrapper(_table).GetValue(Property_Name).AsString().GetValue();
		}

		void SetName(std::string_view value)
		{
			ValueTableWrapper(_table).EnsureValue(Property_Name).SetValueString(value);
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		SemanticVersion GetVersion()
		{
			return SemanticVersion::Parse(
				ValueTableWrapper(_table).GetValue(Property_Version).AsString().GetValue());
		}

		void SetVersion(SemanticVersion value)
		{
			return ValueTableWrapper(_table).EnsureValue(Property_Version).SetValueString(value.ToString());
		}

		/// <summary>
		/// Gets or sets the package type
		/// </summary>
		bool HasType()
		{
			return ValueTableWrapper(_table).HasValue(Property_Type);
		}

		RecipeType GetType()
		{
			if (!HasType())
				throw std::runtime_error("No type.");

			return ParseRecipeType(ValueTableWrapper(_table).GetValue(Property_Type).AsString().GetValue());
		}

		void SetType(RecipeType value)
		{
			return ValueTableWrapper(_table).EnsureValue(Property_Type).SetValueString(ToString(value));
		}

		/// <summary>
		/// Gets or sets the language version
		/// </summary>
		bool HasLanguageVersion()
		{
			return ValueTableWrapper(_table).HasValue(Property_Language);
		}

		RecipeLanguageVersion GetLanguageVersion()
		{
			if (!HasLanguageVersion())
				throw std::runtime_error("No language version.");

			return ParseRecipeLanguageVersion(
				ValueTableWrapper(_table).GetValue(Property_Language).AsString().GetValue());
		}

		void SetLanguageVersion(RecipeLanguageVersion value)
		{
			return ValueTableWrapper(_table).EnsureValue(Property_Language).SetValueString(ToString(value));
		}

		/// <summary>
		/// Gets or sets the list of dependency packages
		/// TODO: Observable?
		/// </summary>
		bool HasDependencies()
		{
			return ValueTableWrapper(_table).HasValue(Property_Dependencies);
		}

		std::vector<PackageReference> GetDependencies()
		{
			if (!HasDependencies())
				throw std::runtime_error("No dependencies.");

			auto values = ValueTableWrapper(_table).GetValue(Property_Dependencies).AsList().CopyAsStringVector();
			auto result = std::vector<PackageReference>();
			for (auto& value : values)
			{
				result.push_back(PackageReference::Parse(value));
			}

			return result;
		}

		void SetDependencies(const std::vector<PackageReference>& values)
		{
			auto stringValues = std::vector<std::string>();
			for (auto& value : values)
			{
				stringValues.push_back(value.ToString());
			}

			ValueTableWrapper(_table).EnsureValue(Property_Dependencies).EnsureList().SetAll(stringValues);
		}

		/// <summary>
		/// Gets or sets the list of dextension packages
		/// TODO: Observable?
		/// </summary>
		bool HasExtensions()
		{
			return ValueTableWrapper(_table).HasValue(Property_Extensions);
		}

		std::vector<PackageReference> GetExtensions()
		{
			if (!HasExtensions())
				throw std::runtime_error("No extensions.");

			auto values = ValueTableWrapper(_table).GetValue(Property_Extensions).AsList().CopyAsStringVector();
			auto result = std::vector<PackageReference>();
			for (auto& value : values)
			{
				result.push_back(PackageReference::Parse(value));
			}

			return result;
		}

		void SetExtensions(const std::vector<PackageReference>& values)
		{
			auto stringValues = std::vector<std::string>();
			for (auto& value : values)
			{
				stringValues.push_back(value.ToString());
			}

			ValueTableWrapper(_table).EnsureValue(Property_Extensions).EnsureList().SetAll(stringValues);
		}

		/// <summary>
		/// Gets or sets the public file
		/// </summary>
		bool HasPublic()
		{
			return ValueTableWrapper(_table).HasValue(Property_Public);
		}

		std::string_view GetPublic()
		{
			if (!HasPublic())
				throw std::runtime_error("No public.");

			return ValueTableWrapper(_table).GetValue(Property_Public).AsString().GetValue();
		}

		void SetPublic(std::string_view value)
		{
			ValueTableWrapper(_table).EnsureValue(Property_Public).SetValueString(value);
		}

		/// <summary>
		/// Gets or sets the source values
		/// TODO: Observable?
		/// </summary>
		bool HasSource()
		{
			return ValueTableWrapper(_table).HasValue(Property_Source);
		}

		std::vector<std::string> GetSource()
		{
			if (!HasSource())
				throw std::runtime_error("No source.");

			return ValueTableWrapper(_table).GetValue(Property_Source).AsList().CopyAsStringVector();
		}

		void SetSource(const std::vector<std::string>& values)
		{
			ValueTableWrapper(_table).EnsureValue(Property_Source).EnsureList().SetAll(values);
		}

		/// <summary>
		/// Gets or sets the include paths values
		/// </summary>
		bool HasIncludePaths()
		{
			return ValueTableWrapper(_table).HasValue(Property_IncludePaths);
		}

		std::vector<std::string> GetIncludePaths()
		{
			if (!HasIncludePaths())
				throw std::runtime_error("No includePaths.");

			return ValueTableWrapper(_table).GetValue(Property_IncludePaths).AsList().CopyAsStringVector();
		}

		void SetIncludePaths(const std::vector<std::string>& values)
		{
			ValueTableWrapper(_table).EnsureValue(Property_IncludePaths).EnsureList().SetAll(values);
		}

		/// <summary>
		/// Gets or sets the define values
		/// </summary>
		bool HasDefines()
		{
			return ValueTableWrapper(_table).HasValue(Property_Defines);
		}

		std::vector<std::string> GetDefines()
		{
			if (!HasDefines())
				throw std::runtime_error("No defines.");

			return ValueTableWrapper(_table).GetValue(Property_Defines).AsList().CopyAsStringVector();
		}

		void SetDefines(const std::vector<std::string>& values)
		{
			ValueTableWrapper(_table).EnsureValue(Property_Defines).EnsureList().SetAll(values);
		}

		/// <summary>
		/// Raw access
		/// </summary>
		ValueTable& GetTable()
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
		ValueTable _table;
	};
}
