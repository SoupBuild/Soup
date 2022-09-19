// <copyright file="SMLValueTableVisitor.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// This class provides an implementation of <see cref="ISMLVisitor{Result}"/>,
	/// which converts the input SML into a <see cref="SMLValue"/>
	/// </summary>
	public class SMLValueTableVisitor : AbstractParseTreeVisitor<object>, ISMLVisitor<object>
	{
		private CommonTokenStream _tokens;

		public SMLValueTableVisitor(CommonTokenStream tokens)
		{
			_tokens = tokens;
		}

		public virtual object VisitDocument(SMLParser.DocumentContext context)
		{
			// TODO: Handle leading trivia before first item...

			var leadingNewlines = (List<SMLToken>)context.leadingNewlines().Accept(this);
			var tableContent = (Dictionary<string, SMLTableValue>)context.tableContent().Accept(this);
			var trailingNewlines = (List<SMLToken>)context.trailingNewlines().Accept(this);
			return new SMLDocument(
				leadingNewlines,
				tableContent,
				trailingNewlines); ;
		}

		public virtual object VisitTable(SMLParser.TableContext context)
		{
			var leadingNewlines = (List<SMLToken>)context.leadingNewlines().Accept(this);
			var tableContent = (Dictionary<string, SMLTableValue>)context.tableContent().Accept(this);
			var trailingNewlines = (List<SMLToken>)context.trailingNewlines().Accept(this);
			return new SMLTable(
				new SMLToken(context.OPEN_BRACE().GetText()),
				leadingNewlines,
				tableContent,
				trailingNewlines,
				new SMLToken(context.CLOSE_BRACE().GetText()));
		}

		public virtual object VisitTableContent(SMLParser.TableContentContext context)
		{
			var tableContent = new Dictionary<string, SMLTableValue>();
			var tableValues = context.tableValue();
			var delimiters = context.delimiter();
			for (var i = 0; i < tableValues.Length; i++)
			{
				var value = tableValues[i];
				var tableValue = (SMLTableValue)value.Accept(this);

				// Check for optional demimilter
				if (delimiters.Length > i)
				{
					var delimiter = (List<SMLToken>)delimiters[i].Accept(this);
					tableValue.Delimiter = delimiter;
				}

				tableContent.Add(tableValue.Key.Text, tableValue);
			}

			return tableContent;
		}

		public virtual object VisitTableValue(SMLParser.TableValueContext context)
		{
			var key = BuildToken(context.KEY());
			var colon = BuildToken(context.COLON());
			var values = (SMLValue)context.value().Accept(this);
			return new SMLTableValue(key, colon, values, new List<SMLToken>());
		}

		public virtual object VisitArray(SMLParser.ArrayContext context)
		{
			var leadingNewlines = (List<SMLToken>)context.leadingNewlines().Accept(this);
			var arrayContent = (List<SMLValue>)context.arrayContent().Accept(this);
			var trailingNewlines = (List<SMLToken>)context.trailingNewlines().Accept(this);
			return new SMLArray(
				BuildToken(context.OPEN_BRACKET()),
				leadingNewlines,
				arrayContent,
				trailingNewlines,
				BuildToken(context.CLOSE_BRACKET()));
		}

		public virtual object VisitArrayContent(SMLParser.ArrayContentContext context)
		{
			var arrayContent = new List<SMLValue>();
			foreach (var value in context.value())
			{
				arrayContent.Add((SMLValue)value.Accept(this));
			}

			return arrayContent;
		}

		public virtual object VisitValueInteger(SMLParser.ValueIntegerContext context)
		{
			var integerNode = context.INTEGER();
			var value = long.Parse(integerNode.Symbol.Text);
			return new SMLValue(
				new SMLIntegerValue(
					value,
					BuildToken(integerNode)));
		}

		public virtual object VisitValueString(SMLParser.ValueStringContext context)
		{
			var literal = context.STRING_LITERAL().Symbol.Text;
			var content = literal.Substring(1, literal.Length - 2);

			return new SMLValue(new SMLStringValue(
				content,
				new SMLToken("\""),
				new SMLToken(content),
				new SMLToken("\"")
				{
					TrailingTrivia = GetTrailingTrivia(context.STRING_LITERAL()),
				}));
		}

		public virtual object VisitValueTrue(SMLParser.ValueTrueContext context)
		{
			return new SMLValue(new SMLBooleanValue(
				true,
				BuildToken(context.TRUE())));
		}

		public virtual object VisitValueFalse(SMLParser.ValueFalseContext context)
		{
			return new SMLValue(
				new SMLBooleanValue(false,
				BuildToken(context.FALSE())));
		}

		public virtual object VisitValueTable(SMLParser.ValueTableContext context)
		{
			var table = (SMLTable)context.table().Accept(this);
			return new SMLValue(table);
		}

		public virtual object VisitValueArray(SMLParser.ValueArrayContext context)
		{
			var array = (SMLArray)context.array().Accept(this);
			return new SMLValue(array);
		}

		public virtual object VisitNewlineDelimiter(SMLParser.NewlineDelimiterContext context)
		{
			return context.NEWLINE().Select(value => BuildToken(value)).ToList();
		}

		public virtual object VisitLeadingNewlines(SMLParser.LeadingNewlinesContext context)
		{
			return context.NEWLINE().Select(value => BuildToken(value)).ToList();
		}

		public virtual object VisitTrailingNewlines(SMLParser.TrailingNewlinesContext context)
		{
			return context.NEWLINE().Select(value => BuildToken(value)).ToList();
		}

		public virtual object VisitCommaDelimiter(SMLParser.CommaDelimiterContext context)
		{
			return new List<SMLToken>()
			{
				BuildToken(context.COMMA()),
			};
		}

		private SMLToken BuildToken(ITerminalNode node)
		{
			var leadingTrivia = new List<string>();
			var trailingTrivia = GetTrailingTrivia(node);
			return new SMLToken(
				leadingTrivia,
				node.Symbol.Text,
				trailingTrivia);
		}

		private List<string> GetLeadingTrivia(ITerminalNode node)
		{
			var left = _tokens.GetHiddenTokensToLeft(node.Symbol.TokenIndex);
			var leadingTrivia = left != null ? left.Select(value => value.Text).ToList() : new List<string>();
			return leadingTrivia;
		}

		private List<string> GetTrailingTrivia(ITerminalNode node)
		{
			var right = _tokens.GetHiddenTokensToRight(node.Symbol.TokenIndex);
			var trailingTrivia = right != null ? right.Select(value => value.Text).ToList() : new List<string>();
			return trailingTrivia;
		}
	}
}