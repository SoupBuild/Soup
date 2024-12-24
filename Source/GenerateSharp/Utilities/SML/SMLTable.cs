// <copyright file="SMLTable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

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
		this.OpenBrace = SMLToken.Empty;
		this.LeadingNewlines = [];
		this.Values = [];
		this.TrailingNewlines = [];
		this.CloseBrace = SMLToken.Empty;
	}

	public SMLTable(
		Dictionary<string, SMLTableValue> values)
	{
		this.OpenBrace = SMLToken.Empty;
		this.LeadingNewlines = [];
		this.Values = values;
		this.TrailingNewlines = [];
		this.CloseBrace = SMLToken.Empty;
	}

	public SMLTable(
		SMLToken openBrace,
		Dictionary<string, SMLTableValue> values,
		SMLToken closeBrace)
	{
		this.OpenBrace = openBrace;
		this.LeadingNewlines = [];
		this.Values = values;
		this.TrailingNewlines = [];
		this.CloseBrace = closeBrace;
	}

	public SMLTable(
		SMLToken openBrace,
		IList<SMLToken> leadingNewlines,
		Dictionary<string, SMLTableValue> values,
		IList<SMLToken> trailingNewlines,
		SMLToken closeBrace)
	{
		this.OpenBrace = openBrace;
		this.LeadingNewlines = leadingNewlines;
		this.Values = values;
		this.TrailingNewlines = trailingNewlines;
		this.CloseBrace = closeBrace;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLTable);
	}

	public bool Equals(SMLTable? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return Enumerable.SequenceEqual(this.Values, other.Values);
	}

	public override int GetHashCode()
	{
		return this.Values.GetHashCode();
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