// <copyright file="PackageReference.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// A package reference object which will consist of a name version pair that
	/// refers to a published package or a path to a local recipe
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class PackageReference
	{
	public:
		/// <summary>
		/// Try parse a package reference from the provided string
		/// </summary>
		static bool TryParse(const std::string& value, PackageReference& result)
		{
			// TODO: Invert try parse to be the default and parse to add the exception
			// Way faster on the failed case and this could eat OOM
			try
			{
				result = Parse(value);
				return true;
			}
			catch (...)
			{
			}

			result = PackageReference();
			return false;
		}

		/// <summary>
		/// Parse a package reference from the provided string.
		/// </summary>
		static PackageReference Parse(const std::string& value)
		{
			// Reuse regex between runs
			static auto nameRegex = std::regex(R"(^(?:([\w#+]+)\|)?([A-Za-z][\w.]*)(?:@(\d+(?:.\d+)?(?:.\d+)?))?$)");

			// Attempt to parse Named reference
			auto nameMatch = std::smatch();
			if (std::regex_match(value, nameMatch, nameRegex))
			{
				// The package is a published reference
				std::optional<std::string> language = std::nullopt;
				auto languageMatch = nameMatch[1];
				if (languageMatch.matched)
				{
					language = languageMatch.str();
				}

				auto name = nameMatch[2].str();

				std::optional<SemanticVersion> version = std::nullopt;
				auto versionMatch = nameMatch[3];
				if (versionMatch.matched)
				{
					version = SemanticVersion::Parse(versionMatch.str());
				}

				return PackageReference(std::move(language), std::move(name), version);
			}
			else
			{
				// Assume that this package is a relative path reference
				return PackageReference(Path(value));
			}
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PackageReference"/> class.
		/// </summary>
		PackageReference() :
			_language(std::nullopt),
			_name(std::nullopt),
			_version(std::nullopt),
			_path(std::nullopt)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageReference"/> class.
		/// </summary>
		PackageReference(
			std::optional<std::string> language,
			std::string name,
			std::optional<SemanticVersion> version) :
			_language(std::move(language)),
			_name(std::move(name)),
			_version(version),
			_path(std::nullopt)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageReference"/> class.
		/// </summary>
		PackageReference(Path path) :
			_language(std::nullopt),
			_name(std::nullopt),
			_version(std::nullopt),
			_path(std::move(path))
		{
		}

		/// <summary>
		/// Gets a value indicating whether the reference is local or not
		/// </summary>
		bool IsLocal() const
		{
			return _path.has_value();
		}

		/// <summary>
		/// Gets or sets the Language.
		/// </summary>
		bool HasLanguage() const
		{
			return _language.has_value();
		}

		/// <summary>
		/// Gets or sets the Language.
		/// </summary>
		const std::string& GetLanguage() const
		{
			if (!_language.has_value())
				throw std::runtime_error("PackageReference does not have a Language value.");
			return _language.value();
		}

		/// <summary>
		/// Gets or sets the Name.
		/// </summary>
		const std::string& GetName() const
		{
			if (!_name.has_value())
				throw std::runtime_error("Cannot get the name of a local reference.");
			return _name.value();
		}

		/// <summary>
		/// Gets or sets the Version.
		/// </summary>
		const SemanticVersion& GetVersion() const
		{
			if (!_version.has_value())
				throw std::runtime_error("PackageReference does not have a Version value.");
			return _version.value();
		}

		/// <summary>
		/// Gets or sets the Path.
		/// </summary>
		const Path& GetPath() const
		{
			if (!_path.has_value())
				throw std::runtime_error("Cannot get the path of a non-local reference.");
			return _path.value();
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const PackageReference& rhs) const
		{
			return _language == rhs. _language &&
				_name == rhs. _name &&
				_version == rhs. _version &&
				_path == rhs. _path;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const PackageReference& rhs) const
		{
			return _language != rhs. _language ||
				_name != rhs. _name ||
				_version != rhs. _version ||
				_path != rhs. _path;
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		std::string ToString() const
		{
			// If the reference is a path then just return that
			if (IsLocal())
			{
				return _path.value().ToString();
			}
			else
			{
				// Build up the language/name/version reference
				std::stringstream stringBuilder;

				if (_language.has_value())
				{
					stringBuilder << _language.value() << '|';
				}

				stringBuilder << _name.value();

				if (_version.has_value())
				{
					stringBuilder << '@' << _version.value().ToString();
				}

				return stringBuilder.str();
			}
		}

	private:
		std::optional<std::string> _language;
		std::optional<std::string> _name;
		std::optional<SemanticVersion> _version;
		std::optional<Path> _path;
	};
}
