// <copyright file="IValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
	}
}
