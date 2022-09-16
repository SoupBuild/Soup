// <copyright file="SML.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System;
using System.Collections.Generic;
using System.Linq;

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

	public class SMLTable : IEquatable<SMLTable>
	{
		private Dictionary<string, SMLValue> _value;

		public SMLTable()
		{
			_value = new Dictionary<string, SMLValue>();
		}

		public SMLTable(Dictionary<string, SMLValue> value)
		{
			_value = value;
		}

		public SMLValue this[string key]
		{
			get
			{
				return _value[key];
			}
		}

		public Dictionary<string, SMLValue> GetValue()
		{
			return _value;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLTable);

		public bool Equals(SMLTable? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return Enumerable.SequenceEqual(this._value, rhs._value);
		}

		public override int GetHashCode() => (_value).GetHashCode();

		public static bool operator ==(SMLTable? lhs, SMLTable? rhs)
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

		public static bool operator !=(SMLTable? lhs, SMLTable? rhs) => !(lhs == rhs);
	}

	public class SMLArray : IEquatable<SMLArray>
	{
		private List<SMLValue> _value;

		public SMLArray()
		{
			_value = new List<SMLValue>();
		}

		public SMLArray(List<SMLValue> value)

		{
			_value = value;
		}

		public SMLValue this[int key]
		{
			get
			{
				return _value[key];
			}
		}

		public int GetSize()
		{
			return _value.Count;
		}

		public List<SMLValue> GetValue()
		{
			return _value;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLArray);

		public bool Equals(SMLArray? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return Enumerable.SequenceEqual(this._value, rhs._value);
		}

		public override int GetHashCode() => (_value).GetHashCode();

		public static bool operator ==(SMLArray? lhs, SMLArray? rhs)
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

		public static bool operator !=(SMLArray? lhs, SMLArray? rhs) => !(lhs == rhs);
	}

	/// <summary>
	/// The SML Document
	/// </summary>
	public class SMLDocument
	{
		private SMLTable _root;

		public SMLDocument(SMLTable root)
		{
			_root = root;
		}

		public SMLTable GetRoot()
		{
			return _root;
		}
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

		public SMLValue(string value)
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

		public string AsString()
		{
			if (Type != SMLValueType.String)
				throw new InvalidCastException("Incorrect access type: Value is not String");
			else if (RawValue is string value)
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
