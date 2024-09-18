// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace Soup.Build.Utilities;

public static partial class SMLExtensions
{
	private static readonly SMLToken CommaToken = new SMLToken(",");

	private static readonly SMLToken NewlineToken = new SMLToken("\r\n");

	private const string Indent = "\t";

	public static void AddItemWithSyntax(this SMLArray array, string value, int indentLevel)
	{
		var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

		// Create a new item and matching syntax
		var newValue = new SMLArrayValue(new SMLValue(new SMLStringValue(
			value,
			new SMLToken("'")
			{
				// Array items must have newline
				LeadingTrivia =
				[
					indent,
				],
			},
			new SMLToken(value),
			new SMLToken("'"))),
			[]);

		// Update the previous last item to have a comma delimiter
		if (array.Values.Count > 0)
		{
			var lastItem = array.Values.Last();
			lastItem.Delimiter.Add(NewlineToken);
		}

		// Add the model to the parent table model
		array.Values.Add(newValue);
	}

	public static SMLArray AddArrayWithSyntax(this SMLDocument document, string name)
	{
		return document.Values.AddArrayWithSyntax(name, 0);
	}

	public static SMLArray EnsureArrayWithSyntax(this SMLTable table, string name, int indentLevel)
	{
		if (table.Values.TryGetValue(name, out var tableValue))
		{
			return tableValue.Value.AsArray();
		}
		else
		{
			return table.AddArrayWithSyntax(name, indentLevel);
		}
	}

	public static SMLArray AddArrayWithSyntax(this SMLTable table, string name, int indentLevel)
	{
		return table.Values.AddArrayWithSyntax(name, indentLevel);
	}

	public static SMLTable EnsureTableWithSyntax(this SMLDocument document, string name)
	{
		if (document.Values.TryGetValue(name, out var tableValue))
		{
			return tableValue.Value.AsTable();
		}
		else
		{
			return document.AddTableWithSyntax(name);
		}
	}

	public static SMLTable AddTableWithSyntax(this SMLDocument document, string name)
	{
		return document.Values.AddTableWithSyntax(name, 0);
	}

	public static SMLTable AddInlineTableWithSyntax(this SMLDocument document, string name)
	{
		return document.Values.AddInlineTableWithSyntax(name, 0);
	}

