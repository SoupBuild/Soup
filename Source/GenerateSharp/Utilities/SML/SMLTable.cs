// <copyright file="SMLTable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Utilities;

public class SMLTable : IEquatable<SMLTable>
{
	public SMLToken OpenBrace { get; init; }
	public IList<SMLToken> LeadingNewlines { get; init; }
	public Dictionary<string, SMLTableValue> Values { get; init; }
	public IList<SMLToken> TrailingNewlines { get; init; }
	public SMLToken CloseBrace { get; init; }

	public SMLTable()
	{
		OpenBrace = SMLToken.Empty;
		LeadingNewlines = [];
		Values = [];
		TrailingNewlines = [];
		CloseBrace = SMLToken.Empty;
	}

	public SMLTable(
		Dictionary<string, SMLTableValue> values)
	{
		OpenBrace = SMLToken.Empty;
		LeadingNewlines = [];
		Values = values;
		TrailingNewlines = [];
		CloseBrace = SMLToken.Empty;
	}

	public SMLTable(
		SMLToken openBrace,
		Dictionary<string, SMLTableValue> values,
		SMLToken closeBrace)
	{
		OpenBrace = openBrace;
		LeadingNewlines = [];
		Values = values;
		TrailingNewlines = [];
		CloseBrace = closeBrace;
	}

	public SMLTable(
		SMLToken openBrace,
		IList<SMLToken> leadingNewlines,
		Dictionary<string, SMLTableValue> values,
		IList<SMLToken> trailingNewlines,
		SMLToken closeBrace)
	{
		OpenBrace = openBrace;
		LeadingNewlines = leadingNewlines;
		Values = values;
		TrailingNewlines = trailingNewlines;
		CloseBrace = closeBrace;
	}

	public override bool Equals(object? obj)
	{
		return this.Equals(obj as SMLTable);
	}

	public bool Equals(SMLTable? other)
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

	public static bool operator ==(SMLTable? lhs, SMLTable? rhs)
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

	public static bool operator !=(SMLTable? lhs, SMLTable? rhs)
	{
		return !(lhs == rhs);
	}
}