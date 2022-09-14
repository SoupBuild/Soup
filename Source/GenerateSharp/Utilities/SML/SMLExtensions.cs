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
			////var newSyntaxValue = new StringValueSyntax(value);
			var newValue = new SMLValue(value)
			{
				// TODO: MirrorSyntax = newSyntaxTable,
			};

			// Add the model to the parent table model
			list.GetValue().Add(newValue);

			// TODO
			////newSyntaxValue.LeadingTrivia = new List<SyntaxTrivia>()
			////{
			////	new SyntaxTrivia(TokenKind.Whitespaces, "\t"),
			////};

			////// Add the new syntax to the parent table syntax
			////var arrayItemSyntax = new ArrayItemSyntax()
			////{
			////	Value = newSyntaxValue,
			////	Comma = SyntaxFactory.Token(TokenKind.Comma),
			////};

			////arrayItemSyntax.Comma.TrailingTrivia = new List<SyntaxTrivia>()
			////{
			////	SyntaxFactory.NewLineTrivia(),
			////};

			////switch (list.MirrorSyntax)
			////{
			////	case ArraySyntax arraySyntax:
			////		arraySyntax.Items.Add(arrayItemSyntax);
			////		break;
			////	default:
			////		throw new InvalidOperationException("Unknown Syntax on ValueList");
			////}
		}

		public static SMLTable AddTableWithSyntax(this SMLArray list)
		{
			// Create a new item and matching syntax
			////var newSyntaxTable = new InlineTableSyntax()
			////{
			////	OpenBrace = SyntaxFactory.Token(TokenKind.OpenBrace),
			////	CloseBrace = SyntaxFactory.Token(TokenKind.CloseBrace),
			////};
			var newTable = new SMLTable()
			{
				// TODO: MirrorSyntax = newSyntaxTable,
			};

			////newSyntaxTable.CloseBrace.AddLeadingWhitespace();

			// Add the model to the parent table model
			list.GetValue().Add(new SMLValue(newTable));

			// TODO
			////newSyntaxTable.LeadingTrivia = new List<SyntaxTrivia>()
			////{
			////	new SyntaxTrivia(TokenKind.Whitespaces, "\t"),
			////};

			////// Add the new syntax to the parent table syntax
			////var arrayItemSyntax = new ArrayItemSyntax()
			////{
			////	Value = newSyntaxTable,
			////	Comma = SyntaxFactory.Token(TokenKind.Comma),
			////};

			////arrayItemSyntax.Comma.TrailingTrivia = new List<SyntaxTrivia>()
			////{
			////	SyntaxFactory.NewLineTrivia(),
			////};

			////switch (list.MirrorSyntax)
			////{
			////	case ArraySyntax arraySyntax:
			////		arraySyntax.Items.Add(arrayItemSyntax);
			////		break;
			////	default:
			////		throw new InvalidOperationException("Unknown Syntax on ValueList");
			////}

			return newTable;
		}

		public static SMLArray AddListWithSyntax(this SMLTable table, string name)
		{
			////if (table.MirrorSyntax == null)
			////	throw new ArgumentException("Table must have syntax", nameof(table));

			// Create a new list and matching syntax
			////var newSyntaxList = new ArraySyntax()
			////{
			////	OpenBracket = SyntaxFactory.Token(TokenKind.OpenBracket),
			////	CloseBracket = SyntaxFactory.Token(TokenKind.CloseBracket),
			////};

			////newSyntaxList.OpenBracket.TrailingTrivia = new List<SyntaxTrivia>()
			////{
			////	SyntaxFactory.NewLineTrivia(),
			////};

			var newList = new SMLArray()
			{
				// TODO: MirrorSyntax = newSyntaxList,
			};

			// Add the model to the parent table model
			table.GetValue().Add(name, new SMLValue(newList));

			// TODO
			////// Add the new syntax to the parent table syntax
			////switch (table.MirrorSyntax)
			////{
			////	case DocumentSyntax documentSyntax:
			////		documentSyntax.KeyValues.Add(new KeyValueSyntax(name, newSyntaxList));
			////		break;
			////	case TableSyntaxBase tableSyntax:
			////		tableSyntax.Items.Add(new KeyValueSyntax(name, newSyntaxList));
			////		break;
			////	default:
			////		throw new InvalidOperationException($"Unknown Syntax on ValueTable {table.MirrorSyntax?.GetType()}");
			////}

			return newList;
		}

		public static SMLTable AddTableWithSyntax(this SMLTable table, string name)
		{
			// Create a new table
			var newTable = new SMLTable();

			// Add the model to the parent table model
			table.GetValue().Add(name, new SMLValue(newTable));

			// TODO: 
			// Add the new syntax to the root document syntax
			////var nameList = new List<string>();
			////nameList.Add(name);
			////var currentSyntax = table.MirrorSyntax;
			////while (currentSyntax is not DocumentSyntax)
			////{
			////	switch (currentSyntax)
			////	{
			////		case TableSyntax tableSyntax:
			////			if (tableSyntax.Name is null)
			////				throw new ArgumentNullException(nameof(tableSyntax));
			////			nameList.Add(tableSyntax.Name.ToString());
			////			currentSyntax = currentSyntax.Parent?.Parent;
			////			break;
			////		default:
			////			throw new InvalidOperationException($"Unknown Syntax on ValueTable: {currentSyntax?.GetType()}");
			////	}
			////}

			////switch (currentSyntax)
			////{
			////	case DocumentSyntax documentSyntax:
			////		// Attach the syntax to the new table
			////		var nameKey = new KeySyntax(nameList.Last());
			////		foreach (var value in nameList.SkipLast(1))
			////		{
			////			nameKey.DotKeys.Add(new DottedKeyItemSyntax(value));
			////		}

			////		var newSyntaxTable = new TableSyntax(nameKey);
			////		newTable.MirrorSyntax = newSyntaxTable;

			////		documentSyntax.Tables.Add(newSyntaxTable);
			////		break;
			////	default:
			////		throw new InvalidOperationException($"Unknown Syntax on ValueTable: {currentSyntax?.GetType()}");
			////}

			return newTable;
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, long value)
		{
			// Create a new item and matching syntax
			////var newSyntaxValue = new IntegerValueSyntax(value);
			var newValue = new SMLValue(value)
			{
				// TODO: MirrorSyntax = newSyntaxTable,
			};

			// Add the model to the parent table model
			table.GetValue().Add(key, newValue);

			// TODO
			////switch (table.MirrorSyntax)
			////{
			////	case DocumentSyntax documentSyntax:
			////		// Add the new syntax to the parent table syntax
			////		var keyValueSyntax = new KeyValueSyntax()
			////		{
			////			EqualToken = SyntaxFactory.Token(TokenKind.Equal),
			////			Key = new KeySyntax(key),
			////			Value = newSyntaxValue,
			////		};

			////		keyValueSyntax.EqualToken?.AddLeadingWhitespace();
			////		keyValueSyntax.EqualToken?.AddTrailingWhitespace();
			////		keyValueSyntax.AddTrailingTriviaNewLine();

			////		documentSyntax.KeyValues.Add(keyValueSyntax);
			////		break;
			////	case InlineTableSyntax inlineTableSyntax:
			////		// Add the new syntax to the parent table syntax
			////		var inlineTableItemSyntax = new InlineTableItemSyntax(
			////			new KeyValueSyntax()
			////			{
			////				EqualToken = SyntaxFactory.Token(TokenKind.Equal),
			////				Key = new KeySyntax(key),
			////				Value = newSyntaxValue,
			////			});

			////		inlineTableItemSyntax.AddLeadingWhitespace();
			////		inlineTableItemSyntax.KeyValue?.EqualToken?.AddLeadingWhitespace();
			////		inlineTableItemSyntax.KeyValue?.EqualToken?.AddTrailingWhitespace();

			////		// A comma can not be on the last item
			////		// Add a comma to the previous item
			////		var previousItem = inlineTableSyntax.Items.LastOrDefault();
			////		if (previousItem != null)
			////		{
			////			previousItem.Comma = SyntaxFactory.Token(TokenKind.Comma);
			////		}

			////		inlineTableSyntax.Items.Add(inlineTableItemSyntax);
			////		break;
			////	default:
			////		throw new InvalidOperationException($"Unknown Syntax on ValueList: {table.MirrorSyntax?.GetType()}");
			////}
		}

		public static void AddItemWithSyntax(this SMLTable table, string key, string value)
		{
			// Create a new item and matching syntax
			////var newSyntaxValue = new StringValueSyntax(value);
			var newValue = new SMLValue(value)
			{
				// TODO: MirrorSyntax = newSyntaxTable,
			};

			// Add the model to the parent table model
			table.GetValue().Add(key, newValue);

			// TODO
			////switch (table.MirrorSyntax)
			////{
			////	case InlineTableSyntax inlineTableSyntax:
			////		// Add the new syntax to the parent table syntax
			////		var inlineTableItemSyntax = new InlineTableItemSyntax(
			////			new KeyValueSyntax()
			////			{
			////				EqualToken = SyntaxFactory.Token(TokenKind.Equal),
			////				Key = new KeySyntax(key),
			////				Value = newSyntaxValue,
			////			});

			////		inlineTableItemSyntax.AddLeadingWhitespace();
			////		inlineTableItemSyntax.KeyValue?.EqualToken?.AddLeadingWhitespace();
			////		inlineTableItemSyntax.KeyValue?.EqualToken?.AddTrailingWhitespace();

			////		// A comma can not be on the last item
			////		// Add a comma to the previous item
			////		var previousItem = inlineTableSyntax.Items.LastOrDefault();
			////		if (previousItem != null)
			////		{
			////			previousItem.Comma = SyntaxFactory.Token(TokenKind.Comma);
			////		}

			////		inlineTableSyntax.Items.Add(inlineTableItemSyntax);
			////		break;
			////	default:
			////		throw new InvalidOperationException("Unknown Syntax on ValueList");
			////}
		}
	}
}
