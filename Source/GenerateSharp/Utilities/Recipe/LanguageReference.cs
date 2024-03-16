// <copyright file="LanguageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
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
		Name = string.Empty;
		Version = new SemanticVersion();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="LanguageReference"/> class.
	/// </summary>
	public LanguageReference(string name, SemanticVersion version)
	{
		Name = name;
		Version = version;
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
		return Name == other.Name &&
			Version == other.Version;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as LanguageReference);
	}

	public override int GetHashCode()
	{
		var nameHash = string.IsNullOrEmpty(Name) ? 0 : Name.GetHashCode(StringComparison.Ordinal) * 0x1000;
		var versionHash = Version is null ? 0 : Version.GetHashCode();
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
		return $"{Name}|{Version}";
	}

	[GeneratedRegex(@"^(?<Name>[A-Za-z][\w#+.]*)(?:\|(?<Version>\d+(?:.\d+)?(?:.\d+)?))?$")]
	private static partial Regex ParseRegex();
}