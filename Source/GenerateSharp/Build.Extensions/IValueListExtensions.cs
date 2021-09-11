// <copyright file="IValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;

namespace Soup.Build
{
	public static class IValueListExtensions
	{
		public static IValueList SetAll(this IValueList list, IEnumerable<IValue> values)
		{
			list.Clear();
			foreach (var value in values)
				list.Add(value);
			return list;
		}

		public static IValueList SetAll(this IValueList list, IValueFactory factory, IEnumerable<string> values)
		{
			list.Clear();
			foreach (var value in values)
				list.Add(factory.Create(value));
			return list;
		}
		public static IValueList SetAll(this IValueList list, IValueFactory factory, IEnumerable<Path> values)
		{
			list.Clear();
			foreach (var value in values)
				list.Add(factory.Create(value.ToString()));
			return list;
		}

		public static IValueList Append(this IValueList list, IEnumerable<IValue> values)
		{
			foreach (var value in values)
				list.Add(value);
			return list;
		}


		public static IValueList Append(this IValueList list, IValueFactory factory, IEnumerable<string> values)
		{
			foreach (var value in values)
				list.Add(factory.Create(value));
			return list;
		}

		public static IValueList Append(this IValueList list, IValueFactory factory, IEnumerable<Path> values)
		{
			foreach (var value in values)
				list.Add(factory.Create(value.ToString()));
			return list;
		}
	}
}
