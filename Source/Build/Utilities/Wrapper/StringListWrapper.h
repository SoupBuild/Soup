// <copyright file="StringListWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Utilities
{
	/// <summary>
	/// Build State property list implementation, string specialization
	/// </summary>
	export class StringListWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the StringListWrapper class
		/// </summary>
		StringListWrapper(IList<const char*>& value) :
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
			auto result = _value.TryResize(size);
			if (result != ApiCallResult::Success)
				throw std::runtime_error("Resize Failed");
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		std::string_view GetValueAt(uint64_t index) const
		{
			const char* result = nullptr;
			auto status = _value.TryGetValueAt(index, result);
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryGetValueAt Failed");

			return result;
		}

		/// <summary>
		/// Property setter methods.
		/// </summary>
		void SetValueAt(uint64_t index, std::string_view value)
		{
			auto status = _value.TrySetValueAt(index, value.data());
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TrySetValueAt Failed");
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
				result[i] = GetValueAt(i);
			}

			return result;
		}

		std::vector<Path> CopyAsPathVector() const
		{
			auto size = static_cast<size_t>(GetSize());
			auto result = std::vector<Path>(size);
			for (size_t i = 0; i < size; i++)
			{
				result[i] = Path(GetValueAt(i));
			}

			return result;
		}

		void SetAll(const std::vector<std::string>& values)
		{
			Resize(static_cast<uint64_t>(values.size()));
			for (size_t i = 0; i < values.size(); i++)
			{
				SetValueAt(i, values[i]);
			}
		}

		void SetAll(const std::vector<Path>& values)
		{
			Resize(static_cast<uint64_t>(values.size()));
			for (size_t i = 0; i < values.size(); i++)
			{
				SetValueAt(i, values[i].ToString());
			}
		}

		void Append(const std::vector<std::string>& values)
		{
			auto currentSize = static_cast<size_t>(GetSize());
			auto finalSize = currentSize + values.size();
			Resize(static_cast<uint64_t>(finalSize));
			for (size_t i = currentSize; i < finalSize; i++)
			{
				SetValueAt(i, values[i - currentSize]);
			}
		}

		void Append(const std::vector<Path>& values)
		{
			auto currentSize = static_cast<size_t>(GetSize());
			auto finalSize = currentSize + values.size();
			Resize(static_cast<uint64_t>(finalSize));
			for (size_t i = currentSize; i < finalSize; i++)
			{
				SetValueAt(i, values[i - currentSize].ToString());
			}
		}

	private:
		IList<const char*>& _value;
	};
}
