// <copyright file="SMLValueTableVisitor.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime.Tree;
using Soup.Build.Runtime;
using System;
using System.Collections.Generic;
using System.Reflection.Metadata;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// This class provides an implementation of <see cref="ISMLVisitor{Result}"/>,
	/// which converts the input SML into a <see cref="Value"/>
	/// </summary>
	public class SMLValueTableVisitor : AbstractParseTreeVisitor<SMLValue>, ISMLVisitor<SMLValue>
	{
		public virtual SMLValue VisitDocument(SMLParser.DocumentContext context)
		{
			return context.tableContent().Accept(this);
		}

		public virtual SMLValue VisitTable(SMLParser.TableContext context)
		{
			return context.tableContent().Accept(this);
		}

		public virtual SMLValue VisitTableContent(SMLParser.TableContentContext context)
		{
			var tableContent = new Dictionary<string, SMLValue>();
			foreach (var value in context.tableValue())
			{
				tableContent.Add(value.KEY().GetText(), value.value().Accept(this));
			}

			return new SMLValue(new SMLTable(tableContent));
		}

		public virtual SMLValue VisitTableValue(SMLParser.TableValueContext context)
		{
			throw new NotImplementedException();
		}

		public virtual SMLValue VisitArray(SMLParser.ArrayContext context)
		{
			return context.arrayContent().Accept(this);
		}

		public virtual SMLValue VisitArrayContent(SMLParser.ArrayContentContext context)
		{
			var arrayContent = new List<SMLValue>();
			foreach (var value in context.arrayValue())
			{
				arrayContent.Add(value.Accept(this));
			}

			return new SMLValue(new SMLArray(arrayContent));
		}

		public virtual SMLValue VisitArrayValue(SMLParser.ArrayValueContext context)
		{
			return context.value().Accept(this);
		}

		public virtual SMLValue VisitValueInteger(SMLParser.ValueIntegerContext context)
		{
			return new SMLValue(long.Parse(context.INTEGER().GetText()));
		}

		public virtual SMLValue VisitValueString(SMLParser.ValueStringContext context)
		{
			var literal = context.STRING_LITERAL().GetText();
			var content = literal.Substring(1, literal.Length - 2);
			return new SMLValue(content);
		}

		public virtual SMLValue VisitValueTrue(SMLParser.ValueTrueContext context)
		{
			return new SMLValue(true);
		}

		public virtual SMLValue VisitValueFalse(SMLParser.ValueFalseContext context)
		{
			return new SMLValue(false);
		}

		public virtual SMLValue VisitValueTable(SMLParser.ValueTableContext context)
		{
			return context.table().Accept(this);
		}

		public virtual SMLValue VisitValueArray(SMLParser.ValueArrayContext context)
		{
			return context.array().Accept(this);
		}

		public virtual SMLValue VisitDelimiter(SMLParser.DelimiterContext context)
		{
			throw new NotImplementedException();
		}
	}
}