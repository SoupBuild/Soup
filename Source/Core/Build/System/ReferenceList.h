// <copyright file="ReferenceList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build list implementation for simple objects
	/// </summary>
	template<typename TStorage, typename T>
	class ReferenceList : public IList<T*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ReferenceList class
		/// </summary>
		ReferenceList() :
			_values()
		{
		}

		/// <summary>
		/// Initializes a new instance of the ReferenceList class
		/// </summary>
		ReferenceList(std::vector<Memory::Reference<TStorage>> values) :
			_values(std::move(values))
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
		OperationResult TryGetValueAt(uint64_t index, T*& result) noexcept override final
		{
			try
			{
				result = _values.at(index).GetRaw();
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
		OperationResult TrySetValueAt(uint64_t index, T* value) noexcept override final
		{
			try
			{
				// TODO: Store interface type?
				auto internalValue = dynamic_cast<TStorage*>(value);
				if (internalValue == nullptr)
					return -2;

				_values.at(index) = internalValue;
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Internal access
		/// </summary>
		const std::vector<Memory::Reference<TStorage>>& GetValues() const
		{
			return _values;
		}

	private:
		std::vector<Memory::Reference<TStorage>> _values;
	};
}
