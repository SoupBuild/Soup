// <copyright file="ValueList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build list implementation for simple objects
	/// </summary>
	class ValueList : public IValueList
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

		OperationResult Resize(uint64_t size) noexcept override final
		{
			try
			{
				_values.resize(size);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		OperationResult TryGetValueAt(uint64_t index, IValue*& result) noexcept override final
		{
			try
			{
				result = &_values.at(index);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Internal Accessor
		/// </summary>
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

	private:
		std::vector<Value> _values;
	};
}
