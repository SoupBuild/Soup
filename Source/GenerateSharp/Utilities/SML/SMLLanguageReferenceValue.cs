// <copyright file="SMLStringValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
		OpenParenthesis = SMLToken.Empty;
		LanguageName = SMLToken.Empty;
		AtSign = SMLToken.Empty;
		VersionReference = SMLToken.Empty;
		CloseParenthesis = SMLToken.Empty;

		Value = value;
	}

	public SMLLanguageReferenceValue(
		SMLToken openParenthesis,
		SMLToken languageName,
		SMLToken atSign,
		SMLToken versionReference,
		SMLToken closeParenthesis,
		LanguageReference value)
	{
		OpenParenthesis = openParenthesis;
		LanguageName = languageName;
		AtSign = atSign;
		VersionReference = versionReference;
		CloseParenthesis = closeParenthesis;

		Value = value;
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
		return Value == other.Value;
	}

	public override int GetHashCode()
	{
		return Value.GetHashCode();
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