// <copyright file="PackageName.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// A package name that uniquely identifies a package in a single build graph
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class PackageName
	{
	public:
		/// <summary>
		/// Try parse a package name from the provided string
		/// </summary>
		static bool TryParse(const std::string& value, PackageName& result)
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

			result = PackageName();
			return false;
		}

		/// <summary>
		/// Parse a package name from the provided string.
		/// </summary>
		static PackageName Parse(const std::string& value)
		{
			// Reuse regex between runs
			static auto nameRegex = std::regex(R"(^(?:([A-Za-z][\w.]*)\|)?([A-Za-z][\w.]*)$)");

			// Attempt to parse Named package
			auto nameMatch = std::smatch();
			if (std::regex_match(value, nameMatch, nameRegex))
			{
				// The package is a valid name
				std::optional<std::string> owner = std::nullopt;
				auto ownerMatch = nameMatch[1];
				if (ownerMatch.matched)
				{
					owner = ownerMatch.str();
				}

				auto name = nameMatch[2].str();

				return PackageName(std::move(owner), std::move(name));
			}
			else
			{
				throw std::runtime_error("Invalid package name");
			}
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PackageName"/> class.
		/// </summary>
		PackageName() :
			_owner(std::nullopt),
			_name()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageName"/> class.
		/// </summary>
		PackageName(
			std::optional<std::string> owner,
			std::string name) :
			_owner(std::move(owner)),
			_name(std::move(name))
		{
		}

		/// <summary>
		/// Gets or sets a value indicating if there is an owner.
		/// </summary>
		bool HasOwner() const
		{
			return _owner.has_value();
		}

		/// <summary>
		/// Gets or sets the Owner.
		/// </summary>
		const std::string& GetOwner() const
		{
			if (!_owner.has_value())
				throw std::runtime_error("Package name does not have an owner.");
			return _owner.value();
		}

		/// <summary>
		/// Gets or sets the Name.
		/// </summary>
		const std::string& GetName() const
		{
			return _name;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const PackageName& rhs) const
		{
			return _owner == rhs._owner &&
				_name == rhs._name;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const PackageName& rhs) const
		{
			return _owner != rhs._owner ||
				_name != rhs._name;
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		std::string ToString() const
		{
			// Build up the owner/name name
			std::stringstream stringBuilder;

			if (_owner.has_value())
			{
				stringBuilder << _owner.value() << '|';
			}

			stringBuilder << _name;

			return stringBuilder.str();
		}

	private:
		std::optional<std::string> _owner;
		std::string _name;
	};
}
