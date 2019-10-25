// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageReference.h"
#include "RecipeType.h"
#include "RecipeLanguageVersion.h"
#include "SemanticVersion.h"

namespace Soup
{
	/// <summary>
	/// The recipe container
	/// </summary>
	export class Recipe
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe() :
			_isDirty(false),
			_name(),
			_version(),
			_type(std::nullopt),
			_languageVersion(std::nullopt),
			_dependencies(std::nullopt),
			_public(std::nullopt),
			_source(std::nullopt),
			_includePaths(std::nullopt)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(
			std::string name,
			SemanticVersion version) :
			_isDirty(false),
			_name(std::move(name)),
			_version(version),
			_type(std::nullopt),
			_languageVersion(std::nullopt),
			_dependencies(std::nullopt),
			_public(std::nullopt),
			_source(std::nullopt),
			_includePaths(std::nullopt)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		Recipe(
			std::string name,
			SemanticVersion version,
			std::optional<RecipeType> type,
			std::optional<RecipeLanguageVersion> languageVersion,
			std::optional<std::vector<PackageReference>> dependencies,
			std::optional<std::string> publicFile,
			std::optional<std::vector<std::string>> source,
			std::optional<std::vector<std::string>> includePaths) :
			_isDirty(false),
			_name(std::move(name)),
			_version(version),
			_type(std::move(type)),
			_languageVersion(std::move(languageVersion)),
			_dependencies(std::move(dependencies)),
			_public(std::move(publicFile)),
			_source(std::move(source)),
			_includePaths(std::move(includePaths))
		{
		}

		/// <summary>
		/// Gets a value indicating whether the content has changed or not
		/// </summary>
		bool IsDirty() const
		{
			return _isDirty;
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		const std::string& GetName() const
		{
			return _name;
		}

		void SetName(std::string value)
		{
			if (value != _name)
			{
				_name = std::move(value);
				_isDirty = true;
			}
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		SemanticVersion GetVersion() const
		{
			return _version;
		}

		void SetVersion(SemanticVersion value)
		{
			if (value != _version)
			{
				_version = value;
				_isDirty = true;
			}
		}

		/// <summary>
		/// Gets or sets the package type
		/// </summary>
		bool HasType() const
		{
			return _type.has_value();
		}

		RecipeType GetType() const
		{
			if (HasType())
			{
				return _type.value();
			}
			else
			{
				return DefaultRecipeType;
			}
		}

		void SetType(RecipeType value)
		{
			if (!HasType() || _type.value() != value)
			{
				_type = value;
				_isDirty = true;
			}
		}

		/// <summary>
		/// Gets or sets the language version
		/// </summary>
		bool HasLanguageVersion() const
		{
			return _languageVersion.has_value();
		}

		RecipeLanguageVersion GetLanguageVersion() const
		{
			if (HasLanguageVersion())
			{
				return _languageVersion.value();
			}
			else
			{
				return DefaultRecipeLanguageVersion;
			}
		}

		/// <summary>
		/// Gets or sets the list of dependency packages
		/// TODO: Observable?
		/// </summary>
		bool HasDependencies() const
		{
			return _dependencies.has_value();
		}

		const std::vector<PackageReference>& GetDependencies() const
		{
			if (!HasDependencies())
				throw std::runtime_error("No dependencies.");
			return _dependencies.value();
		}

		void SetDependencies(const std::vector<PackageReference>& value)
		{
			if (!HasDependencies() || _dependencies.value() != value)
			{
				_dependencies = value;
				_isDirty = true;
			}
		}

		/// <summary>
		/// Gets or sets the public file
		/// </summary>
		bool HasPublic() const
		{
			return _public.has_value();
		}

		const std::string& GetPublic() const
		{
			if (!HasPublic())
				throw std::runtime_error("No public.");
			return _public.value();
		}

		Path GetPublicAsPath() const
		{
			if (!HasPublic())
				throw std::runtime_error("No public.");

			return Path(_public.value());
		}

		void SetPublic(const std::string& value)
		{
			if (!HasPublic() || _public.value() != value)
			{
				_public = value;
				_isDirty = true;
			}
		}

		/// <summary>
		/// Gets or sets the source values
		/// TODO: Observable?
		/// </summary>
		bool HasSource() const
		{
			return _source.has_value();
		}

		const std::vector<std::string>& GetSource() const
		{
			if (!HasSource())
				throw std::runtime_error("No source.");
			return _source.value();
		}

		std::vector<Path> GetSourceAsPath() const
		{
			std::vector<Path> result;
			if (HasSource())
			{
				result.reserve(_source.value().size());
				for (auto& value : _source.value())
					result.push_back(Path(value));
			}

			return result;
		}

		void SetSource(const std::vector<std::string>& value)
		{
			if (!HasSource() || _source.value() != value)
			{
				_source = value;
				_isDirty = true;
			}
		}

		/// <summary>
		/// Gets or sets the include paths values
		/// TODO: Observable?
		/// </summary>
		bool HasIncludePaths() const
		{
			return _includePaths.has_value();
		}

		const std::vector<std::string>& GetIncludePaths() const
		{
			if (!HasIncludePaths())
				throw std::runtime_error("No includePaths.");
			return _includePaths.value();
		}

		std::vector<Path> GetIncludePathsAsPath() const
		{
			if (!HasIncludePaths())
				throw std::runtime_error("No includePaths.");

			std::vector<Path> result;
			result.reserve(_includePaths.value().size());
			for (auto& value : _includePaths.value())
				result.push_back(Path(value));

			return result;
		}

		void SetIncludePaths(const std::vector<std::string>& value)
		{
			if (!HasIncludePaths() || _includePaths.value() != value)
			{
				_includePaths = value;
				_isDirty = true;
			}
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const Recipe& rhs) const
		{
			return _name == rhs._name &&
				_version == rhs._version &&
				_type == rhs._type &&
				_languageVersion == rhs._languageVersion &&
				_dependencies == rhs._dependencies &&
				_public == rhs._public &&
				_source == rhs._source &&
				_includePaths == rhs._includePaths;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const Recipe& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		static const RecipeType DefaultRecipeType = RecipeType::StaticLibrary;
		static const RecipeLanguageVersion DefaultRecipeLanguageVersion = RecipeLanguageVersion::CPP20;
		bool _isDirty;
		std::string _name;
		SemanticVersion _version;
		std::optional<RecipeType> _type;
		std::optional<RecipeLanguageVersion> _languageVersion;
		std::optional<std::vector<PackageReference>> _dependencies;
		std::optional<std::string> _public;
		std::optional<std::vector<std::string>> _source;
		std::optional<std::vector<std::string>> _includePaths;
	};
}
