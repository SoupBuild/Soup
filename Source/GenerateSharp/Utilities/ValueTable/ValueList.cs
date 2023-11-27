// <copyright file="ValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace Soup.Build.Utilities;

[SuppressMessage("Naming", "CA1710:Identifiers should have correct suffix", Justification = "Disagree")]
public class ValueList : IEnumerable, IEnumerable<Value>
{
	private readonly List<Value> _impl;

	public ValueList()
	{
		_impl = [];
	}

	public ValueList(IEnumerable<Value> collection)
	{
		_impl = new List<Value>(collection);
	}

	public Value this[int index] => this._impl[index];

	public int Count => this._impl.Count;

	public void Add(Value item)
	{
		this._impl.Add(item);
	}

	public void Clear()
	{
		this._impl.Clear();
	}

	public ValueList Clone()
	{
		return new ValueList(_impl);
	}

	public IEnumerator<Value> GetEnumerator()
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

		_ = builder.Append("[ ");
		foreach (var value in this)
		{
			_ = builder.Append(value);
			_ = builder.Append(", ");
		}

		_ = builder.Append(']');

		return builder.ToString();
	}
}