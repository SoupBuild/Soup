// <copyright file="IValueExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build
{
	using System;

	public static class IValueExtensions
	{
		public static bool IsTable(this IValue value)
		{
			return value.Type == ValueType.Table;
		}

		public static IValueTable AsTable(this IValue value)
		{
			if (value.IsTable() && value.RawValue is IValueTable result)
				return result;
			else
				throw new InvalidOperationException("Attempt to get value as incorrect type: Table");
		}

		public static bool IsList(this IValue value)
		{
			return value.Type == ValueType.List;
		}

		public static IValueList AsList(this IValue value)
		{
			if (value.IsList() && value.RawValue is IValueList result)
				return result;
			else
				throw new InvalidOperationException("Attempt to get value as incorrect type: List");
		}

		public static bool IsString(this IValue value)
		{
			return value.Type == ValueType.String;
		}

		public static string AsString(this IValue value)
		{
			if (value.IsString() && value.RawValue is string result)
				return result;
			else
				throw new InvalidOperationException("Attempt to get value as incorrect type: String");
		}

		public static bool IsInteger(this IValue value)
		{
			return value.Type == ValueType.Integer;
		}

		public static long AsInteger(this IValue value)
		{
			if (value.IsInteger() && value.RawValue is long result)
				return result;
			else
				throw new InvalidOperationException("Attempt to get value as incorrect type: Integer");
		}

		public static bool IsFloat(this IValue value)
		{
			return value.Type == ValueType.Float;
		}

		public static double AsFloat(this IValue value)
		{
			if (value.IsFloat() && value.RawValue is double result)
				return result;
			else
				throw new InvalidOperationException("Attempt to get value as incorrect type: Float");
		}

		public static bool IsBoolean(this IValue value)
		{
			return value.Type == ValueType.Boolean;
		}

		public static bool AsBoolean(this IValue value)
		{
			if (value.IsBoolean() && value.RawValue is bool result)
				return result;
			else
				throw new InvalidOperationException("Attempt to get value as incorrect type: Boolean");
		}
	}
}
