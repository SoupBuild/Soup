// <copyright file="SMLValueTableVisitor.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime.Tree;
using Soup.Build.Runtime;
using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// This class provides an implementation of <see cref="ISMLVisitor{Result}"/>,
	/// which converts the input SML into a <see cref="Value"/>
	/// </summary>
	public class SMLValueTableVisitor : AbstractParseTreeVisitor<Value>, ISMLVisitor<Value>
	{
		public virtual Value VisitDocument(SMLParser.DocumentContext context)
		{
			return context.table_content().Accept(this);
		}

		public virtual Value VisitTable_content(SMLParser.Table_contentContext context)
		{
			var tableContent = new Dictionary<string, IValue>();
			foreach (var value in context.assign_value())
			{
				tableContent.Add(value.KEY().GetText(), value.value().Accept(this));
			}

			return new Value(new ValueTable(tableContent, null));
		}

		public virtual Value VisitArray_content(SMLParser.Array_contentContext context)
		{
			var arrayContent = new List<Value>();
			foreach (var value in context.value())
			{
				arrayContent.Add(value.Accept(this));
			}

			return new Value(new ValueList(arrayContent));
		}

		public virtual Value VisitAssign_value(SMLParser.Assign_valueContext context)
		{
			throw new NotSupportedException();
		}

		public virtual Value VisitValueInteger(SMLParser.ValueIntegerContext context)
		{
			return new Value(long.Parse(context.INTEGER().GetText()));
		}

		public virtual Value VisitValueString(SMLParser.ValueStringContext context)
		{
			var literal = context.STRING_LITERAL().GetText();
			var content = literal.Substring(1, literal.Length - 2);
			return new Value(content);
		}

		public virtual Value VisitValueTable(SMLParser.ValueTableContext context)
		{
			return context.table().Accept(this);
		}

		public virtual Value VisitValueArray(SMLParser.ValueArrayContext context)
		{
			return context.array().Accept(this);
		}

		public virtual Value VisitTable(SMLParser.TableContext context)
		{
			return context.table_content().Accept(this);
		}

		public virtual Value VisitArray(SMLParser.ArrayContext context)
		{
			return context.array_content().Accept(this);
		}
	}
}