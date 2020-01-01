// <copyright file="BuildPropertyValue.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class BuildPropertyValue : public IBuildPropertyValue
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyValue class
		/// </summary>
		BuildPropertyValue() :
			_value()
		{
		}

		/// <summary>
		/// Type checker methods
		/// </summary>
		bool IsString() const noexcept override final
		{
			return _value.type() == typeid(std::string);
		}

		bool IsInteger() const noexcept override final
		{
			return _value.type() == typeid(int64_t);
		}

		bool IsDouble() const noexcept override final
		{
			return _value.type() == typeid(double);
		}

		bool IsBoolean() const noexcept override final
		{
			return _value.type() == typeid(bool);
		}

		/// <summary>
		/// Returns true if the propery is an immutable system constant
		/// </summary>
		bool IsConstant() const noexcept override final
		{
			return false;
		}

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		BuildSystemResult TryGetStringValue(const char*& result) const noexcept override final
		{
			try
			{
				result = false;
				if (_value.type() == typeid(std::string))
				{
					const std::string& value = std::any_cast<const std::string&>(_value);
					result = value.c_str();
					return 0;
				}
				else
				{
					// Wrong type
					return -2;
				}
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

		BuildSystemResult TryGetIntegerValue(int64_t& result) const noexcept override final
		{
			try
			{
				result = false;
				if (_value.type() == typeid(int64_t))
				{
					result = std::any_cast<int64_t>(_value);
					return 0;
				}
				else
				{
					// Wrong type
					return -2;
				}
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

		BuildSystemResult TryGetDoubleValue(double& result) const noexcept override final
		{
			try
			{
				result = false;
				if (_value.type() == typeid(double))
				{
					result = std::any_cast<double>(_value);
					return 0;
				}
				else
				{
					// Wrong type
					return -2;
				}
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

		BuildSystemResult TryGetBooleanValue(bool& result) const noexcept override final
		{
			try
			{
				result = false;
				if (_value.type() == typeid(bool))
				{
					result = std::any_cast<bool>(_value);
					return 0;
				}
				else
				{
					// Wrong type
					return -2;
				}
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

		/// <summary>
		/// Property setter methods.
		/// Note: Will perform a no-op if the value is immutable to prevent 
		/// exceptions accross dll boundaries.
		/// </summary>
		BuildSystemResult TrySetStringValue(const char* value) noexcept override final
		{
			try
			{
				_value = std::string(value);
				return 0;
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

		BuildSystemResult TrySetIntegerValue(int64_t value) noexcept override final
		{
			try
			{
				_value = value;
				return 0;
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

		BuildSystemResult TrySetDoubleValue(double value) noexcept override final
		{
			try
			{
				_value = value;
				return 0;
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

		BuildSystemResult TrySetBooleanValue(bool value) noexcept override final
		{
			try
			{
				_value = value;
				return 0;
			}
			catch (...)
			{
				// Unknown errors
				return -1;
			}
		}

	private:
		std::any _value;
	};
}
