// <copyright file="ValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections;
using System.Collections.Generic;
using System.Text;
using Tomlyn.Syntax;

namespace Soup.Build.Runtime
{
	public class ValueList : IValueList
	{
		private List<IValue> _impl;

		public SyntaxNode? MirrorSyntax { get; set; }

		private ValueList(IEnumerable<IValue> collection, SyntaxNode? mirrorSyntax)
		{
			_impl = new List<IValue>(collection);
			MirrorSyntax = mirrorSyntax;
		}

		public ValueList()
		{
			_impl = new List<IValue>();
			MirrorSyntax = null;
		}

		public ValueList(IEnumerable<IValue> collection)
		{
			_impl = new List<IValue>(collection);
		}

		public IValue this[int index] => this._impl[index];

		public int Count => this._impl.Count;

		public void Add(IValue item)
		{
			this._impl.Add(item);
		}

		public void Clear()
		{
			this._impl.Clear();
		}

		public ValueList Clone()
		{
			return new ValueList(_impl, MirrorSyntax);
		}

		public IEnumerator<IValue> GetEnumerator()
		{
			return this._impl.GetEnumerator();
		}

		IEnumerator IEnumerable.GetEnumerator()
		{
			return this._impl.GetEnumerator();
		}

		public override string ToString()
		{
			var builder = new StringBuilder();

			builder.Append("[ ");
			foreach (var value in this)
			{
				builder.Append(value);
				builder.Append(", ");
			}

			builder.Append("]");

			return builder.ToString();
		}
	}
}
