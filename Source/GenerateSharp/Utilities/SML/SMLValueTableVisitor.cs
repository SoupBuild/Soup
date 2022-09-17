// <copyright file="SMLValueTableVisitor.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime.Tree;
using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// This class provides an implementation of <see cref="ISMLVisitor{Result}"/>,
	/// which converts the input SML into a <see cref="SMLValue"/>
	/// </summary>
	public class SMLValueTableVisitor : AbstractParseTreeVisitor<object>, ISMLVisitor<object>
	{
		public virtual object VisitDocument(SMLParser.DocumentContext context)
		{
			var tableContent = (Dictionary<string, SMLTableValue>)context.tableContent().Accept(this);
			return new SMLDocument(tableContent);
		}

		public virtual object VisitTable(SMLParser.TableContext context)
		{
			var tableContent = (Dictionary<string, SMLTableValue>)context.tableContent().Accept(this);
			return new SMLTable(tableContent);
		}

		public virtual object VisitTableContent(SMLParser.TableContentContext context)
		{
			var tableContent = new Dictionary<string, SMLTableValue>();
			foreach (var value in context.tableValue())
			{
				var tableValue = (SMLTableValue)value.Accept(this);
				tableContent.Add(tableValue.Key.Text, tableValue);
			}

			return tableContent;
		}

		public virtual object VisitTableValue(SMLParser.TableValueContext context)
		{
			return new SMLTableValue(
				new SMLToken(context.KEY().GetText()),
				(SMLValue)context.value().Accept(this));
		}

		public virtual object VisitArray(SMLParser.ArrayContext context)
		{
			var arrayContent = (List<SMLValue>)context.arrayContent().Accept(this);
			return new SMLArray(arrayContent);
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
			return new SMLValue(long.Parse(context.INTEGER().GetText()));
		}

		public virtual object VisitValueString(SMLParser.ValueStringContext context)
		{
			var literal = context.STRING_LITERAL().GetText();
			var content = literal.Substring(1, literal.Length - 2);
			return new SMLValue(content);
		}

		public virtual object VisitValueTrue(SMLParser.ValueTrueContext context)
		{
			return new SMLValue(true);
		}

		public virtual object VisitValueFalse(SMLParser.ValueFalseContext context)
		{
			return new SMLValue(false);
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

		public virtual object VisitDelimiter(SMLParser.DelimiterContext context)
		{
			throw new NotImplementedException();
		}
	}
}