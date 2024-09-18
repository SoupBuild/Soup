// <copyright file="SMLStringValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

public class SMLPackageReferenceValue : IEquatable<SMLPackageReferenceValue>
{
	public SMLToken LessThan { get; set; }
	public SMLLanguageName? LanguageName { get; set; }
	public SMLToken UserName { get; set; }
	public SMLToken UserPipe { get; set; }
	public SMLToken PackageName { get; set; }
	public SMLToken AtSign { get; set; }
	public SMLToken VersionReference { get; set; }
	public SMLToken GreaterThan { get; set; }

	public PackageReference Value { get; set; }

	public SMLPackageReferenceValue(
		PackageReference value)
	{
		LessThan = SMLToken.Empty;
		LanguageName = null;
		UserName = SMLToken.Empty;
		UserPipe = SMLToken.Empty;
		PackageName = SMLToken.Empty;
		AtSign = SMLToken.Empty;
		VersionReference = SMLToken.Empty;
		GreaterThan = SMLToken.Empty;

		Value = value;
	}

	public SMLPackageReferenceValue(
		SMLToken lessThan,
		SMLLanguageName? languageName,
		SMLToken userName,
		SMLToken userPipe,
		SMLToken packageName,
		SMLToken atSign,
		SMLToken versionReference,
		SMLToken greaterThan,
		PackageReference value)
	{
		LessThan = lessThan;
		LanguageName = languageName;
		UserName = userName;
		UserPipe = userPipe;
		PackageName = packageName;
		AtSign = atSign;
		VersionReference = versionReference;
		GreaterThan = greaterThan;

		Value = value;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLPackageReferenceValue);
	}

	public bool Equals(SMLPackageReferenceValue? other)
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

	public static bool operator ==(SMLPackageReferenceValue? lhs, SMLPackageReferenceValue? rhs)
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

	public static bool operator !=(SMLPackageReferenceValue? lhs, SMLPackageReferenceValue? rhs)
	{
		return !(lhs == rhs);
	}
}