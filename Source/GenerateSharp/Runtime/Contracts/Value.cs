// <copyright file="ValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Runtime
{
	public class Value : IValue
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

		public Value(IValueTable value)
		{
			Type = ValueType.Table;
			RawValue = value;
		}

		public Value(IValueList value)
		{
			Type = ValueType.List;
			RawValue = value;
		}

		public Value Clone()
		{
			return new Value(Type, RawValue);
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
}
