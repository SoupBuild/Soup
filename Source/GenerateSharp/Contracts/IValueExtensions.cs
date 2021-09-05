// <copyright file="IValueExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build
{
	public static class IValueExtensions
	{
		public static IValueTable AsTable(this IValue value)
		{
			if (value.Type == ValueType.Table)
				return (IValueTable)value.RawValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Table");
		}

		public static IValueList AsList(this IValue value)
		{
			if (value.Type == ValueType.List)
				return (IValueList)value.RawValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: List");
		}

		public static string AsString(this IValue value)
		{
			if (value.Type == ValueType.String)
				return (string)value.RawValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: String");
		}

		public static long AsInteger(this IValue value)
		{
			if (value.Type == ValueType.Integer)
				return (long)value.RawValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Integer");
		}

		public static double AsFloat(this IValue value)
		{
			if (value.Type == ValueType.Float)
				return (double)value.RawValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Float");
		}

		public static bool AsBoolean(this IValue value)
		{
			if (value.Type == ValueType.Boolean)
				return (bool)value.RawValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: Boolean");
		}

		public static DateTime AsDateTime(this IValue value)
		{
			if (value.Type == ValueType.DateTime)
				return (DateTime)value.RawValue;
			else
				throw new InvalidOperationException("Attept to get value as incorrect type: DateTime");
		}
	}
}
