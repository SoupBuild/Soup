// <copyright file="SMLDocument.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Utilities;

public class SMLDocument : IEquatable<SMLDocument>
{
	public IList<SMLToken> LeadingNewlines { get; init; }

	public Dictionary<string, SMLTableValue> Values { get; init; }

	public IList<SMLToken> TrailingNewlines { get; init; }

	public SMLDocument()
	{
		LeadingNewlines = [];
		Values = [];
		TrailingNewlines = [];
	}

	public SMLDocument(
		Dictionary<string, SMLTableValue> values)
	{
		LeadingNewlines = [];
		Values = values;
		TrailingNewlines = [];
	}

	public SMLDocument(
		IList<SMLToken> leadingNewlines,
		Dictionary<string, SMLTableValue> values,
		IList<SMLToken> trailingNewlines)
	{
		LeadingNewlines = leadingNewlines;
		Values = values;
		TrailingNewlines = trailingNewlines;
	}

	public override bool Equals(object? obj)
	{
		return this.Equals(obj as SMLDocument);
	}

	public bool Equals(SMLDocument? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (object.ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return Enumerable.SequenceEqual(this.Values, other.Values);
	}

	public override int GetHashCode()
	{
		return Values.GetHashCode();
	}

	public static bool operator ==(SMLDocument? lhs, SMLDocument? rhs)
	{
		if (lhs is null)
		{
			if (rhs is null)
				return true;
			else
				return false;
		}

		return lhs.Equals(rhs);
	}

	public static bool operator !=(SMLDocument? lhs, SMLDocument? rhs)
	{
		return !(lhs == rhs);
	}
}