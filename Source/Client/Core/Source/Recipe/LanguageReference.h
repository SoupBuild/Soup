// <copyright file="LanguageReference.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// A language reference object which will consist of a name version pair that
	/// refers to a published package
	/// </summary>
	export class LanguageReference
	{
	public:
		/// <summary>
		/// Try parse a language reference from the provided string
		/// </summary>
		static bool TryParse(const std::string& value, LanguageReference& result)
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

			result = LanguageReference();
			return false;
		}

		/// <summary>
		/// Parse a language reference from the provided string.
		/// </summary>
		static LanguageReference Parse(const std::string& value)
		{
			// Reuse regex between runs
			static auto nameRegex = std::regex(R"(^([A-Za-z][\w#+.]*)(?:\|(\d+(?:.\d+)?(?:.\d+)?))?$)");

			// Attempt to parse Named reference
			auto nameMatch = std::smatch();
			if (std::regex_match(value, nameMatch, nameRegex))
			{
				auto name = nameMatch[1].str();

				auto versionMatch = nameMatch[2];
				auto version = SemanticVersion(0, 1, 0);
				if (versionMatch.matched)
				{
					version = SemanticVersion::Parse(versionMatch.str());
				}

				return LanguageReference(std::move(name), version);
			}
			else
			{
				throw std::runtime_error("Invalid language reference: " + value);
			}
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LanguageReference"/> class.
		/// </summary>
		LanguageReference() :
			_name(""),
			_version()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="LanguageReference"/> class.
		/// </summary>
		LanguageReference(
			std::string name,
			SemanticVersion version) :
			_name(std::move(name)),
			_version(version)
		{
		}

		/// <summary>
		/// Gets or sets the Name.
		/// </summary>
		const std::string& GetName() const
		{
			return _name;
		}

		/// <summary>
		/// Gets or sets the Version.
		/// </summary>
		const SemanticVersion& GetVersion() const
		{
			return _version;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const LanguageReference& rhs) const
		{
			return _name == rhs. _name &&
				_version == rhs. _version;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const LanguageReference& rhs) const
		{
			return _name != rhs. _name ||
				_version != rhs. _version;
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		std::string ToString() const
		{
			// Build up the name/version reference
			std::stringstream stringBuilder;

			stringBuilder << _name << '|' << _version.ToString();

			return stringBuilder.str();
		}

	private:
		std::string _name;
		SemanticVersion _version;
	};
}
