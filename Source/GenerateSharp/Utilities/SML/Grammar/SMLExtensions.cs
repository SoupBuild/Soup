// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Utilities
{
	public static class SMLExtensions
	{
		public static void AddItemWithSyntax(this SMLArray list, string value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			list.Values.Add(newValue);
		}

		public static SMLTable AddTableWithSyntax(this SMLArray list)
		{
			var newTable = new SMLTable();

			// Add the model to the parent table model
			list.Values.Add(new SMLValue(newTable));

			return newTable;
		}

		public static SMLArray AddArrayWithSyntax(this SMLDocument document, string name)
		{
			var newArray = new SMLArray();

			// Add the model to the parent table model
			document.Values.Add(name, new SMLTableValue(new SMLValue(newArray)));

			return newArray;
		}

		public static SMLArray AddArrayWithSyntax(this SMLTable table, string name)
		{
			var newArray = new SMLArray();

			// Add the model to the parent table model
			table.Values.Add(name, new SMLTableValue(new SMLValue(newArray)));

			return newArray;
		}

		public static SMLTable AddTableWithSyntax(this SMLDocument document, string name)
		{
			// Create a new table
			var newTable = new SMLTable();

			// Add the model to the parent table model
			document.Values.Add(name, new SMLTableValue(new SMLValue(newTable)));

			return newTable;
		}

		public static SMLTable AddTableWithSyntax(this SMLTable table, string name)
		{
			// Create a new table
			var newTable = new SMLTable();

			// Add the model to the parent table model
			table.Values.Add(name, new SMLTableValue(new SMLValue(newTable)));

			return newTable;
		}

		public static void AddItemWithSyntax(this SMLDocument document, string key, long value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			document.Values.Add(key, new SMLTableValue(newValue));
		}

		public static void AddItemWithSyntax(this SMLDocument document, string key, string value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			document.Values.Add(key, new SMLTableValue(newValue));
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, long value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			table.Values.Add(key, new SMLTableValue(newValue));
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, string value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			table.Values.Add(key, new SMLTableValue(newValue));
		}
	}
}
