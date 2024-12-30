// <copyright file="SMLArray.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities;

public class SMLArray : IEquatable<SMLArray>
{
	public SMLToken OpenBracket { get; set; }
	public IList<SMLToken> LeadingNewlines { get; init; }
	public IList<SMLArrayValue> Values { get; init; }
	public IList<SMLToken> TrailingNewlines { get; init; }
	public SMLToken CloseBracket { get; set; }

	public SMLArray()
	{
		this.OpenBracket = SMLToken.Empty;
		this.LeadingNewlines = [];
		this.Values = [];
		this.TrailingNewlines = [];
		this.CloseBracket = SMLToken.Empty;
	}

	public SMLArray(IList<SMLArrayValue> values)
	{
		this.OpenBracket = SMLToken.Empty;
		this.LeadingNewlines = [];
		this.Values = values;
		this.TrailingNewlines = [];
		this.CloseBracket = SMLToken.Empty;
	}

	public SMLArray(
		SMLToken openBracket,
		IList<SMLArrayValue> values,
		SMLToken closeBracket)
	{
		this.OpenBracket = openBracket;
		this.LeadingNewlines = [];
		this.Values = values;
		this.TrailingNewlines = [];
		this.CloseBracket = closeBracket;
	}

	public SMLArray(
		SMLToken openBracket,
		IList<SMLToken> leadingNewlines,
		IList<SMLArrayValue> values,
		IList<SMLToken> trailingNewlines,
		SMLToken closeBracket)
	{
		this.OpenBracket = openBracket;
		this.LeadingNewlines = leadingNewlines;
		this.Values = values;
		this.TrailingNewlines = trailingNewlines;
		this.CloseBracket = closeBracket;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLArray);
	}

	public bool Equals(SMLArray? other)
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

	public static bool operator ==(SMLArray? lhs, SMLArray? rhs)
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

	public static bool operator !=(SMLArray? lhs, SMLArray? rhs)
	{
		return !(lhs == rhs);
	}
}