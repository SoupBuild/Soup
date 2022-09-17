// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	public static class SMLExtensions
	{
		public static void AddItemWithSyntax(this SMLArray array, string value)
		{
			// If this is the first item then place it on a newline
			var leadingTrivia = new List<string>();
			if (array.Values.Count == 0)
				leadingTrivia.Add("\r\n");

			// Arrays items should always force closing on newline
			var trailingTrivia = new List<string>()
			{
				"\r\n",
			};

			// Create a new item and matching syntax
			var newValue = new SMLValue(new SMLStringValue(
				value,
				new SMLToken("\"")
				{
					// Array items must have newline
					LeadingTrivia = leadingTrivia,
				},
				new SMLToken(value),
				new SMLToken("\"")
				{
					TrailingTrivia = trailingTrivia,
				}));

			// Add the model to the parent table model
			array.Values.Add(newValue);
		}

		public static SMLArray AddArrayWithSyntax(this SMLDocument document, string name)
		{
			// Document items should not be offset by default
			var leadingTrivia = new List<string>();
			var trailingTrivia = new List<string>();

			// If this is not the first item then place it on a newline
			if (document.Values.Count > 0)
				leadingTrivia.Add("\r\n");

			return document.Values.AddArrayWithSyntax(leadingTrivia, name, trailingTrivia);
		}

		public static SMLArray AddArrayWithSyntax(this SMLTable table, string name)
		{
			// If this is the first item then place it on a newline
			var leadingTrivia = new List<string>();
			if (table.Values.Count == 0)
				leadingTrivia.Add("\r\n");

			// Tables items should always force closing on newline
			var trailingTrivia = new List<string>()
			{
				"\r\n",
			};

			return table.Values.AddArrayWithSyntax(leadingTrivia, name, trailingTrivia);
		}

		public static SMLTable AddTableWithSyntax(this SMLDocument document, string name)
		{
			// Document items should not be offset
			var leadingTrivia = new List<string>();

			return document.Values.AddTableWithSyntax(name, leadingTrivia);
		}

		public static SMLTable AddTableWithSyntax(this SMLTable table, string name)
		{
			// Tables items should be on newline
			var leadingTrivia = new List<string>()
			{
				"\r\n",
			};

			return table.Values.AddTableWithSyntax(name, leadingTrivia);
		}

		public static SMLTable AddTableWithSyntax(this SMLArray array)
		{
			// If this is the first item then place it on a newline
			var leadingTrivia = new List<string>();
			if (array.Values.Count == 0)
				leadingTrivia.Add("\r\n");

			// Arrays items should always force closing on newline
			var trailingTrivia = new List<string>()
			{
				"\r\n",
			};

			var newTable = new SMLTable(
				new SMLToken("{")
				{
					LeadingTrivia = leadingTrivia,
				},
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}")
				{
					TrailingTrivia = trailingTrivia,
				});

			// Add the model to the parent table model
			array.Values.Add(new SMLValue(newTable));

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
			var newValue = new SMLValue(new SMLStringValue(
				value,
				new SMLToken("\""),
				new SMLToken(value),
				new SMLToken("\"")));

			// Add the model to the parent table model
			document.Values.Add(key, CreateTableValue(key, newValue));
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, long value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(value);

			// Tables items should be on newline
			var keyToken = new SMLToken(key)
			{
				LeadingTrivia = new List<string>()
				{
					"\r\n",
				},
			};

			// Add the model to the parent table model
			table.Values.Add(key, CreateTableValue(keyToken, newValue));
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, string value)
		{
			// If this is the first item then place it on a newline
			var leadingTrivia = new List<string>();
			if (table.Values.Count == 0)
				leadingTrivia.Add("\r\n");

			// Arrays items should always force closing on newline
			var trailingTrivia = new List<string>()
			{
				"\r\n",
			};

			// Create a new item and matching syntax
			var newValue = new SMLValue(new SMLStringValue(
				value,
				new SMLToken("\""),
				new SMLToken(value),
				new SMLToken("\"")
				{
					TrailingTrivia = trailingTrivia,
				}));

			// Tables items should be on newline
			var keyToken = new SMLToken(key)
			{
				LeadingTrivia = leadingTrivia,
			};

			// Add the model to the parent table model
			table.Values.Add(key, CreateTableValue(keyToken, newValue));
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

		public static SMLTable AddTableWithSyntax(
			this IDictionary<string, SMLTableValue> values,
			string name,
			List<string> leadingTrivia)
		{
			// Create a new table
			var newTable = new SMLTable(
				new SMLToken("{")
				{
					LeadingTrivia = leadingTrivia,
				},
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}"));

			// If this is not the first item then place it on a newline
			var keyLeadingTrivia = new List<string>();
			if (values.Count > 0)
				keyLeadingTrivia.Add("\r\n");

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = keyLeadingTrivia,
			};

			// Add the model to the parent table model
			values.Add(name, CreateTableValue(keyToken, new SMLValue(newTable)));

			return newTable;
		}

		private static SMLArray AddArrayWithSyntax(
			this IDictionary<string, SMLTableValue> values,
			List<string> leadingTrivia,
			string name,
			List<string> trailingTrivia)
		{
			var newArray = new SMLArray(
				new SMLToken("["),
				new List<SMLValue>(),
				new SMLToken("]")
				{
					TrailingTrivia = trailingTrivia,
				});

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = leadingTrivia,
			};

			// Add the model to the parent table model
			values.Add(
				name,
				CreateTableValue(keyToken, new SMLValue(newArray)));

			return newArray;
		}
	}
}