	public static SMLTable EnsureTableWithSyntax(this SMLTable table, string name, int indentLevel)
	{
		if (table.Values.TryGetValue(name, out var tableValue))
		{
			return tableValue.Value.AsTable();
		}
		else
		{
			return table.AddTableWithSyntax(name, indentLevel);
		}
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

		var newTable = CreateInlineTable(
		[
			indent,
		]);

		var newItem = new SMLArrayValue(
			new SMLValue(newTable),
			[]);

		// Update the previous last item to have a comma delimiter
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
				LeadingTrivia =
				[
					indent,
				],
			},
			[
				NewlineToken
			],
			[],
			[
					NewlineToken
			],
			new SMLToken("}")
			{
				// Arrays items should always force closing on newline
				LeadingTrivia =
				[
					indent,
				],
			});
		var newItem = new SMLArrayValue(
			new SMLValue(newTable),
			[]);

		// Update the previous last item to have a comma delimiter
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
			new SMLToken("'"),
			new SMLToken(value),
			new SMLToken("'")));

		// Update the previous last item to have a comma delimiter
		if (document.Values.Count > 0)
		{
			var lastItem = document.Values.Last();
			lastItem.Value.Delimiter.Add(NewlineToken);
		}

		// Add the model to the parent table model
		document.Values.Add(key, CreateTableValue(key, newValue));
	}

	public static void AddItemWithSyntax(this SMLTable table, string key, long value)
	{
		// Create a new item and matching syntax
		var newValue = new SMLValue(new SMLIntegerValue(value));

		// Tables items should be on newline
		var keyToken = new SMLToken(EnsureSafeKey(key));

		// Add the model to the parent table model
		table.Values.Add(key, CreateTableValue(keyToken, newValue));
	}

	public static void AddItemWithSyntax(this SMLTable table, string key, string value)
	{
		// Create a new item and matching syntax
		var newValue = new SMLValue(new SMLStringValue(value));

		// Tables items should be on newline
		var keyToken = new SMLToken(EnsureSafeKey(key));

		// Add the model to the parent table model
		table.Values.Add(key, CreateTableValue(keyToken, newValue));
	}

	public static void AddItemWithSyntax(this SMLTable table, string key, string value, int indentLevel)
	{
		var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

		// If this is the first item then place it on a newline
		List<string> leadingTrivia =
		[
			indent,
		];

		// Create a new item and matching syntax
		var newValue = new SMLValue(new SMLStringValue(
			value,
			new SMLToken("'"),
			new SMLToken(value),
			new SMLToken("'")));

		// Tables items should be on newline
		var keyToken = new SMLToken(EnsureSafeKey(key))
		{
			LeadingTrivia = leadingTrivia,
		};

		// Update the previous last item to have a comma delimiter
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
			new SMLToken("'"),
			new SMLToken(value),
			new SMLToken("'")));

		// Tables items should be on newline
		var keyToken = new SMLToken(EnsureSafeKey(key))
		{
			LeadingTrivia =
			[
				" ",
			],
		};

		// Update the previous last item to have a comma delimiter
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
		return CreateTableValue(new SMLToken(EnsureSafeKey(key)), value);
	}

	private static SMLTableValue CreateInlineTableValue(SMLToken key, SMLValue value)
	{
		// The comma delimiter will be added when needed
		return new SMLTableValue(
			key,
			key.Text,
			new SMLToken(":")
			{
				TrailingTrivia = [" "],
			},
			value,
			[]);
	}

	private static SMLTableValue CreateTableValue(SMLToken key, SMLValue value)
	{
		return new SMLTableValue(
			key,
			key.Text,
			new SMLToken(":")
			{
				TrailingTrivia = [" "],
			},
			value,
			[]);
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
			[
				NewlineToken,
			],
			[],
			[
				NewlineToken,
			],
			new SMLToken("}")
			{
				// Offset the closing brace
				LeadingTrivia =
				[
					indent,
				],
			});

		var keyToken = new SMLToken(EnsureSafeKey(name))
		{
			LeadingTrivia =
			[
				indent,
			],
		};

		// Update the previous last item to have a comma delimiter
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
			[],
			new SMLToken("}")
			{
				// Space out the inline table
				LeadingTrivia =
				[
					" ",
				],
			});
		return newTable;
	}

	private static SMLTable AddInlineTableWithSyntax(
		this Dictionary<string, SMLTableValue> values,
		string name,
		int indentLevel)
	{
		var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

		// Create a new table
		var newTable = CreateInlineTable([]);

		var keyToken = new SMLToken(EnsureSafeKey(name))
		{
			LeadingTrivia =
			[
				indent,
			],
		};

		// Update the previous last item to have a comma delimiter
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
		this Dictionary<string, SMLTableValue> values,
		string name,
		int indentLevel)
	{
		var indent = string.Concat(Enumerable.Repeat(Indent, indentLevel));

		var newArray = new SMLArray(
			new SMLToken("["),
			[
				NewlineToken,
			],
			[],
			[
				NewlineToken,
			],
			new SMLToken("]")
			{
				LeadingTrivia =
				[
					indent,
				],
			});

		var keyToken = new SMLToken(EnsureSafeKey(name))
		{
			LeadingTrivia =
			[
				indent,
			],
		};

		// Update the previous last item to have a comma delimiter
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

	private static string EnsureSafeKey(string name)
	{
		// If the key contains unsafe characters, wrap in string key
		var keyTokenText = name;
		if (!SafeKeyRegex().IsMatch(keyTokenText))
			keyTokenText = $"'{keyTokenText}'";

		return keyTokenText;
	}

	[GeneratedRegex(@"^([A-Za-z0-9]+)$")]
	private static partial Regex SafeKeyRegex();
}