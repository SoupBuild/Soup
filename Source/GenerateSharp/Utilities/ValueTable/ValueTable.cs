// <copyright file="ValueTable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Text;

namespace Soup.Build.Utilities;

[SuppressMessage("Naming", "CA1710:Identifiers should have correct suffix", Justification = "Disagree")]
public class ValueTable : IEnumerable, IEnumerable<KeyValuePair<string, Value>>
{
	private readonly Dictionary<string, Value> _impl;

	public ValueTable(IDictionary<string, Value> values)
	{
		_impl = new Dictionary<string, Value>(values);
	}

	public ValueTable()
	{
		_impl = [];
	}

	public Value this[string key]
	{
		get => this._impl[key];
		set => this._impl[key] = value;
	}

	public IEnumerable<string> Keys => this._impl.Keys;

	public IEnumerable<Value> Values => this._impl.Values;

	public int Count => this._impl.Count;

	public void Add(string key, Value value)
	{
		this._impl.Add(key, value);
	}

	public bool ContainsKey(string key)
	{
		return this._impl.ContainsKey(key);
	}

	public ValueTable Clone()
	{
		return new ValueTable(_impl);
	}

	public IEnumerator<KeyValuePair<string, Value>> GetEnumerator()
	{
		return this._impl.GetEnumerator();
	}

	public bool Remove(string key)
	{
		return this._impl.Remove(key);
	}

	public bool TryGetValue(string key, [MaybeNullWhen(false)] out Value value)
	{
		var result = this._impl.TryGetValue(key, out var valueImpl);
		value = valueImpl;
		return result;
	}

	IEnumerator IEnumerable.GetEnumerator()
	{
		return this._impl.GetEnumerator();
	}

	public override string ToString()
	{
		var builder = new StringBuilder();

		_ = builder.Append("{ ");
		foreach (var value in this)
		{
			_ = builder.Append('\"');
			_ = builder.Append(value.Key);
			_ = builder.Append("\": ");
			_ = builder.Append(value.Value);
			_ = builder.Append(", ");
		}

		_ = builder.Append('}');

		return builder.ToString();
	}
}