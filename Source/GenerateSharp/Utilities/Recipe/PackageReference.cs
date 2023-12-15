﻿// <copyright file="PackageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Text.RegularExpressions;

namespace Soup.Build;

/// <summary>
/// A package reference object which will consist of a name version pair that
/// refers to a published package or a path to a local recipe
/// </summary>
public partial class PackageReference : IEquatable<PackageReference>
{
	private readonly string? _language;
	private readonly string? _owner;
	private readonly string? _name;
	private readonly SemanticVersion? _version;
	private readonly Path? _path;

	/// <summary>
	/// Try parse a package reference from the provided string
	/// </summary>
	public static bool TryParse(string value, out PackageReference result)
	{
		var nameRegex = ParseRegex();
		var matchName = nameRegex.Match(value);
		if (matchName.Success)
		{
			// The package is a published reference
			var language = matchName.Groups.ContainsKey("Language") && matchName.Groups["Language"].Success ?
				matchName.Groups["Language"].Value : null;
			var owner = matchName.Groups.ContainsKey("Owner") && matchName.Groups["Owner"].Success ?
				matchName.Groups["Owner"].Value : null;
			var name = matchName.Groups["Name"].Value;
			var version = matchName.Groups.ContainsKey("Version") && matchName.Groups["Version"].Success ?
					SemanticVersion.Parse(matchName.Groups["Version"].Value) : null;
			result = new PackageReference(language, owner, name, version);
			return true;
		}
		else
		{
			// Assume that this package is a relative path reference
			result = new PackageReference(new Path(value));
			return true;
		}
	}

	/// <summary>
	/// Parse a package reference from the provided string.
	/// </summary>
	public static PackageReference Parse(string value)
	{
		if (TryParse(value, out var result))
		{
			return result;
		}
		else
		{
			throw new ArgumentException("Invalid package reference");
		}
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageReference"/> class.
	/// </summary>
	public PackageReference()
	{
		_language = null;
		_owner = null;
		_name = null;
		_version = null;
		_path = null;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageReference"/> class.
	/// </summary>
	public PackageReference(string? language, string? owner, string name, SemanticVersion? version)
	{
		_language = language;
		_owner = owner;
		_name = name;
		_version = version;
		_path = null;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageReference"/> class.
	/// </summary>
	public PackageReference(Path path)
	{
		_language = null;
		_owner = null;
		_name = null;
		_version = null;
		_path = path;
	}

	/// <summary>
	/// Gets a value indicating whether the reference is local or not
	/// </summary>
	public bool IsLocal => string.IsNullOrEmpty(_name);

	/// <summary>
	/// Gets or sets the Language.
	/// </summary>
	public string? Language
	{
		get
		{
			if (IsLocal)
				throw new InvalidOperationException("Cannot get the language of a local reference.");
			return _language;
		}
	}

	/// <summary>
	/// Gets or sets the Owner.
	/// </summary>
	public string? Owner
	{
		get
		{
			if (IsLocal)
				throw new InvalidOperationException("Cannot get the owner of a local reference.");
			return _owner;
		}
	}

	/// <summary>
	/// Gets or sets the Name.
	/// </summary>
	public string Name
	{
		get
		{
			if (_name is null)
				throw new InvalidOperationException("Cannot get the name of a local reference.");
			return _name;
		}
	}

	/// <summary>
	/// Gets or sets the Version.
	/// </summary>
	public SemanticVersion? Version
	{
		get
		{
			if (IsLocal)
				throw new InvalidOperationException("Cannot get the version of a local reference.");
			return _version;
		}
	}

	/// <summary>
	/// Gets or sets the Path.
	/// </summary>
	public Path Path
	{
		get
		{
			if (_path is null)
				throw new InvalidOperationException("Cannot get the path of a non-local reference.");
			return _path;
		}
	}

	/// <summary>
	/// Equality operator
	/// </summary>
	public bool Equals(PackageReference? other)
	{
		if (other is null)
			return false;
		return _owner == other._owner &&
			_name == other._name &&
			_version == other._version &&
			_path == other._path;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as PackageReference);
	}

	public override int GetHashCode()
	{
		var ownerHash = string.IsNullOrEmpty(_owner) ? 0 : _owner.GetHashCode(StringComparison.Ordinal) * 0x200000;
		var nameHash = string.IsNullOrEmpty(_name) ? 0 : _name.GetHashCode(StringComparison.Ordinal) * 0x100000;
		var versionHash = _version is null ? 0 : _version.GetHashCode() * 0x1000;
		var pathHash = _path is null ? 0 : _path.GetHashCode();
		return ownerHash + nameHash + versionHash + pathHash;
	}

	public static bool operator ==(PackageReference? lhs, PackageReference? rhs)
	{
		return lhs is null ? rhs is null : lhs.Equals(rhs);
	}

	public static bool operator !=(PackageReference? lhs, PackageReference? rhs)
	{
		return !(lhs == rhs);
	}

	/// <summary>
	/// Convert to string
	/// </summary>
	public override string ToString()
	{
		// If the reference is a path then just return that
		if (_path is not null)
		{
			return _path.ToString();
		}
		else
		{
			// Build up the language/name/version reference
			if (_language is not null)
			{
				if (_version is not null)
				{
					return $"{_language}|{_owner}|{_name}@{_version}";
				}
				else
				{
					return $"{_language}|{_owner}|{_name}";
				}
			}
			else
			{
				if (_version is not null)
				{
					return $"{_owner}|{_name}@{_version}";
				}
				else
				{
					return $"{_owner}|{_name}";
				}
			}
		}
	}

	[GeneratedRegex(@"^(?:(?<Language>[\w#+]+)\|)?(?:(?<Owner>[\w#+]+)\|)(?<Name>[A-Za-z][\w.]*)(?:@(?<Version>\d+(?:.\d+)?(?:.\d+)?))?$")]
	private static partial Regex ParseRegex();
}