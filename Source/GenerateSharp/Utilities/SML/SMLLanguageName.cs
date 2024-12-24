// <copyright file="SMLStringValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

public class SMLLanguageName : IEquatable<SMLLanguageName>
{
	public SMLToken OpenParenthesis { get; set; }
	public SMLToken LanguageName { get; set; }
	public SMLToken CloseParenthesis { get; set; }

	public string Value { get; set; }

	public SMLLanguageName(
		SMLToken openParenthesis,
		SMLToken languageName,
		SMLToken closeParenthesis,
		string value)
	{
		this.OpenParenthesis = openParenthesis;
		this.LanguageName = languageName;
		this.CloseParenthesis = closeParenthesis;

		this.Value = value;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLLanguageName);
	}

	public bool Equals(SMLLanguageName? other)
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

	public static bool operator ==(SMLLanguageName? lhs, SMLLanguageName? rhs)
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

	public static bool operator !=(SMLLanguageName? lhs, SMLLanguageName? rhs)
	{
		return !(lhs == rhs);
	}
}