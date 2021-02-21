// <copyright file="IValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build
{
	public interface IValueTable : IDictionary<string, Value>
	{
	}

	public interface IValueList : IList<Value>
	{
	}

	public enum ValueType : uint
	{
		Empty = 1,
		Table = 2,
		List = 3,
		String = 4,
		Integer = 5,
		Float = 6,
		Boolean = 7,
	};

	/// <summary>
	/// The build property value interface definition to allow build extensions read/write
	/// access to a single property value.
	/// Note: Has strict interface requirements to prevent version incompatible
	/// </summary>
	public class Value
	{
		private bool _booleanValue;
		private long _integerValue;
		private double _floatValue;
		private string? _stringValue;
		private IValueTable? _tableValue;
		private IValueList? _listValue;

		public Value()
		{
			Type = ValueType.Empty;
		}

		public Value(bool value)
		{
			Type = ValueType.Boolean;
			_booleanValue = value;
		}

		public Value(long value)
		{
			Type = ValueType.Integer;
			_integerValue = value;
		}

		public Value(double value)
		{
			Type = ValueType.Float;
			_floatValue = value;
		}

		public Value(string value)
		{
			Type = ValueType.String;
			_stringValue = value;
		}

		public Value(IValueTable value)
		{
			Type = ValueType.Table;
			_tableValue = value;
		}

		public Value(IValueList value)
		{
			Type = ValueType.List;
			_listValue = value;
		}

		/// <summary>
		/// Type checker methods
		/// </summary>
		public ValueType Type { get; init; }

		public IValueTable AsTable()
		{
			if (Type == ValueType.Table && !ReferenceEquals(_tableValue, null))
				return _tableValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Table");
		}

		public IValueList AsList()
		{
			if (Type == ValueType.List && !ReferenceEquals(_listValue, null))
				return _listValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: List");
		}

		public string AsString()
		{
			if (Type == ValueType.String && !ReferenceEquals(_stringValue, null))
				return _stringValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: String");
		}

		public long AsInteger()
		{
			if (Type == ValueType.Integer)
				return _integerValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Integer");
		}

		public double AsFloat()
		{
			if (Type == ValueType.Float)
				return _floatValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Float");
		}

		public bool AsBoolean()
		{
			if (Type == ValueType.Boolean)
				return _booleanValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Boolean");
		}
	};
}
