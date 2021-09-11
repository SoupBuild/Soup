// <copyright file="IValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build
{
	public static class IValueListExtensions
	{
		public static void SetAll(this IValueList list, IEnumerable<IValue> values)
		{
			list.Clear();
			foreach (var value in values)
				list.Add(value);
		}

		public static void SetAll(this IValueList list, IValueFactory factory, IEnumerable<string> values)
		{
			list.Clear();
			foreach (var value in values)
				list.Add(factory.Create(value));
		}

		public static void Append(this IValueList list, IEnumerable<IValue> values)
		{
			foreach (var value in values)
				list.Add(value);
		}


		public static void Append(this IValueList list, IValueFactory factory, IEnumerable<string> values)
		{
			foreach (var value in values)
				list.Add(factory.Create(value));
		}
	}
}
