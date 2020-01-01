// <copyright file="BuildPropertyList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build State property list implementation
	/// </summary>
	template<typename T>
	class BuildPropertyList : public IBuildPropertyList<T>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyList class
		/// </summary>
		BuildPropertyList() :
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

		BuildSystemResult Resize(uint64_t size) noexcept override final
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
		BuildSystemResult TryGetValueAt(uint64_t index, T& result) const noexcept override final
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
		BuildSystemResult TrySetValueAt(uint64_t index, T value) noexcept override final
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

	template<>
	class BuildPropertyList<const char*> : public IBuildPropertyList<const char*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyList class
		/// </summary>
		BuildPropertyList() :
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

		BuildSystemResult Resize(uint64_t size) noexcept override final
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
		BuildSystemResult TryGetValueAt(uint64_t index, const char*& result) const noexcept override final
		{
			try
			{
				result = _value.at(index).c_str();
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
		BuildSystemResult TrySetValueAt(uint64_t index, const char* value) noexcept override final
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
		std::vector<std::string> _value;
	};
}
