// <copyright file="SMLStringValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

public class SMLLanguageReferenceValue : IEquatable<SMLLanguageReferenceValue>
{
	public SMLToken OpenParenthesis { get; set; }
	public SMLToken LanguageName { get; set; }
	public SMLToken AtSign { get; set; }
	public SMLToken VersionReference { get; set; }
	public SMLToken CloseParenthesis { get; set; }

	public LanguageReference Value { get; set; }

	public SMLLanguageReferenceValue(
		LanguageReference value)
	{
		this.OpenParenthesis = SMLToken.Empty;
		this.LanguageName = SMLToken.Empty;
		this.AtSign = SMLToken.Empty;
		this.VersionReference = SMLToken.Empty;
		this.CloseParenthesis = SMLToken.Empty;

		this.Value = value;
	}

	public SMLLanguageReferenceValue(
		SMLToken openParenthesis,
		SMLToken languageName,
		SMLToken atSign,
		SMLToken versionReference,
		SMLToken closeParenthesis,
		LanguageReference value)
	{
		this.OpenParenthesis = openParenthesis;
		this.LanguageName = languageName;
		this.AtSign = atSign;
		this.VersionReference = versionReference;
		this.CloseParenthesis = closeParenthesis;

		this.Value = value;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLLanguageReferenceValue);
	}

	public bool Equals(SMLLanguageReferenceValue? other)
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
		return this.Value.GetHashCode();
	}

	public static bool operator ==(SMLLanguageReferenceValue? lhs, SMLLanguageReferenceValue? rhs)
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

	public static bool operator !=(SMLLanguageReferenceValue? lhs, SMLLanguageReferenceValue? rhs)
	{
		return !(lhs == rhs);
	}
}