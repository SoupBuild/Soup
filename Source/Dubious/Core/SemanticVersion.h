// <copyright file="SemanticVersion.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Dubious
{
	/// <summary>
	/// The semantic version class
	/// </summary>
	export class SemanticVersion
	{
	public:
		/// <summary>
		/// Try parse the value
		/// </summary>
		static bool TryParse(const std::string& value, SemanticVersion& result)
		{
			// TODO: Invert this so TryParse is the default and Parse adds an exception
			// it is way faster in the fail case
			try
			{
				result = Parse(value);
				return true;
			}
			catch (...)
			{
			}

			result = SemanticVersion();
			return false;
		}

		/// <summary>
		/// Parse the value
		/// </summary>
		static SemanticVersion Parse(const std::string& value)
		{
			// Parse the integer values
			std::vector<int> values;
			auto stream = std::istringstream(value);
			std::string stringValue;
			while (std::getline(stream, stringValue, '.'))
			{
				auto intValue = std::stoi(stringValue);
				values.push_back(intValue);
			}

			if (values.size() != 3)
			{
				throw std::runtime_error("The version string must have three values.");
			}

			return SemanticVersion(
				values[0],
				values[1],
				values[2]);
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
		/// </summary>
		SemanticVersion() :
			_major(0),
			_minor(0),
			_patch(0)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
		/// </summary>
		SemanticVersion(short major, short minor, short patch) :
			_major(major),
			_minor(minor),
			_patch(patch)
		{
		}

		/// <summary>
		/// Gets or sets the version major
		/// </summary>
		int GetMajor() const
		{
			return _major;
		}

		/// <summary>
		/// Gets or sets the version minor
		/// </summary>
		int GetMinor() const
		{
			return _minor;
		}

		/// <summary>
		/// Gets or sets the version patch
		/// </summary>
		int GetPatch() const
		{
			return _patch;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const SemanticVersion& rhs) const
		{
			return _major == rhs. _major &&
				_minor == rhs. _minor &&
				_patch == rhs. _patch;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const SemanticVersion& rhs) const
		{
			return _major != rhs. _major ||
				_minor != rhs. _minor ||
				_patch != rhs. _patch;
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		std::string ToString() const
		{
			// "{Major}.{Minor}.{Patch}"
			std::stringstream stringBuilder;
			stringBuilder << _major << ".";
			stringBuilder << _minor << ".";
			stringBuilder << _patch;
			return stringBuilder.str();
		}

	private:
		int _major;
		int _minor;
		int _patch;
	};
}
