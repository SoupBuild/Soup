// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	public static class ValueTableExtensions
	{
		public static ValueTable ToBuildValue(this SMLDocument value)
		{
			var values = new Dictionary<string, Value>();
			foreach (var item in value.Values)
			{
				values.Add(item.Key, item.Value.Value.ToBuildValue());
			}

			return new ValueTable(values);
		}

		public static ValueTable ToBuildValue(this SMLTable value)
		{
			var values = new Dictionary<string, Value>();
			foreach (var item in value.Values)
			{
				values.Add(item.Key, item.Value.Value.ToBuildValue());
			}

			return new ValueTable(values);
		}

		public static ValueList ToBuildValue(this SMLArray value)
		{
			var values = new List<Value>();
			foreach (var item in value.Values)
			{
				values.Add(item.Value.ToBuildValue());
			}

			return new ValueList(values);
		}

		public static Value ToBuildValue(this SMLValue value)
		{
			switch (value.Type)
			{
				case SMLValueType.Empty:
					return new Value();
				case SMLValueType.Boolean:
					return new Value(value.AsBoolean().Value);
				case SMLValueType.Integer:
					return new Value(value.AsInteger().Value);
				case SMLValueType.Float:
					return new Value(value.AsFloat().Value);
				case SMLValueType.String:
					return new Value(value.AsString().Value);
				case SMLValueType.Table:
					return new Value(value.AsTable().ToBuildValue());
				case SMLValueType.Array:
					return new Value(value.AsArray().ToBuildValue());
				default:
					throw new InvalidOperationException("Unknown SMLValueType");
			}
		}
	}
}
