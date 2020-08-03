// <copyright file="StringList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Utilities
{
	export class StringList : public IList<const char*>, public IReadOnlyList<const char*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the StringList class
		/// </summary>
		StringList() :
			_values()
		{
		}

		/// <summary>
		/// Initializes a new instance of the StringList class
		/// </summary>
		StringList(std::vector<std::string> values) :
			_values(std::move(values))
		{
		}

		/// <summary>
		/// Initializes a new instance of the StringList class
		/// </summary>
		StringList(const std::vector<Path>& values) :
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

		ApiCallResult TryResize(uint64_t size) noexcept override final
		{
			try
			{
				_values.resize(size);
				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
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
		/// Property setter methods.
		/// </summary>
		ApiCallResult TrySetValueAt(uint64_t index, const char* value) noexcept override final
		{
			try
			{
				_values.at(index) = value;
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
