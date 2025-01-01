// <copyright file="LanguageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Diagnostics.CodeAnalysis;
using System.Text.RegularExpressions;

namespace Soup.Build;

/// <summary>
/// A language reference object which will consist of a name version pair
/// </summary>
public partial class LanguageReference : IEquatable<LanguageReference>
{
	/// <summary>
	/// Try parse a language reference from the provided string
	/// </summary>
	public static bool TryParse(string value, [MaybeNullWhen(false)] out LanguageReference result)
	{
		var nameRegex = ParseRegex();
		var matchName = nameRegex.Match(value);
		if (matchName.Success)
		{
			// The language is a published reference
			var name = matchName.Groups["Name"].Value;
			var version = matchName.Groups.ContainsKey("Version") && matchName.Groups["Version"].Success ?
					SemanticVersion.Parse(matchName.Groups["Version"].Value) :
					new SemanticVersion();
			result = new LanguageReference(name, version);
			return true;
		}
		else
		{
			result = null;
			return false;
		}
	}

	/// <summary>
	/// Parse a language reference from the provided string.
	/// </summary>
	public static LanguageReference Parse(string value)
	{
		if (TryParse(value, out var result))
		{
			return result;
		}
		else
		{
			throw new ArgumentException($"Invalid Language Reference: {value}");
		}
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="LanguageReference"/> class.
	/// </summary>
	public LanguageReference()
	{
		this.Name = string.Empty;
		this.Version = new SemanticVersion();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="LanguageReference"/> class.
	/// </summary>
	public LanguageReference(string name, SemanticVersion version)
	{
		this.Name = name;
		this.Version = version;
	}

	/// <summary>
	/// Gets or sets the Name.
	/// </summary>
	public string Name { get; }

	/// <summary>
	/// Gets or sets the Version.
	/// </summary>
	public SemanticVersion Version { get; }

	/// <summary>
	/// Equality operator
	/// </summary>
	public bool Equals(LanguageReference? other)
	{
		if (other is null)
			return false;
		return this.Name == other.Name &&
			this.Version == other.Version;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as LanguageReference);
	}

	public override int GetHashCode()
	{
		var nameHash = string.IsNullOrEmpty(this.Name) ? 0 : this.Name.GetHashCode(StringComparison.Ordinal) * 0x1000;
		var versionHash = this.Version is null ? 0 : this.Version.GetHashCode();
		return nameHash + versionHash;
	}

	public static bool operator ==(LanguageReference? lhs, LanguageReference? rhs)
	{
		if (lhs is null)
			return rhs is null;
		return lhs.Equals(rhs);
	}

	public static bool operator !=(LanguageReference? lhs, LanguageReference? rhs)
	{
		return !(lhs == rhs);
	}

	/// <summary>
	/// Convert to string
	/// </summary>
	public override string ToString()
	{
		// Build up the name/version reference
		return $"{this.Name}|{this.Version}";
	}

	[GeneratedRegex(@"^(?<Name>[A-Za-z][\w#+.]*)(?:\|(?<Version>\d+(?:.\d+)?(?:.\d+)?))?$")]
	private static partial Regex ParseRegex();
}