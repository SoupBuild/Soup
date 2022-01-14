// <copyright file="ValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System;
using System.Collections.Generic;
using Tomlyn.Syntax;

namespace Soup.Build.Utilities
{
	public static class SyntaxUtilsValueTableExtensions
	{
		public static void AddItemWithSyntax(this ValueList list, string value)
		{
			// Create a new item and matching syntax
			var newSyntaxValue = new StringValueSyntax(value);
			var newValue = new Value(value)
			{
				// TODO: MirrorSyntax = newSyntaxTable,
			};

			// Add the model to the parent table model
			list.Add(newValue);

			newSyntaxValue.LeadingTrivia = new List<SyntaxTrivia>()
			{
				new SyntaxTrivia(TokenKind.Whitespaces, "\t"),
			};

			// Add the new syntax to the parent table syntax
			var arrayItemSyntax = new ArrayItemSyntax()
			{
				Value = newSyntaxValue,
				Comma = SyntaxFactory.Token(TokenKind.Comma),
			};

			arrayItemSyntax.Comma.TrailingTrivia = new List<SyntaxTrivia>()
			{
				SyntaxFactory.NewLineTrivia(),
			};

			switch (list.MirrorSyntax)
			{
				case ArraySyntax arraySyntax:
					arraySyntax.Items.Add(arrayItemSyntax);
					break;
				default:
					throw new InvalidOperationException("Unknown Syntax on ValueList");
			}
		}

		public static ValueList AddListWithSyntax(this ValueTable table, string name)
		{
			if (table.MirrorSyntax == null)
				throw new ArgumentException("Table must have syntax", nameof(table));

			// Create a new list and matching syntax
			var newSyntaxList = new ArraySyntax()
			{
				OpenBracket = SyntaxFactory.Token(TokenKind.OpenBracket),
				CloseBracket = SyntaxFactory.Token(TokenKind.CloseBracket),
			};

			newSyntaxList.OpenBracket.TrailingTrivia = new List<SyntaxTrivia>()
			{
				SyntaxFactory.NewLineTrivia(),
			};

			var newList = new ValueList()
			{
				MirrorSyntax = newSyntaxList,
			};

			// Add the model to the parent table model
			table.Add(name, new Value(newList));

			// Add the new syntax to the parent table syntax
			switch (table.MirrorSyntax)
			{
				case DocumentSyntax documentSyntax:
					documentSyntax.KeyValues.Add(new KeyValueSyntax(name, newSyntaxList));
					break;
				case TableSyntaxBase tableSyntax:
					tableSyntax.Items.Add(new KeyValueSyntax(name, newSyntaxList));
					break;
				default:
					throw new InvalidOperationException($"Unknown Syntax on ValueTable {table.MirrorSyntax?.GetType()}");
			}

			return newList;
		}

		public static ValueTable AddTableWithSyntax(this ValueTable table, string name)
		{
			// Create a new table and matching syntax
			var newSyntaxTable = new TableSyntax(name);
			var newTable = new ValueTable()
			{
				MirrorSyntax = newSyntaxTable,
			};

			// Add the model to the parent table model
			table.Add(name, new Value(newTable));

			// Add the new syntax to the parent table syntax
			switch (table.MirrorSyntax)
			{
				case DocumentSyntax documentSyntax:
					documentSyntax.Tables.Add(newSyntaxTable);
					break;
				default:
					throw new InvalidOperationException("Unknown Syntax on ValueTable");
			}

			return newTable;
		}

	}
}
