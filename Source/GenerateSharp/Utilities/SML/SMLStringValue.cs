// <copyright file="SMLStringValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

public class SMLStringValue : IEquatable<SMLStringValue>
{
	public string Value { get; set; }

	public SMLToken OpenQuote { get; set; }

	public SMLToken Content { get; set; }

	public SMLToken CloseQuote { get; set; }

	public SMLStringValue()
	{
		this.Value = string.Empty;
		this.OpenQuote = SMLToken.Empty;
		this.Content = SMLToken.Empty;
		this.CloseQuote = SMLToken.Empty;
	}

	public SMLStringValue(string content)
	{
		this.Value = content;
		this.OpenQuote = SMLToken.Empty;
		this.Content = new SMLToken(content);
		this.CloseQuote = SMLToken.Empty;
	}

	public SMLStringValue(
		string value,
		SMLToken openQuote,
		SMLToken content,
		SMLToken closeQuote)
	{
		this.Value = value;
		this.OpenQuote = openQuote;
		this.Content = content;
		this.CloseQuote = closeQuote;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLStringValue);
	}

	public bool Equals(SMLStringValue? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return this.Value == other.Value;
	}

	public override int GetHashCode()
	{
		return this.Value.GetHashCode(StringComparison.InvariantCulture);
	}

	public static bool operator ==(SMLStringValue? lhs, SMLStringValue? rhs)
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

	public static bool operator !=(SMLStringValue? lhs, SMLStringValue? rhs)
	{
		return !(lhs == rhs);
	}
}