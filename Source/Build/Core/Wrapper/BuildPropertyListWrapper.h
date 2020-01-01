// <copyright file="BuildPropertyListWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build State property list implementation
	/// </summary>
	template<typename T>
	class BuildPropertyListWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyListWrapper class
		/// </summary>
		BuildPropertyListWrapper(IBuildPropertyList<T>& value) :
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
			auto status = _value.Resize(size);
			if (status != 0)
				throw std::runtime_error("Resize Failed");
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		T GetValueAt(uint64_t index) const
		{
			T result;
			auto status = _value.TryGetValueAt(index, result);
			if (status != 0)
				throw std::runtime_error("TryGetValueAt Failed");

			return result;
		}

		/// <summary>
		/// Property setter methods.
		/// </summary>
		void SetValueAt(uint64_t index, T value)
		{
			auto status = _value.TryGetValueAt(index, value);
			if (status != 0)
				throw std::runtime_error("TrySetValueAt Failed");
		}

	private:
		IBuildPropertyList<T>& _value;
	};

	/// <summary>
	/// Build State property list implementation, string specialization
	/// </summary>
	template<>
	class BuildPropertyListWrapper<const char*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyListWrapper class
		/// </summary>
		BuildPropertyListWrapper(IBuildPropertyList<const char*>& value) :
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
			auto status = _value.Resize(size);
			if (status != 0)
				throw std::runtime_error("Resize Failed");
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		std::string_view GetValueAt(uint64_t index) const
		{
			const char* result = nullptr;
			auto status = _value.TryGetValueAt(index, result);
			if (status != 0)
				throw std::runtime_error("TryGetValueAt Failed");

			return result;
		}

		/// <summary>
		/// Property setter methods.
		/// </summary>
		void SetValueAt(uint64_t index, std::string_view value)
		{
			auto status = _value.TrySetValueAt(index, value.data());
			if (status != 0)
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

	private:
		IBuildPropertyList<const char*>& _value;
	};
}
