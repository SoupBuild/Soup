// <copyright file="ValueListWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ValueWrapper.h"

namespace Soup::Build::Extensions
{
	/// <summary>
	/// Build State property list implementation, string specialization
	/// </summary>
	export class ValueListWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ValueListWrapper class
		/// </summary>
		ValueListWrapper(IValueList& value) :
			_value(value)
		{
		}

		/// <summary>
		/// Size access methods
		/// </summary>
		uint64_t GetSize() const noexcept
		{
			return _value.GetSize();
		}

		void Resize(uint64_t size)
		{
			auto status = _value.TryResize(size);
			if (status != ApiCallResult::Success)
				throw std::runtime_error("Resize Failed");
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		ValueWrapper GetValueAt(uint64_t index) const
		{
			IValue* result = nullptr;
			auto status = _value.TryGetValueAt(index, result);
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryGetValueAt Failed");
			if (result == nullptr)
				throw std::runtime_error("TryGetValueAt result was null.");

			return ValueWrapper(*result);
		}

		/// <summary>
		/// Extended helpers for easy updating of entire contents
		/// </summary>
		std::vector<std::string> CopyAsStringVector() const
		{
			auto size = static_cast<size_t>(GetSize());
			auto result = std::vector<std::string>(size);
			for (size_t i = 0; i < size; i++)
			{
				auto value = GetValueAt(i);
				result[i] = value.AsString().GetValue();
			}

			return result;
		}

		std::vector<Path> CopyAsPathVector() const
		{
			auto size = static_cast<size_t>(GetSize());
			auto result = std::vector<Path>(size);
			for (size_t i = 0; i < size; i++)
			{
				auto value = GetValueAt(i);
				result[i] = Path(value.AsString().GetValue());
			}

			return result;
		}

		void SetAll(const std::vector<std::string>& values)
		{
			Resize(static_cast<uint64_t>(values.size()));
			for (size_t i = 0; i < values.size(); i++)
			{
				GetValueAt(i).SetValueString(values[i]);
			}
		}

		void SetAll(const std::vector<Path>& values)
		{
			Resize(static_cast<uint64_t>(values.size()));
			for (size_t i = 0; i < values.size(); i++)
			{
				GetValueAt(i).SetValueString(values[i].ToString());
			}
		}

		void SetAll(ValueListWrapper values)
		{
			auto finalSize = values.GetSize();
			Resize(finalSize);
			for (size_t i = 0; i < finalSize; i++)
			{
				auto& currentValue = values.GetValueAt(i);
				GetValueAt(i).SetValue(currentValue);
			}
		}

		void Append(const std::vector<std::string>& values)
		{
			auto currentSize = static_cast<size_t>(GetSize());
			auto finalSize = currentSize + values.size();
			Resize(static_cast<uint64_t>(finalSize));
			for (size_t i = currentSize; i < finalSize; i++)
			{
				auto& currentValue = values[i - currentSize];
				GetValueAt(i).SetValueString(currentValue);
			}
		}

		void Append(const std::vector<Path>& values)
		{
			auto currentSize = static_cast<size_t>(GetSize());
			auto finalSize = currentSize + values.size();
			Resize(static_cast<uint64_t>(finalSize));
			for (size_t i = currentSize; i < finalSize; i++)
			{
				auto& currentValue = values[i - currentSize];
				GetValueAt(i).SetValueString(currentValue.ToString());
			}
		}

		void Append(ValueListWrapper values)
		{
			// Append the intput list to the target
			auto currentSize = GetSize();
			auto finalSize = currentSize + values.GetSize();
			Resize(finalSize);
			for (uint64_t i = currentSize; i < finalSize; i++)
			{
				auto& currentValue = values.GetValueAt(i - currentSize);
				GetValueAt(i).SetValue(currentValue);
			}
		}

	private:
		IValueList& _value;
	};
}
