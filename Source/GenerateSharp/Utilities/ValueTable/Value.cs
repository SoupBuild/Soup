// <copyright file="ValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Globalization;

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

	public Value(SemanticVersion value)
	{
		Type = ValueType.Version;
		RawValue = value;
	}

	public Value(LanguageReference value)
	{
		Type = ValueType.LanguageReference;
		RawValue = value;
	}

	public Value(PackageReference value)
	{
		Type = ValueType.PackageReference;
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

	public bool IsVersion()
	{
		return Type == ValueType.Version;
	}

	public SemanticVersion AsVersion()
	{
		if (IsVersion() && RawValue is SemanticVersion result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Version");
	}

	public bool IsLanguageReference()
	{
		return Type == ValueType.LanguageReference;
	}

	public LanguageReference AsLanguageReference()
	{
		if (IsLanguageReference() && RawValue is LanguageReference result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: LanguageReference");
	}

	public bool IsPackageReference()
	{
		return Type == ValueType.PackageReference;
	}

	public PackageReference AsPackageReference()
	{
		if (IsPackageReference() && RawValue is PackageReference result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: PackageReference");
	}

	public override string ToString()
	{
		return Type switch
		{
			ValueType.Table => AsTable().ToString() ?? string.Empty,
			ValueType.List => AsList().ToString() ?? string.Empty,
			ValueType.String => $"\"{AsString()}\"",
			ValueType.Integer => AsInteger().ToString(CultureInfo.InvariantCulture),
			ValueType.Float => AsFloat().ToString(CultureInfo.InvariantCulture),
			ValueType.Boolean => AsBoolean().ToString(),
			ValueType.Version => AsVersion().ToString(),
			ValueType.PackageReference => AsPackageReference().ToString(),
			ValueType.LanguageReference => AsLanguageReference().ToString(),
			_ => "UNKNOWN",
		};
	}
}