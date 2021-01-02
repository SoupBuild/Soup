// <copyright file="ValuePrimitive.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	template<typename T>
	class ValuePrimitive : public IValuePrimitive<T>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ValuePrimitive class
		/// </summary>
		ValuePrimitive() :
			_value()
		{
		}

		/// <summary>
		/// Initializes a new instance of the ValuePrimitive class
		/// </summary>
		ValuePrimitive(T value) :
			_value(value)
		{
		}

		/// <summary>
		/// Type checker methods
		/// </summary>
		T GetValue() const noexcept override final
		{
			return _value;
		}

		ApiCallResult TrySetValue(T value) noexcept override final
		{
			_value = value;
			return ApiCallResult::Success;
		}

		/// <summary>
		/// Internal Accessor
		/// </summary>
		std::string ToString() const
		{
			return std::to_string(_value);
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const ValuePrimitive<T>& rhs) const
		{
			return _value == rhs._value;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const ValuePrimitive<T>& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		T _value;
	};

	template<>
	class ValuePrimitive<const char*> : public IValuePrimitive<const char*>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ValuePrimitive class
		/// </summary>
		ValuePrimitive() :
			_value()
		{
		}
		
		/// <summary>
		/// Initializes a new instance of the ValuePrimitive class
		/// </summary>
		ValuePrimitive(std::string value) :
			_value(std::move(value))
		{
		}

		/// <summary>
		/// Type checker methods
		/// </summary>
		const char* GetValue() const noexcept override final
		{
			return _value.c_str();
		}

		ApiCallResult TrySetValue(const char* value) noexcept override final
		{
			try
			{
				// String assign can fail
				_value = value;
				return ApiCallResult::Success;
			}
			catch(...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		/// <summary>
		/// Internal Accessor
		/// </summary>
		const std::string& ToString() const noexcept
		{
			return _value;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const ValuePrimitive<const char*>& rhs) const
		{
			return _value == rhs._value;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const ValuePrimitive<const char*>& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::string _value;
	};
}
