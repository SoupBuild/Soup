// <copyright file="SMLValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Diagnostics.CodeAnalysis;

namespace Soup.Build.Utilities;

[SuppressMessage("Naming", "CA1720:Identifier contains type name", Justification = "Enum of types")]
public enum SMLValueType
{
	Empty,
	Boolean,
	Integer,
	Float,
	String,
	Table,
	Array,
	Version,
	PackageReference,
	LanguageReference,
}

public class SMLValue : IEquatable<SMLValue>
{
	public SMLValueType Type { get; private set; }

	public object? RawValue { get; private set; }

	public SMLValue()
	{
		this.Type = SMLValueType.Empty;
		this.RawValue = null;
	}

	public SMLValue(SMLFloatValue value)
	{
		this.Type = SMLValueType.Float;
		this.RawValue = value;
	}

	public SMLValue(SMLIntegerValue value)
	{
		this.Type = SMLValueType.Integer;
		this.RawValue = value;
	}

	public SMLValue(SMLBooleanValue value)
	{
		this.Type = SMLValueType.Boolean;
		this.RawValue = value;
	}

	public SMLValue(SMLStringValue value)
	{
		this.Type = SMLValueType.String;
		this.RawValue = value;
	}

	public SMLValue(SMLArray value)
	{
		this.Type = SMLValueType.Array;
		this.RawValue = value;
	}

	public SMLValue(SMLTable value)
	{
		this.Type = SMLValueType.Table;
		this.RawValue = value;
	}

	public SMLValue(SMLVersionValue value)
	{
		this.Type = SMLValueType.Version;
		this.RawValue = value;
	}

	public SMLValue(SMLPackageReferenceValue value)
	{
		this.Type = SMLValueType.PackageReference;
		this.RawValue = value;
	}

	public SMLValue(SMLLanguageReferenceValue value)
	{
		this.Type = SMLValueType.LanguageReference;
		this.RawValue = value;
	}

	public SMLArray AsArray()
	{
		if (this.Type != SMLValueType.Array)
			throw new InvalidCastException("Incorrect access type: Value is not Array");
		else if (this.RawValue is SMLArray value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Array");
	}

	public SMLTable AsTable()
	{
		if (this.Type != SMLValueType.Table)
			throw new InvalidCastException("Incorrect access type: Value is not Table");
		else if (this.RawValue is SMLTable value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Table");
	}

	public SMLStringValue AsString()
	{
		if (this.Type != SMLValueType.String)
			throw new InvalidCastException("Incorrect access type: Value is not String");
		else if (this.RawValue is SMLStringValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: String");
	}

	public SMLIntegerValue AsInteger()
	{
		if (this.Type != SMLValueType.Integer)
			throw new InvalidCastException("Incorrect access type: Value is not Integer");
		else if (this.RawValue is SMLIntegerValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Integer");
	}

	public SMLBooleanValue AsBoolean()
	{
		if (this.Type != SMLValueType.Boolean)
			throw new InvalidCastException("Incorrect access type: Value is not Boolean");
		else if (this.RawValue is SMLBooleanValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Boolean");
	}

	public SMLFloatValue AsFloat()
	{
		if (this.Type != SMLValueType.Float)
			throw new InvalidCastException("Incorrect access type: Value is not Float");
		else if (this.RawValue is SMLFloatValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Float");
	}

	public SMLVersionValue AsVersion()
	{
		if (this.Type != SMLValueType.Version)
			throw new InvalidCastException("Incorrect access type: Value is not Version");
		else if (this.RawValue is SMLVersionValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Version");
	}

	public SMLPackageReferenceValue AsPackageReference()
	{
		if (this.Type != SMLValueType.PackageReference)
			throw new InvalidCastException("Incorrect access type: Value is not PackageReference");
		else if (this.RawValue is SMLPackageReferenceValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: PackageReference");
	}

	public SMLLanguageReferenceValue AsLanguageReference()
	{
		if (this.Type != SMLValueType.LanguageReference)
			throw new InvalidCastException("Incorrect access type: Value is not LanguageReference");
		else if (this.RawValue is SMLLanguageReferenceValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: LanguageReference");
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLValue);
	}

	public bool Equals(SMLValue? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		if (this.Type != other.Type)
			return false;

		return this.Type switch
		{
			SMLValueType.Empty => true,
			SMLValueType.Boolean => AsBoolean() == other.AsBoolean(),
			SMLValueType.Integer => AsInteger() == other.AsInteger(),
			SMLValueType.Float => AsFloat() == other.AsFloat(),
			SMLValueType.String => AsString() == other.AsString(),
			SMLValueType.Table => AsTable() == other.AsTable(),
			SMLValueType.Array => AsArray() == other.AsArray(),
			SMLValueType.Version => AsVersion() == other.AsVersion(),
			SMLValueType.PackageReference => AsPackageReference() == other.AsPackageReference(),
			SMLValueType.LanguageReference => AsLanguageReference() == other.AsLanguageReference(),
			_ => false,
		};
	}

	public override int GetHashCode()
	{
		return HashCode.Combine(this.Type, this.RawValue);
	}

	public static bool operator ==(SMLValue? lhs, SMLValue? rhs)
	{
		if (lhs is null)
		{
			if (rhs is null)
				return true;
			else
				return false;
		}

		return lhs.Equals(rhs);
	}

	public static bool operator !=(SMLValue? lhs, SMLValue? rhs)
	{
		return !(lhs == rhs);
	}
}