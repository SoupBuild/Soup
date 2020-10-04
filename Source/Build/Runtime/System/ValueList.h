// <copyright file="ValueList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime
{
	/// <summary>
	/// Build list implementation for simple objects
	/// </summary>
	export class ValueList : public IValueList
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ValueList class
		/// </summary>
		ValueList() :
			_values()
		{
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
		ApiCallResult TryGetValueAt(uint64_t index, IValue*& result) noexcept override final
		{
			try
			{
				result = &_values.at(index);
				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		/// <summary>
		/// Internal access to the state
		/// </summary>
		std::vector<Value>& GetValues()
		{
			return _values;
		}

		std::string ToString()
		{
			auto stream = std::stringstream();

			stream << "[";
			bool isFirst = true;
			for (auto& value : _values)
			{
				if (!isFirst)
				{
					stream << ", ";
				}
				
				stream << "\"" << value.ToString() << "\"";
				isFirst = false;
			}

			stream << "]";

			return stream.str();
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const ValueList& rhs) const
		{
			return _values == rhs._values;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const ValueList& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::vector<Value> _values;
	};
}
