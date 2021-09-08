// <copyright file="IValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Linq;

namespace Soup.Build
{
	public static class IValueTableExtensions
	{
		public static IValueTable EnsureValueTable(this IValueTable table, IValueFactory factory, string propertyName)
		{
			if (!table.ContainsKey(propertyName))
			{
				table.Add(propertyName, factory.Create(factory.CreateTable()));
			}

			return table[propertyName].AsTable();
		}

		public static IValueList EnsureValueList(this IValueTable table, IValueFactory factory, string propertyName)
		{
			if (!table.ContainsKey(propertyName))
			{
				table.Add(propertyName, factory.Create(factory.CreateList()));
			}

			return table[propertyName].AsList();
		}

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
