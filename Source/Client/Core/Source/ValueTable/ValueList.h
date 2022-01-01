// <copyright file="ValueList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime
{
	/// <summary>
	/// Build list implementation for simple objects
	/// </summary>
	export class ValueList
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
		/// Initializes a new instance of the ValueList class
		/// </summary>
		ValueList(std::vector<Value> values) :
			_values(std::move(values))
		{
		}

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept
		{
			return _values.size();
		}

		void Resize(uint64_t size)
		{
			_values.resize(size);
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		Value& GetValueAt(uint64_t index)
		{
			return _values.at(index);
		}

		/// <summary>
		/// Internal access to the state
		/// </summary>
		std::vector<Value>& GetValues()
		{
			return _values;
		}

		const std::vector<Value>& GetValues() const
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
