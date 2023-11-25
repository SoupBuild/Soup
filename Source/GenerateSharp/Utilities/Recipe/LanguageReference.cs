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
public class LanguageReference : IEquatable<LanguageReference>
{
	private string _name;
	private SemanticVersion _version;

	/// <summary>
	/// Try parse a language reference from the provided string
	/// </summary>
	public static bool TryParse(string value, [MaybeNullWhen(false)] out LanguageReference result)
	{
		var nameRegex = new Regex(@"^(?<Name>[A-Za-z][\w#+.]*)(?:\|(?<Version>\d+(?:.\d+)?(?:.\d+)?))?$");
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
		_name = string.Empty;
		_version = new SemanticVersion();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="LanguageReference"/> class.
	/// </summary>
	public LanguageReference(string name, SemanticVersion version)
	{
		_name = name;
		_version = version;
	}

	/// <summary>
	/// Gets or sets the Name.
	/// </summary>
	public string Name
	{
		get
		{
			return _name;
		}
	}

	/// <summary>
	/// Gets or sets the Version.
	/// </summary>
	public SemanticVersion Version
	{
		get
		{
			return _version;
		}
	}

	/// <summary>
	/// Equality operator
	/// </summary>
	public bool Equals(LanguageReference? other)
	{
		if (ReferenceEquals(other, null))
			return false;
		return _name == other._name &&
			_version == other._version;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as LanguageReference);
	}

	public override int GetHashCode()
	{
		var nameHash = string.IsNullOrEmpty(_name) ? 0 : _name.GetHashCode(StringComparison.Ordinal) * 0x1000;
		var versionHash = ReferenceEquals(_version, null) ? 0 : _version.GetHashCode();
		return nameHash + versionHash;
	}

	public static bool operator ==(LanguageReference? lhs, LanguageReference? rhs)
	{
		if (ReferenceEquals(lhs, null))
			return ReferenceEquals(rhs, null);
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
		return $"{_name}|{_version}";
	}
}