﻿// <copyright file="PackageIdentifier.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Diagnostics.CodeAnalysis;
using System.Text.RegularExpressions;

namespace Soup.Build;

/// <summary>
/// A package identifier object which will consist of a name, owner and language pair
/// </summary>
public partial class PackageIdentifier : IEquatable<PackageIdentifier>
{
	private readonly PackageName _name;

	/// <summary>
	/// Try parse a package identifier from the provided string
	/// </summary>
	public static bool TryParse(string value, [MaybeNullWhen(false)] out PackageIdentifier result)
	{
		var nameRegex = ParseRegex();
		var matchName = nameRegex.Match(value);
		if (matchName.Success)
		{
			var language = matchName.Groups.ContainsKey("Language") && matchName.Groups["Language"].Success ?
				matchName.Groups["Language"].Value : null;
			var owner = matchName.Groups.ContainsKey("Owner") && matchName.Groups["Owner"].Success ?
				matchName.Groups["Owner"].Value : null;
			var name = matchName.Groups["Name"].Value;
			result = new PackageIdentifier(language, owner, name);
			return true;
		}
		else
		{
			result = null;
			return false;
		}
	}

	/// <summary>
	/// Parse a package identifier from the provided string.
	/// </summary>
	public static PackageIdentifier Parse(string value)
	{
		if (TryParse(value, out var result))
		{
			return result;
		}
		else
		{
			throw new ArgumentException("Invalid package identifier");
		}
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageIdentifier"/> class.
	/// </summary>
	public PackageIdentifier()
	{
		Language = null;
		_name = new PackageName(null, string.Empty);
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageIdentifier"/> class.
	/// </summary>
	public PackageIdentifier(string? language, string? owner, string name)
	{
		Language = language;
		_name = new PackageName(owner, name);
	}

	/// <summary>
	/// Gets a value indicating whether the identifier has a language
	/// </summary>
	public bool HasLanguage => Language is not null;

	/// <summary>
	/// Gets or sets the Language.
	/// </summary>
	public string? Language { get; set; }

	/// <summary>
	/// Gets a value indicating whether the identifier has an owner
	/// </summary>
	public bool HasOwner => _name.HasOwner;

	/// <summary>
	/// Gets or sets the Owner.
	/// </summary>
	public string? Owner => _name.Owner;

	/// <summary>
	/// Gets or sets the Name.
	/// </summary>
	public string Name => _name.Name;

	/// <summary>
	/// Equality operator
	/// </summary>
	public bool Equals(PackageIdentifier? other)
	{
		if (other is null)
			return false;
		return Language == other.Language &&
			_name == other._name;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as PackageIdentifier);
	}

	public override int GetHashCode()
	{
		var languageHash = Language is null ? 0 : Language.GetHashCode(StringComparison.Ordinal) * 0x1000;
		var nameHash = _name.GetHashCode();
		return languageHash + nameHash;
	}

	public static bool operator ==(PackageIdentifier? lhs, PackageIdentifier? rhs)
	{
		return lhs is null ? rhs is null : lhs.Equals(rhs);
	}

	public static bool operator !=(PackageIdentifier? lhs, PackageIdentifier? rhs)
	{
		return !(lhs == rhs);
	}

	/// <summary>
	/// Convert to string
	/// </summary>
	public override string ToString()
	{
		// Build up the language/owner/name reference
		if (Language is not null)
		{
			return $"[{Language}]{_name}";
		}
		else
		{
			return _name.ToString();
		}
	}

	[GeneratedRegex(@"^(?:\[(?<Language>[\w#+]+)\])?(?:(?<Owner>[A-Za-z][\w.]*)\|)?(?<Name>[A-Za-z][\w.]*)(?:@(?<Version>\d+(?:.\d+)?(?:.\d+)?))?$")]
	private static partial Regex ParseRegex();
}