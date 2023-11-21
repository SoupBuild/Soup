// <copyright file="DictionaryComparer.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;

namespace Soup.Build.Discover.UnitTests;

public class DictionaryOfListsComparer<T> : IEqualityComparer<IDictionary<string, IList<T>>>
{
	private readonly IEqualityComparer<T> valueComparer;

	public DictionaryOfListsComparer(IEqualityComparer<T>? valueComparer = null)
	{
		this.valueComparer = valueComparer ?? EqualityComparer<T>.Default;
	}

	public bool Equals(IDictionary<string, IList<T>>? x, IDictionary<string, IList<T>>? y)
	{
		if (x is null || y is null)
			return x is null && y is null;

		if (x.Count != y.Count)
			return false;

		if (x.Keys.Except(y.Keys).Any())
			return false;

		foreach (var pair in x)
		{
			var xValue = pair.Value;
			var yValue = y[pair.Key];

			if (!xValue.SequenceEqual(yValue, this.valueComparer))
				return false;
		}

		return true;
	}

	public int GetHashCode([DisallowNull] IDictionary<string, IList<T>> obj)
	{
		unchecked
		{
			// Hash the keys for a close approximation
			var hash = 445566;
			foreach (var key in obj.Keys)
			{
				hash = hash * 123 + key.GetHashCode();
			}

			return hash;
		}
	}
}