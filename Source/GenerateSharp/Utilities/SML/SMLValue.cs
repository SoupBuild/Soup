// <copyright file="SMLValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
		Type = SMLValueType.Empty;
		RawValue = null;
	}

	public SMLValue(SMLFloatValue value)
	{
		Type = SMLValueType.Float;
		RawValue = value;
	}

	public SMLValue(SMLIntegerValue value)
	{
		Type = SMLValueType.Integer;
		RawValue = value;
	}

	public SMLValue(SMLBooleanValue value)
	{
		Type = SMLValueType.Boolean;
		RawValue = value;
	}

	public SMLValue(SMLStringValue value)
	{
		Type = SMLValueType.String;
		RawValue = value;
	}

	public SMLValue(SMLArray value)
	{
		Type = SMLValueType.Array;
		RawValue = value;
	}

	public SMLValue(SMLTable value)
	{
		Type = SMLValueType.Table;
		RawValue = value;
	}

	public SMLValue(SMLVersionValue value)
	{
		Type = SMLValueType.Version;
		RawValue = value;
	}

	public SMLValue(SMLPackageReferenceValue value)
	{
		Type = SMLValueType.PackageReference;
		RawValue = value;
	}

	public SMLValue(SMLLanguageReferenceValue value)
	{
		Type = SMLValueType.LanguageReference;
		RawValue = value;
	}

	public SMLArray AsArray()
	{
		if (Type != SMLValueType.Array)
			throw new InvalidCastException("Incorrect access type: Value is not Array");
		else if (RawValue is SMLArray value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Array");
	}

	public SMLTable AsTable()
	{
		if (Type != SMLValueType.Table)
			throw new InvalidCastException("Incorrect access type: Value is not Table");
		else if (RawValue is SMLTable value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Table");
	}

	public SMLStringValue AsString()
	{
		if (Type != SMLValueType.String)
			throw new InvalidCastException("Incorrect access type: Value is not String");
		else if (RawValue is SMLStringValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: String");
	}

	public SMLIntegerValue AsInteger()
	{
		if (Type != SMLValueType.Integer)
			throw new InvalidCastException("Incorrect access type: Value is not Integer");
		else if (RawValue is SMLIntegerValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Integer");
	}

	public SMLBooleanValue AsBoolean()
	{
		if (Type != SMLValueType.Boolean)
			throw new InvalidCastException("Incorrect access type: Value is not Boolean");
		else if (RawValue is SMLBooleanValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Boolean");
	}

	public SMLFloatValue AsFloat()
	{
		if (Type != SMLValueType.Float)
			throw new InvalidCastException("Incorrect access type: Value is not Float");
		else if (RawValue is SMLFloatValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Float");
	}

	public SMLVersionValue AsVersion()
	{
		if (Type != SMLValueType.Version)
			throw new InvalidCastException("Incorrect access type: Value is not Version");
		else if (RawValue is SMLVersionValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: Version");
	}

	public SMLPackageReferenceValue AsPackageReference()
	{
		if (Type != SMLValueType.PackageReference)
			throw new InvalidCastException("Incorrect access type: Value is not PackageReference");
		else if (RawValue is SMLPackageReferenceValue value)
			return value;
		else
			throw new InvalidOperationException("Underlying type was incorrect: PackageReference");
	}

	public SMLLanguageReferenceValue AsLanguageReference()
	{
		if (Type != SMLValueType.LanguageReference)
			throw new InvalidCastException("Incorrect access type: Value is not LanguageReference");
		else if (RawValue is SMLLanguageReferenceValue value)
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
		if (Type != other.Type)
			return false;

		return Type switch
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
		return HashCode.Combine(Type, RawValue);
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