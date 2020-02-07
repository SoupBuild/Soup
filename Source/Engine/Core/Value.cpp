// <copyright file="Value.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

module;

#include <any>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

module SoupEngineCore;

using namespace Opal;

namespace Soup::Build
{

Value::Value() :
	_value()
{
}

Value::Value(ValueTable table) :
	_value(std::move(table))
{
}

OperationResult Value::TryGetType(uint64_t& type) const noexcept
{
	try
	{
		type = static_cast<uint64_t>(GetType());
		return 0;
	}
	catch (...)
	{
		// Unknown type
		return -1;
	}
}

OperationResult Value::TrySetType(uint64_t type) noexcept
{

	try
	{
		auto updatedType = static_cast<ValueType>(type);
		auto currentType = GetType();

		// Ignore requests to set to same type
		if (currentType != updatedType)
		{
			switch (updatedType)
			{
				case ValueType::Table:
					_value = ValueTable();
					break;
				case ValueType::List:
					_value = ValueList();
					break;
				case ValueType::String:
					_value = ValuePrimitive<const char*>();
					break;
				case ValueType::Integer:
					_value = ValuePrimitive<int64_t>();
					break;
				case ValueType::Float:
					_value = ValuePrimitive<double>();
					break;
				case ValueType::Boolean:
					_value = ValuePrimitive<bool>();
					break;
				default:
					// Unknown type
					return -2;
			}
		}

		return 0;
	}
	catch (...)
	{
		// Unknown error
		return -1;
	}
}

OperationResult Value::TryGetAsTable(IValueTable*& result) noexcept
{
	try
	{
		result = nullptr;
		if (_value.type() == typeid(ValueTable))
		{
			result = &std::any_cast<ValueTable&>(_value);
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

OperationResult Value::TryGetAsList(IValueList*& result) noexcept
{
	try
	{
		result = false;
		if (_value.type() == typeid(ValueList))
		{
			result = &std::any_cast<ValueList&>(_value);
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

OperationResult Value::TryGetAsString(IValuePrimitive<const char*>*& result) noexcept
{
	try
	{
		result = false;
		if (_value.type() == typeid(ValuePrimitive<const char*>))
		{
			result = &std::any_cast<ValuePrimitive<const char*>&>(_value);
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

OperationResult Value::TryGetAsInteger(IValuePrimitive<int64_t>*& result) noexcept
{
	try
	{
		result = false;
		if (_value.type() == typeid(ValuePrimitive<int64_t>))
		{
			result = &std::any_cast<ValuePrimitive<int64_t>&>(_value);
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

OperationResult Value::TryGetAsFloat(IValuePrimitive<double>*& result) noexcept
{
	try
	{
		result = false;
		if (_value.type() == typeid(ValuePrimitive<double>))
		{
			result = &std::any_cast<ValuePrimitive<double>&>(_value);
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

OperationResult Value::TryGetAsBoolean(IValuePrimitive<bool>*& result) noexcept
{
	try
	{
		result = false;
		if (_value.type() == typeid(ValuePrimitive<bool>))
		{
			result = &std::any_cast<ValuePrimitive<bool>&>(_value);
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

ValueType Value::GetType() const
{
	if (_value.has_value())
	{
		auto& valueType = _value.type();
		if (valueType == typeid(ValueTable))
			return ValueType::Table;
		else if (valueType == typeid(ValueList))
			return ValueType::List;
		else if (valueType == typeid(ValuePrimitive<const char*>))
			return ValueType::String;
		else if (valueType == typeid(ValuePrimitive<int64_t>))
			return ValueType::Integer;
		else if (valueType == typeid(ValuePrimitive<double>))
			return ValueType::Float;
		else if (valueType == typeid(ValuePrimitive<bool>))
			return ValueType::Boolean;
		else
			throw std::runtime_error("Unknown internal type.");
	}
	else
	{
		return ValueType::Empty;
	}
}

std::string Value::ToString()
{
	switch (GetType())
	{
		case ValueType::Empty:
			return "";
		case ValueType::Table:
			return std::any_cast<ValueTable&>(_value).ToString();
		case ValueType::List:
			return std::any_cast<ValueList&>(_value).ToString();
		case ValueType::String:
			return std::any_cast<ValuePrimitive<const char*>&>(_value).ToString();
		case ValueType::Integer:
			return std::any_cast<ValuePrimitive<int64_t>&>(_value).ToString();
		case ValueType::Float:
			return std::any_cast<ValuePrimitive<double>&>(_value).ToString();
		case ValueType::Boolean:
			return std::any_cast<ValuePrimitive<bool>&>(_value).ToString();
		default:
			return "UnknownType";
	}
}

ValueTable& Value::AsTable()
{
	if (_value.type() == typeid(ValueTable))
	{
		return std::any_cast<ValueTable&>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as Table with incorrect type.");
	}
}

ValueList& Value::AsList()
{
	if (_value.type() == typeid(ValueList))
	{
		return std::any_cast<ValueList&>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as List with incorrect type.");
	}
}

bool Value::operator ==(const Value& rhs) const
{
	if (GetType() == rhs.GetType())
	{
		switch (GetType())
		{
			case ValueType::Empty:
				return true;
			case ValueType::Table:
				return std::any_cast<const ValueTable&>(_value) == std::any_cast<const ValueTable&>(rhs._value);
			case ValueType::List:
				return std::any_cast<const ValueList&>(_value) == std::any_cast<const ValueList&>(rhs._value);
			case ValueType::String:
				return std::any_cast<const ValuePrimitive<const char*>&>(_value) == std::any_cast<const ValuePrimitive<const char*>&>(rhs._value);
			case ValueType::Integer:
				return std::any_cast<const ValuePrimitive<int64_t>&>(_value) == std::any_cast<const ValuePrimitive<int64_t>&>(rhs._value);
			case ValueType::Float:
				return std::any_cast<const ValuePrimitive<double>&>(_value) == std::any_cast<const ValuePrimitive<double>&>(rhs._value);
			case ValueType::Boolean:
				return std::any_cast<const ValuePrimitive<bool>&>(_value) == std::any_cast<const ValuePrimitive<bool>&>(rhs._value);
			default:
				throw std::runtime_error("Unkown ValueType for comparison.");
		}
	}
	else
	{
		return false;
	}
}

bool Value::operator !=(const Value& rhs) const
{
	return !(*this == rhs);
}

}
