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
	private readonly List<Value> impl;

	public ValueList()
	{
		this.impl = [];
	}

	public ValueList(IEnumerable<Value> collection)
	{
		this.impl = new List<Value>(collection);
	}

	public Value this[int index] => this.impl[index];

	public int Count => this.impl.Count;

	public void Add(Value item)
	{
		this.impl.Add(item);
	}

	public void Clear()
	{
		this.impl.Clear();
	}

	public ValueList Clone()
	{
		return new ValueList(this.impl);
	}

	public IEnumerator<Value> GetEnumerator()
	{
		return this.impl.GetEnumerator();
	}

	IEnumerator IEnumerable.GetEnumerator()
	{
		return this.impl.GetEnumerator();
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