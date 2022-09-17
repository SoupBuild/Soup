// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

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
			var newArray = new SMLArray(
				new SMLToken("["),
				new List<SMLValue>(),
				new SMLToken("]"));

			// If this is not the first item then place it on a newline
			var keyLeadingTrivia = new List<string>();
			if (document.Values.Count > 0)
				keyLeadingTrivia.Add("\r\n");

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = keyLeadingTrivia,
			};

			// Add the model to the parent table model
			document.Values.Add(
				name,
				CreateTableValue(keyToken, new SMLValue(newArray)));

			return newArray;
		}

		public static SMLArray AddArrayWithSyntax(this SMLTable table, string name)
		{
			var newArray = new SMLArray(
				new SMLToken("["),
				new List<SMLValue>(),
				new SMLToken("]"));

			// Add the model to the parent table model
			table.Values.Add(name, CreateTableValue(name, new SMLValue(newArray)));

			return newArray;
		}

		public static SMLTable AddTableWithSyntax(this SMLDocument document, string name)
		{
			// Create a new table
			var newTable = new SMLTable(
				new SMLToken("{"),
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}"));

			// If this is not the first item then place it on a newline
			var keyLeadingTrivia = new List<string>();
			if (document.Values.Count > 0)
				keyLeadingTrivia.Add("\r\n");

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = keyLeadingTrivia,
			};

			// Add the model to the parent table model
			document.Values.Add(name, CreateTableValue(keyToken, new SMLValue(newTable)));

			return newTable;
		}

		public static SMLTable AddTableWithSyntax(this SMLTable table, string name)
		{
			// Create a new table
			var newTable = new SMLTable(
				new SMLToken("{"),
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}"));

			// Add the model to the parent table model
			table.Values.Add(name, CreateTableValue(name, new SMLValue(newTable)));

			return newTable;
		}

		public static void AddItemWithSyntax(this SMLDocument document, string key, long value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			document.Values.Add(key, CreateTableValue(key, newValue));
		}

		public static void AddItemWithSyntax(this SMLDocument document, string key, string value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			document.Values.Add(key, CreateTableValue(key, newValue));
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, long value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			table.Values.Add(key, CreateTableValue(key, newValue));
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, string value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Add the model to the parent table model
			table.Values.Add(key, CreateTableValue(key, newValue));
		}

		private static SMLTableValue CreateTableValue(string key, SMLValue value)
		{
			return CreateTableValue(new SMLToken(key), value);
		}

		private static SMLTableValue CreateTableValue(SMLToken key, SMLValue value)
		{
			return new SMLTableValue(
				key,
				new SMLToken(":")
				{
					TrailingTrivia = new List<string>() { " " },
				},
				value);
		}
	}
}
