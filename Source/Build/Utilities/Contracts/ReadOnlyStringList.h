// <copyright file="StringList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Utilities
{
	export class ReadOnlyStringList : public IReadOnlyList<const char*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ReadOnlyStringList class
		/// </summary>
		ReadOnlyStringList() :
			_values()
		{
		}

		/// <summary>
		/// Initializes a new instance of the ReadOnlyStringList class
		/// </summary>
		ReadOnlyStringList(std::vector<std::string> values) :
			_values(std::move(values))
		{
		}

		/// <summary>
		/// Initializes a new instance of the ReadOnlyStringList class
		/// </summary>
		ReadOnlyStringList(const std::vector<Path>& values) :
			_values()
		{
			_values.reserve(values.size());
			for (auto& value : values)
				_values.push_back(value.ToString());
		}

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept override final
		{
			return _values.size();
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		ApiCallResult TryGetValueAt(
			uint64_t index,
			const char*& result) const noexcept override final
		{
			try
			{
				result = _values.at(index).c_str();
				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		/// <summary>
		/// Internal accessor
		/// </summary>
		const std::vector<std::string>& GetValues() const noexcept
		{
			return _values;
		}

		std::vector<std::string>& GetValues() noexcept
		{
			return _values;
		}

	private:
		std::vector<std::string> _values;
	};
}
