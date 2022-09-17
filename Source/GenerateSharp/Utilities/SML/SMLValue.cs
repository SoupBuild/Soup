// <copyright file="SMLValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities
{
	public enum SMLValueType
	{
		Empty,
		Boolean,
		Integer,
		Float,
		String,
		Table,
		Array,
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

		public SMLValue(long value)
		{
			Type = SMLValueType.Integer;
			RawValue = value;
		}

		public SMLValue(bool value)
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

		public long AsInteger()
		{
			if (Type != SMLValueType.Integer)
				throw new InvalidCastException("Incorrect access type: Value is not Integer");
			else if (RawValue is long value)
				return value;
			else
				throw new InvalidOperationException("Underlying type was incorrect: Integer");
		}

		public bool AsBoolean()
		{
			if (Type != SMLValueType.Boolean)
				throw new InvalidCastException("Incorrect access type: Value is not Boolean");
			else if (RawValue is bool value)
				return value;
			else
				throw new InvalidOperationException("Underlying type was incorrect: Boolean");
		}

		public double AsFloat()
		{
			if (Type != SMLValueType.Float)
				throw new InvalidCastException("Incorrect access type: Value is not Float");
			else if (RawValue is double value)
				return value;
			else
				throw new InvalidOperationException("Underlying type was incorrect: Float");
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLValue);

		public bool Equals(SMLValue? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			if (this.Type != rhs.Type)
				return false;

			switch (this.Type)
			{
				case SMLValueType.Empty:
					return true;
				case SMLValueType.Boolean:
					return this.AsBoolean() == rhs.AsBoolean();
				case SMLValueType.Integer:
					return this.AsInteger() == rhs.AsInteger();
				case SMLValueType.Float:
					return this.AsFloat() == rhs.AsFloat();
				case SMLValueType.String:
					return this.AsString() == rhs.AsString();
				case SMLValueType.Table:
					return this.AsTable() == rhs.AsTable();
				case SMLValueType.Array:
					return this.AsArray() == rhs.AsArray();
				default:
					throw new InvalidOperationException("Unknown SMLValueType");
			}
		}

		public override int GetHashCode() => (Type, RawValue).GetHashCode();

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

		public static bool operator !=(SMLValue? lhs, SMLValue? rhs) => !(lhs == rhs);
	}
}
