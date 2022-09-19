// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public static class SMLExtensions
	{
		private static string Indent => "\t";

		public static void AddItemWithSyntax(this SMLArray array, string value, int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			// Create a new item and matching syntax
			var newValue = new SMLValue(new SMLStringValue(
				value,
				new SMLToken("\"")
				{
					// Array items must have newline
					LeadingTrivia = new List<string>()
					{
						indent,
					},
				},
				new SMLToken(value),
				new SMLToken("\"")
				{
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					},
				}));

			// Add the model to the parent table model
			array.Values.Add(newValue);
		}

		public static SMLArray AddArrayWithSyntax(this SMLDocument document, string name)
		{
			return document.Values.AddArrayWithSyntax(name, 0);
		}

		public static SMLArray AddArrayWithSyntax(this SMLTable table, string name, int indentLevel)
		{
			return table.Values.AddArrayWithSyntax(name, indentLevel);
		}

		public static SMLTable AddTableWithSyntax(this SMLDocument document, string name)
		{
			return document.Values.AddTableWithSyntax(name, 0);
		}

		public static SMLTable AddInlineTableWithSyntax(this SMLDocument document, string name)
		{
			return document.Values.AddInlineTableWithSyntax(name, 0);
		}

		public static SMLTable AddTableWithSyntax(this SMLTable table, string name, int indentLevel)
		{
			return table.Values.AddTableWithSyntax(name, indentLevel);
		}

		public static SMLTable AddInlineTableWithSyntax(this SMLTable document, string name, int indentLevel)
		{
			return document.Values.AddInlineTableWithSyntax(name, indentLevel);
		}

		public static SMLTable AddTableWithSyntax(this SMLArray array, int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			var newTable = new SMLTable(
				new SMLToken("{")
				{
					LeadingTrivia = new List<string>()
					{
						indent,
					},
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					},
				},
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}")
				{
					// Arrays items should always force closing on newline
					LeadingTrivia = new List<string>()
					{
						indent,
					},
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					},
				});

			// Add the model to the parent table model
			array.Values.Add(new SMLValue(newTable));

			return newTable;
		}

		public static void AddItemWithSyntax(this SMLDocument document, string key, long value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(new SMLIntegerValue(value));

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
			var newValue = new SMLValue(new SMLIntegerValue(value));

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

		public static void AddItemWithSyntax(this SMLTable table, string key, string value, int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			// If this is the first item then place it on a newline
			var leadingTrivia = new List<string>()
			{
				indent,
			};

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

		public static void AddInlineItemWithSyntax(this SMLTable table, string key, string value)
		{
			// If this is the first item add space, otherwise include a comma delimiter
			var leadingTrivia = new List<string>();
			if (table.Values.Count == 0)
				leadingTrivia.Add(" ");
			else
				leadingTrivia.Add(", ");

			// Arrays items should always force closing on newline
			var trailingTrivia = new List<string>();

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
			int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			// Create a new table
			var newTable = new SMLTable(
				new SMLToken("{")
				{
					// Add the newline between the items
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					}
				},
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}")
				{
					// Offset the closing brace
					LeadingTrivia = new List<string>()
					{
						indent,
					},
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					}
				});

			// If this is not the first item then place it on a newline
			var keyLeadingTrivia = new List<string>();

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = keyLeadingTrivia,
			};

			// Add the model to the parent table model
			values.Add(name, CreateTableValue(keyToken, new SMLValue(newTable)));

			return newTable;
		}

		public static SMLTable AddInlineTableWithSyntax(
			this IDictionary<string, SMLTableValue> values,
			string name,
			int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			// Create a new table
			var newTable = new SMLTable(
				new SMLToken("{"),
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}")
				{
					// Space out the inline table
					LeadingTrivia = new List<string>()
					{
						" ",
					},
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					}
				});

			// If this is not the first item then place it on a newline
			var keyLeadingTrivia = new List<string>()
			{
				indent,
			};

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
			string name,
			int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			var newArray = new SMLArray(
				new SMLToken("[")
				{
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					},
				},
				new List<SMLValue>(),
				new SMLToken("]")
				{
					LeadingTrivia = new List<string>()
					{
						indent,
					},
					TrailingTrivia = new List<string>()
					{
						"\r\n",
					},
				});

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = new List<string>()
				{
					indent,
				},
			};

			// Add the model to the parent table model
			values.Add(
				name,
				CreateTableValue(keyToken, new SMLValue(newArray)));

			return newArray;
		}
	}
}
