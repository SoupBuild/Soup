// <copyright file="ValueWrapper.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

module;

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

module SoupEngineWrapper;

using namespace Opal;

namespace Soup::Build
{

ValueWrapper::ValueWrapper(IValue& value) :
	_value(value)
{
}

ValueType ValueWrapper::GetType() const
{
	uint64_t result = false;
	auto status = _value.TryGetType(result);
	if (status != 0)
		throw std::runtime_error("TryGetType Failed");

	return static_cast<ValueType>(result);
}

void ValueWrapper::SetType(ValueType type)
{
	auto status = _value.TrySetType(static_cast<uint64_t>(type));
	if (status != 0)
		throw std::runtime_error("TrySetType Failed");
}

ValueTableWrapper ValueWrapper::AsTable()
{
	IValueTable* result = nullptr;
	auto status = _value.TryGetAsTable(result);
	if (status != 0)
		throw std::runtime_error("TryGetAsTable Failed");
	if (result == nullptr)
		throw std::runtime_error("TryGetAsTable result was null.");

	return ValueTableWrapper(*result);
}

ValueListWrapper ValueWrapper::AsList()
{
	IValueList* result = nullptr;
	auto status = _value.TryGetAsList(result);
	if (status != 0)
		throw std::runtime_error("TryGetAsList Failed");
	if (result == nullptr)
		throw std::runtime_error("TryGetAsList result was null.");

	return ValueListWrapper(*result);
}

ValuePrimitiveWrapper<const char*> ValueWrapper::AsString()
{
	IValuePrimitive<const char*>* result = nullptr;
	auto status = _value.TryGetAsString(result);
	if (status != 0)
		throw std::runtime_error("TryGetAsString Failed");

	return ValuePrimitiveWrapper<const char*>(*result);
}

ValuePrimitiveWrapper<int64_t> ValueWrapper::AsInteger()
{
	IValuePrimitive<int64_t>* result = nullptr;
	auto status = _value.TryGetAsInteger(result);
	if (status != 0)
		throw std::runtime_error("TryGetAsInteger Failed");

	return ValuePrimitiveWrapper<int64_t>(*result);
}

ValuePrimitiveWrapper<double> ValueWrapper::AsFloat()
{
	IValuePrimitive<double>* result = nullptr;
	auto status = _value.TryGetAsFloat(result);
	if (status != 0)
		throw std::runtime_error("TryGetAsFloat Failed");

	return ValuePrimitiveWrapper<double>(*result);
}

ValuePrimitiveWrapper<bool> ValueWrapper::AsBoolean()
{
	IValuePrimitive<bool>* result = nullptr;
	auto status = _value.TryGetAsBoolean(result);
	if (status != 0)
		throw std::runtime_error("TryGetAsBoolean Failed");

	return ValuePrimitiveWrapper<bool>(*result);
}

ValueTableWrapper ValueWrapper::EnsureTable()
{
	auto type = GetType();

	// Auto convert empty values
	if (type == ValueType::Empty)
	{
		SetType(ValueType::Table);
	}
	else if (type != ValueType::Table)
	{
		throw std::runtime_error("EnsureTable will not auto convert other types.");
	}

	return AsTable();
}

ValueListWrapper ValueWrapper::EnsureList()
{
	auto type = GetType();

	// Auto convert empty values
	if (type == ValueType::Empty)
	{
		SetType(ValueType::List);
	}
	else if (type != ValueType::List)
	{
		throw std::runtime_error("EnsureList will not auto convert other types.");
	}

	return AsList();
}

ValuePrimitiveWrapper<const char*> ValueWrapper::EnsureString()
{
	auto type = GetType();

	// Auto convert empty values
	if (type == ValueType::Empty)
	{
		SetType(ValueType::String);
	}
	else if (type != ValueType::String)
	{
		throw std::runtime_error("EnsureString will not auto convert other types.");
	}

	return AsString();
}

ValuePrimitiveWrapper<int64_t> ValueWrapper::EnsureInteger()
{
	auto type = GetType();

	// Auto convert empty values
	if (type == ValueType::Empty)
	{
		SetType(ValueType::Integer);
	}
	else if (type != ValueType::Integer)
	{
		throw std::runtime_error("EnsureInteger will not auto convert other types.");
	}

	return AsInteger();
}

ValuePrimitiveWrapper<double> ValueWrapper::EnsureFloat()
{
	auto type = GetType();

	// Auto convert empty values
	if (type == ValueType::Empty)
	{
		SetType(ValueType::Float);
	}
	else if (type != ValueType::Float)
	{
		throw std::runtime_error("EnsureFloat will not auto convert other types.");
	}

	return AsFloat();
}

ValuePrimitiveWrapper<bool> ValueWrapper::EnsureBoolean()
{
	auto type = GetType();

	// Auto convert empty values
	if (type == ValueType::Empty)
	{
		SetType(ValueType::Boolean);
	}
	else if (type != ValueType::Boolean)
	{
		throw std::runtime_error("EnsureBoolean will not auto convert other types.");
	}

	return AsBoolean();
}

void ValueWrapper::SetValue(ValueWrapper value)
{
	// Perform a deep copy of the value
	switch (value.GetType())
	{
		case ValueType::Table:
			throw std::runtime_error("Not implemented");
			break;
		case ValueType::List:
			EnsureList().SetAll(value.AsList());
			break;
		case ValueType::String:
			EnsureString().SetValue(value.AsString().GetValue());
			break;
		case ValueType::Integer:
			EnsureInteger().SetValue(value.AsInteger().GetValue());
			break;
		case ValueType::Float:
			EnsureFloat().SetValue(value.AsFloat().GetValue());
			break;
		case ValueType::Boolean:
			EnsureBoolean().SetValue(value.AsBoolean().GetValue());
			break;
		default:
			// Unknown type
			throw std::runtime_error("Unknown type in SetValue");
	}
}

void ValueWrapper::SetValueStringList(const std::vector<std::string>& value)
{
	auto typedValue = EnsureList();
	typedValue.SetAll(value);
}

void ValueWrapper::SetValuePathList(const std::vector<Path>& value)
{
	auto typedValue = EnsureList();
	typedValue.SetAll(value);
}

void ValueWrapper::SetValueString(std::string_view value)
{
	auto typedValue = EnsureString();
	typedValue.SetValue(value.data());
}

void ValueWrapper::SetValueInteger(int64_t value)
{
	auto typedValue = EnsureInteger();
	typedValue.SetValue(value);
}

void ValueWrapper::SetValueFloat(double value)
{
	auto typedValue = EnsureFloat();
	typedValue.SetValue(value);
}

void ValueWrapper::SetValueBoolean(bool value)
{
	auto typedValue = EnsureBoolean();
	typedValue.SetValue(value);
}

IValue& ValueWrapper::GetReference()
{
	return _value;
}

}
