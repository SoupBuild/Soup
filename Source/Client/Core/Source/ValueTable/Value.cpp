// <copyright file="Value.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

module;

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

module Soup.Core;

using namespace Opal;

namespace Soup::Core
{

Value::Value(ValueTable table) :
	_value(std::move(table))
{
}

Value::Value(ValueList list) :
	_value(std::move(list))
{
}

Value::Value(std::string value) :
	_value(std::move(value))
{
}

Value::Value(int64_t value) :
	_value(value)
{
}

Value::Value(double value) :
	_value(value)
{
}

Value::Value(bool value) :
	_value(value)
{
}

ValueType Value::GetType() const
{
	switch (_value.index())
	{
		case 0:
			return ValueType::Table;
		case 1:
			return ValueType::List;
		case 2:
			return ValueType::String;
		case 3:
			return ValueType::Integer;
		case 4:
			return ValueType::Float;
		case 5:
			return ValueType::Boolean;
		default:
			throw std::runtime_error("Unknown value type.");
	}
}

ValueTable& Value::AsTable()
{
	if (GetType() == ValueType::Table)
	{
		return std::get<ValueTable>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as Table with incorrect type.");
	}
}

ValueList& Value::AsList()
{
	if (GetType() == ValueType::List)
	{
		return std::get<ValueList>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as List with incorrect type.");
	}
}

const ValueTable& Value::AsTable() const
{
	if (GetType() == ValueType::Table)
	{
		return std::get<ValueTable>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as Table with incorrect type.");
	}
}

const ValueList& Value::AsList() const
{
	if (GetType() == ValueType::List)
	{
		return std::get<ValueList>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as List with incorrect type.");
	}
}

const std::string& Value::AsString() const
{
	if (GetType() == ValueType::String)
	{
		return std::get<std::string>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as String with incorrect type.");
	}
}

int64_t Value::AsInteger() const
{
	if (GetType() == ValueType::Integer)
	{
		return std::get<int64_t>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as Integer with incorrect type.");
	}
}

double Value::AsFloat() const
{
	if (GetType() == ValueType::Float)
	{
		return std::get<double>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as Float with incorrect type.");
	}
}

bool Value::AsBoolean() const
{
	if (GetType() == ValueType::Boolean)
	{
		return std::get<bool>(_value);
	}
	else
	{
		// Wrong type
		throw std::runtime_error("Attempt to access value as Boolean with incorrect type.");
	}
}

bool Value::operator ==(const Value& rhs) const
{
	if (GetType() == rhs.GetType())
	{
		switch (GetType())
		{
			case ValueType::Table:
				return std::get<ValueTable>(_value) == std::get<ValueTable>(rhs._value);
			case ValueType::List:
				return std::get<ValueList>(_value) == std::get<ValueList>(rhs._value);
			case ValueType::String:
				return std::get<std::string>(_value) == std::get<std::string>(rhs._value);
			case ValueType::Integer:
				return std::get<int64_t>(_value) == std::get<int64_t>(rhs._value);
			case ValueType::Float:
				return std::get<double>(_value) == std::get<double>(rhs._value);
			case ValueType::Boolean:
				return std::get<bool>(_value) == std::get<bool>(rhs._value);
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
