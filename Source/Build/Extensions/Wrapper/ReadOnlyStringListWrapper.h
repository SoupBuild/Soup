// <copyright file="ReadOnlyStringListWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Extensions
{
	/// <summary>
	/// Build State property list implementation, string specialization
	/// </summary>
	export class ReadOnlyStringListWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ReadOnlyStringListWrapper class
		/// </summary>
		ReadOnlyStringListWrapper(const IReadOnlyList<const char*>& value) :
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

	private:
		const IReadOnlyList<const char*>& _value;
	};
}
