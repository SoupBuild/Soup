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
		this.Type = type;
		this.RawValue = rawValue;
	}

	public Value(bool value)
	{
		this.Type = ValueType.Boolean;
		this.RawValue = value;
	}

	public Value(long value)
	{
		this.Type = ValueType.Integer;
		this.RawValue = value;
	}

	public Value(double value)
	{
		this.Type = ValueType.Float;
		this.RawValue = value;
	}

	public Value(string value)
	{
		this.Type = ValueType.String;
		this.RawValue = value;
	}

	public Value(ValueTable value)
	{
		this.Type = ValueType.Table;
		this.RawValue = value;
	}

	public Value(ValueList value)
	{
		this.Type = ValueType.List;
		this.RawValue = value;
	}

	public Value(SemanticVersion value)
	{
		this.Type = ValueType.Version;
		this.RawValue = value;
	}

	public Value(LanguageReference value)
	{
		this.Type = ValueType.LanguageReference;
		this.RawValue = value;
	}

	public Value(PackageReference value)
	{
		this.Type = ValueType.PackageReference;
		this.RawValue = value;
	}

	public Value Clone()
	{
		return new Value(this.Type, this.RawValue);
	}

	public bool IsTable()
	{
		return this.Type == ValueType.Table;
	}

	public ValueTable AsTable()
	{
		if (IsTable() && this.RawValue is ValueTable result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Table");
	}

	public bool IsList()
	{
		return this.Type == ValueType.List;
	}

	public ValueList AsList()
	{
		if (IsList() && this.RawValue is ValueList result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: List");
	}

	public bool IsString()
	{
		return this.Type == ValueType.String;
	}

	public string AsString()
	{
		if (IsString() && this.RawValue is string result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: String");
	}

	public bool IsInteger()
	{
		return this.Type == ValueType.Integer;
	}

	public long AsInteger()
	{
		if (IsInteger() && this.RawValue is long result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Integer");
	}

	public bool IsFloat()
	{
		return this.Type == ValueType.Float;
	}

	public double AsFloat()
	{
		if (IsFloat() && this.RawValue is double result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Float");
	}

	public bool IsBoolean()
	{
		return this.Type == ValueType.Boolean;
	}

	public bool AsBoolean()
	{
		if (IsBoolean() && this.RawValue is bool result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Boolean");
	}

	public bool IsVersion()
	{
		return this.Type == ValueType.Version;
	}

	public SemanticVersion AsVersion()
	{
		if (IsVersion() && this.RawValue is SemanticVersion result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: Version");
	}

	public bool IsLanguageReference()
	{
		return this.Type == ValueType.LanguageReference;
	}

	public LanguageReference AsLanguageReference()
	{
		if (IsLanguageReference() && this.RawValue is LanguageReference result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: LanguageReference");
	}

	public bool IsPackageReference()
	{
		return this.Type == ValueType.PackageReference;
	}

	public PackageReference AsPackageReference()
	{
		if (IsPackageReference() && this.RawValue is PackageReference result)
			return result;
		else
			throw new InvalidOperationException("Attempt to get value as incorrect type: PackageReference");
	}

	public override string ToString()
	{
		return this.Type switch
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