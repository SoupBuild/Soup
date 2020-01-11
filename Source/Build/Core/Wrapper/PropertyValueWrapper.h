// <copyright file="PropertyValueWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build State property value wrapper that makes the strict API easier to use
	/// </summary>
	class PropertyValueWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the PropertyValueWrapper  class
		/// </summary>
		PropertyValueWrapper(IPropertyValue& value) :
			_value(value)
		{
		}

		/// <summary>
		/// Type checker methods
		/// </summary>
		bool IsString() const noexcept
		{
			return _value.IsString();
		}

		bool IsInteger() const noexcept
		{
			return _value.IsInteger();
		}

		bool IsDouble() const noexcept
		{
			return _value.IsDouble();
		}

		bool IsBoolean() const noexcept
		{
			return _value.IsBoolean();
		}

		/// <summary>
		/// Returns true if the propery is an immutable system constant
		/// </summary>
		bool IsConstant() const noexcept
		{
			return _value.IsConstant();
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		std::string_view GetStringValue() const
		{
			const char* result = nullptr;
			auto status = _value.TryGetStringValue(result);
			if (status != 0)
				throw std::runtime_error("TryGetStringValue Failed");
			if (result == nullptr)
				throw std::runtime_error("TryGetStringValue has no value.");

			return result;
		}

		int64_t GetIntegerValue() const
		{
			int64_t result = 0;
			auto status = _value.TryGetIntegerValue(result);
			if (status != 0)
				throw std::runtime_error("TryGetIntegerValue Failed");

			return result;
		}

		double GetDoubleValue() const
		{
			double result = 0.0;
			auto status = _value.TryGetDoubleValue(result);
			if (status != 0)
				throw std::runtime_error("TryGetDoubleValue Failed");

			return result;
		}

		bool GetBooleanValue() const
		{
			bool result = 0;
			auto status = _value.TryGetBooleanValue(result);
			if (status != 0)
				throw std::runtime_error("TryGetBooleanValue Failed");

			return result;
		}

		/// <summary>
		/// Property setter methods.
		/// Note: Will perform a no-op if the value is immutable to prevent 
		/// exceptions accross dll boundaries.
		/// </summary>
		void SetStringValue(std::string_view value)
		{
			auto status = _value.TrySetStringValue(value.data());
			if (status != 0)
				throw std::runtime_error("TrySetValue String Failed");
		}

		void SetIntegerValue(int64_t value)
		{
			auto status = _value.TrySetIntegerValue(value);
			if (status != 0)
				throw std::runtime_error("TrySetValue Integer Failed");
		}

		void SetDoubleValue(double value)
		{
			auto status = _value.TrySetDoubleValue(value);
			if (status != 0)
				throw std::runtime_error("TrySetValue Double Failed");
		}

		void SetBooleanValue(bool value)
		{
			auto status = _value.TrySetBooleanValue(value);
			if (status != 0)
				throw std::runtime_error("TrySetValue Boolean Failed");
		}

	private:
		IPropertyValue& _value;
	};
}
