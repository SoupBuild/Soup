// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	public static class ValueTableExtensions
	{
		public static IValueTable ToBuildValue(this SMLTable value)
		{
			var values = new Dictionary<string, IValue>();
			foreach (var item in value.GetValue())
			{
				values.Add(item.Key, item.Value.ToBuildValue());
			}

			return new ValueTable(values);
		}

		public static IValueList ToBuildValue(this SMLArray value)
		{
			var values = new List<IValue>();
			foreach (var item in value.GetValue())
			{
				values.Add(item.ToBuildValue());
			}

			return new ValueList(values);
		}

		public static IValue ToBuildValue(this SMLValue value)
		{
			switch (value.Type)
			{
				case SMLValueType.Empty:
					return new Value();
				case SMLValueType.Boolean:
					return new Value(value.AsBoolean());
				case SMLValueType.Integer:
					return new Value(value.AsInteger());
				case SMLValueType.Float:
					return new Value(value.AsFloat());
				case SMLValueType.String:
					return new Value(value.AsString());
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
