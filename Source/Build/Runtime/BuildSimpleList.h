// <copyright file="BuildSimpleList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime
{
	/// <summary>
	/// Build list implementation for simple objects
	/// </summary>
	template<typename T>
	class BuildSimpleList : public IList<T>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildSimpleList class
		/// </summary>
		BuildSimpleList() :
			_value()
		{
		}

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept override final
		{
			return _value.size();
		}

		ApiCallResult TryResize(uint64_t size) noexcept override final
		{
			try
			{
				_value.resize(size);
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
		ApiCallResult TryGetValueAt(uint64_t index, T& result) noexcept override final
		{
			try
			{
				result = _value.at(index);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Property setter methods.
		/// </summary>
		ApiCallResult TrySetValueAt(uint64_t index, T value) noexcept override final
		{
			try
			{
				_value.at(index) = value;
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		std::vector<T> _value;
	};
}
