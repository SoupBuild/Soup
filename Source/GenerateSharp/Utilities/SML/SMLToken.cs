// <copyright file="SMLToken.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities;

public class SMLToken : IEquatable<SMLToken>
{
	public IList<string> LeadingTrivia { get; init; }
	public string Text { get; set; }
	public IList<string> TrailingTrivia { get; init; }

	public static SMLToken Empty => new SMLToken(string.Empty);

	public SMLToken(string text)
	{
		this.LeadingTrivia = [];
		this.Text = text;
		this.TrailingTrivia = [];
	}

	public SMLToken(IList<string> leadingTrivia, string text, IList<string> trailingTrivia)
	{
		this.LeadingTrivia = leadingTrivia;
		this.Text = text;
		this.TrailingTrivia = trailingTrivia;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLToken);
	}

	public bool Equals(SMLToken? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return this.Text == other.Text &&
			Enumerable.SequenceEqual(this.LeadingTrivia, other.LeadingTrivia) &&
			Enumerable.SequenceEqual(this.TrailingTrivia, other.TrailingTrivia);
	}

	// Only use the text to allow for mapping to same bins
	public override int GetHashCode()
	{
		return this.Text.GetHashCode(StringComparison.Ordinal);
	}

	public static bool operator ==(SMLToken? lhs, SMLToken? rhs)
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

	public static bool operator !=(SMLToken? lhs, SMLToken? rhs)
	{
		return !(lhs == rhs);
	}
}