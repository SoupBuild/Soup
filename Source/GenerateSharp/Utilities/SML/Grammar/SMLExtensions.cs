// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public static class SMLExtensions
	{
		private static SMLToken CommaToken => new SMLToken(",");

		private static SMLToken NewlineToken => new SMLToken("\r\n");

		private static string Indent => "\t";

		public static void AddItemWithSyntax(this SMLArray array, string value, int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			// Create a new item and matching syntax
			var newValue = new SMLArrayValue(new SMLValue(new SMLStringValue(
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
				new SMLToken("\""))),
				new List<SMLToken>());

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

		public static SMLTable AddInlineTableWithSyntax(this SMLArray array, int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			var newTable = CreateInlineTable(new List<string>()
			{
				indent,
			});

			var newItem = new SMLArrayValue(
				new SMLValue(newTable),
				new List<SMLToken>());

			// Update the previous last item to have a comma delmiter
			if (array.Values.Count > 0)
			{
				var lastItem = array.Values.Last();
				lastItem.Delimiter.Add(NewlineToken);
			}

			// Add the model to the parent table model
			array.Values.Add(newItem);

			return newTable;
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
				},
				new List<SMLToken>()
				{
					NewlineToken
				},
				new Dictionary<string, SMLTableValue>(),
				new List<SMLToken>()
				{
					NewlineToken
				},
				new SMLToken("}")
				{
					// Arrays items should always force closing on newline
					LeadingTrivia = new List<string>()
					{
						indent,
					},
				});
			var newItem = new SMLArrayValue(
				new SMLValue(newTable),
				new List<SMLToken>());

			// Update the previous last item to have a comma delmiter
			if (array.Values.Count > 0)
			{
				var lastItem = array.Values.Last();
				lastItem.Delimiter.Add(NewlineToken);
			}

			// Add the model to the parent table model
			array.Values.Add(newItem);

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
			var keyToken = new SMLToken(key); ;

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

			// Create a new item and matching syntax
			var newValue = new SMLValue(new SMLStringValue(
				value,
				new SMLToken("\""),
				new SMLToken(value),
				new SMLToken("\"")));

			// Tables items should be on newline
			var keyToken = new SMLToken(key)
			{
				LeadingTrivia = leadingTrivia,
			};

			// Update the previous last item to have a comma delmiter
			if (table.Values.Count > 0)
			{
				var lastItem = table.Values.Last();
				lastItem.Value.Delimiter.Add(NewlineToken);
			}

			// Add the model to the parent table model
			table.Values.Add(key, CreateTableValue(keyToken, newValue));
		}

		public static void AddInlineItemWithSyntax(this SMLTable table, string key, string value)
		{
			// Create a new item and matching syntax
			var newValue = new SMLValue(new SMLStringValue(
				value,
				new SMLToken("\""),
				new SMLToken(value),
				new SMLToken("\"")));

			// Tables items should be on newline
			var keyToken = new SMLToken(key)
			{
				LeadingTrivia = new List<string>()
				{
					" ",
				},
			};

			// Update the previous last item to have a comma delmiter
			if (table.Values.Count > 0)
			{
				var lastItem = table.Values.Last();
				lastItem.Value.Delimiter.Add(CommaToken);
			}

			// Add the model to the parent table model
			table.Values.Add(key, CreateInlineTableValue(keyToken, newValue));
		}

		private static SMLTableValue CreateTableValue(string key, SMLValue value)
		{
			return CreateTableValue(new SMLToken(key), value);
		}

		private static SMLTableValue CreateInlineTableValue(SMLToken key, SMLValue value)
		{
			// The comma delimiter will be added when needed
			return new SMLTableValue(
				key,
				new SMLToken(":")
				{
					TrailingTrivia = new List<string>() { " " },
				},
				value,
				new List<SMLToken>());
		}

		private static SMLTableValue CreateTableValue(SMLToken key, SMLValue value)
		{
			return new SMLTableValue(
				key,
				new SMLToken(":")
				{
					TrailingTrivia = new List<string>() { " " },
				},
				value,
				new List<SMLToken>());
		}

		public static SMLTable AddTableWithSyntax(
			this IDictionary<string, SMLTableValue> values,
			string name,
			int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			// Create a new table
			var newTable = new SMLTable(
				new SMLToken("{"),
				new List<SMLToken>()
				{
					NewlineToken,
				},
				new Dictionary<string, SMLTableValue>(),
				new List<SMLToken>()
				{
					NewlineToken,
				},
				new SMLToken("}")
				{
					// Offset the closing brace
					LeadingTrivia = new List<string>()
					{
						indent,
					},
				});

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = new List<string>()
				{
					indent,
				},
			};

			// Update the previous last item to have a comma delmiter
			if (values.Count > 0)
			{
				var lastItem = values.Last();
				lastItem.Value.Delimiter.Add(NewlineToken);
			}

			// Add the model to the parent table model
			values.Add(name, CreateTableValue(keyToken, new SMLValue(newTable)));

			return newTable;
		}

		private static SMLTable CreateInlineTable(List<string> leadingTrivia)
		{
			var newTable = new SMLTable(
				new SMLToken("{")
				{
					LeadingTrivia = leadingTrivia,
				},
				new Dictionary<string, SMLTableValue>(),
				new SMLToken("}")
				{
					// Space out the inline table
					LeadingTrivia = new List<string>()
					{
						" ",
					},
				});
			return newTable;
		}

		private static SMLTable AddInlineTableWithSyntax(
			this IDictionary<string, SMLTableValue> values,
			string name,
			int indentLevel)
		{
			var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

			// Create a new table
			var newTable = CreateInlineTable(new List<string>());

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = new List<string>()
				{
					indent,
				},
			};

			// Update the previous last item to have a comma delmiter
			if (values.Count > 0)
			{
				var lastItem = values.Last();
				lastItem.Value.Delimiter.Add(NewlineToken);
			}

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
				new SMLToken("["),
				new List<SMLToken>()
				{
					NewlineToken,
				},
				new List<SMLArrayValue>(),
				new List<SMLToken>()
				{
					NewlineToken,
				},
				new SMLToken("]")
				{
					LeadingTrivia = new List<string>()
					{
						indent,
					},
				});

			var keyToken = new SMLToken(name)
			{
				LeadingTrivia = new List<string>()
				{
					indent,
				},
			};

			// Update the previous last item to have a comma delmiter
			if (values.Count > 0)
			{
				var lastItem = values.Last();
				lastItem.Value.Delimiter.Add(NewlineToken);
			}

			// Add the model to the parent table model
			values.Add(
				name,
				CreateTableValue(keyToken, new SMLValue(newArray)));

			return newArray;
		}
	}
}
