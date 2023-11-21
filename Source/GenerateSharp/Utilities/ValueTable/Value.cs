// <copyright file="ValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

public class Value
{
	public ValueType Type { get; private set; }

	public object? RawValue { get; private set; }

	private Value(ValueType type, object? rawValue)
	{
		Type = type;
		RawValue = rawValue;
	}

	public Value()
	{
		Type = ValueType.Empty;
		RawValue = null;
	}

	public Value(bool value)
	{
		Type = ValueType.Boolean;
		RawValue = value;
	}

	public Value(long value)
	{
		Type = ValueType.Integer;
		RawValue = value;
	}

	public Value(double value)
	{
		Type = ValueType.Float;
		RawValue = value;
	}

	public Value(string value)
	{
		Type = ValueType.String;
		RawValue = value;
	}

	public Value(ValueTable value)
	{
		Type = ValueType.Table;
		RawValue = value;
	}

	public Value(ValueList value)
	{
		Type = ValueType.List;
		RawValue = value;
	}

	public Value Clone()
	{
		return new Value(Type, RawValue);
	}

	public bool IsTable()
	{
		return Type == ValueType.Table;
	}

	public ValueTable AsTable()
	{
		if (IsTable() && RawValue is ValueTable result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Table");
	}

	public bool IsList()
	{
		return Type == ValueType.List;
	}

	public ValueList AsList()
	{
		if (IsList() && RawValue is ValueList result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: List");
	}

	public bool IsString()
	{
		return Type == ValueType.String;
	}

	public string AsString()
	{
		if (IsString() && RawValue is string result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: String");
	}

	public bool IsInteger()
	{
		return Type == ValueType.Integer;
	}

	public long AsInteger()
	{
		if (IsInteger() && RawValue is long result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Integer");
	}

	public bool IsFloat()
	{
		return Type == ValueType.Float;
	}

	public double AsFloat()
	{
		if (IsFloat() && RawValue is double result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Float");
	}

	public bool IsBoolean()
	{
		return Type == ValueType.Boolean;
	}

	public bool AsBoolean()
	{
		if (IsBoolean() && RawValue is bool result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Boolean");
	}

	public override string ToString()
	{
		switch (Type)
		{
			case ValueType.Table:
				return this.AsTable().ToString() ?? string.Empty;
			case ValueType.List:
				return this.AsList().ToString() ?? string.Empty;
			case ValueType.String:
				return $"\"{this.AsString()}\"";
			case ValueType.Integer:
				return this.AsInteger().ToString();
			case ValueType.Float:
				return this.AsFloat().ToString();
			case ValueType.Boolean:
				return this.AsBoolean().ToString();
			default:
				return "UNKNOWN";
		}
	}
}