// <copyright file="PackageName.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Diagnostics.CodeAnalysis;
using System.Text.RegularExpressions;

namespace Soup.Build;

/// <summary>
/// A package name that uniquely identifies a package in a single build graph
/// </summary>
public partial class PackageName : IEquatable<PackageName>, IComparable<PackageName>
{
	private readonly string? _owner;

	/// <summary>
	/// Try parse a package name from the provided string
	/// </summary>
	public static bool TryParse(string value, [MaybeNullWhen(false)] out PackageName result)
	{
		var nameRegex = ParseRegex();
		var matchName = nameRegex.Match(value);
		if (matchName.Success)
		{
			// The package is a valid name
			var owner = matchName.Groups.ContainsKey("Owner") && matchName.Groups["Owner"].Success ?
				matchName.Groups["Owner"].Value : null;
			var name = matchName.Groups["Name"].Value;
			result = new PackageName(owner, name);
			return true;
		}
		else
		{
			result = null;
			return false;
		}
	}

	/// <summary>
	/// Parse a package name from the provided string.
	/// </summary>
	public static PackageName Parse(string value)
	{
		if (TryParse(value, out var result))
		{
			return result;
		}
		else
		{
			throw new ArgumentException("Invalid package name");
		}
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageName"/> class.
	/// </summary>
	public PackageName(string? owner, string name)
	{
		_owner = owner;
		Name = name;
	}

	/// <summary>
	/// Gets a value indicating whether the name has an owner or not
	/// </summary>
	public bool HasOwner => _owner is not null;

	/// <summary>
	/// Gets or sets the Owner.
	/// </summary>
	public string Owner
	{
		get
		{
			if (_owner is null)
				throw new InvalidOperationException("Cannot get null owner");
			return _owner;
		}
	}

	/// <summary>
	/// Gets or sets the Name.
	/// </summary>
	public string Name { get; init; }

	/// <summary>
	/// Equality operator
	/// </summary>
	public bool Equals(PackageName? other)
	{
		if (other is null)
			return false;
		return _owner == other._owner &&
			Name == other.Name;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as PackageName);
	}

	public override int GetHashCode()
	{
		var ownerHash = string.IsNullOrEmpty(_owner) ? 0 : _owner.GetHashCode(StringComparison.Ordinal) * 0x1000;
		var nameHash = Name.GetHashCode();
		return ownerHash + nameHash;
	}

	public int CompareTo(PackageName? other)
	{
		return ToString().CompareTo(other?.ToString());
	}

	public static bool operator ==(PackageName? lhs, PackageName? rhs)
	{
		return lhs is null ? rhs is null : lhs.Equals(rhs);
	}

	public static bool operator !=(PackageName? lhs, PackageName? rhs)
	{
		return !(lhs == rhs);
	}

	/// <summary>
	/// Convert to string
	/// </summary>
	public override string ToString()
	{
		if (_owner is not null)
		{
			return $"{_owner}|{Name}";
		}
		else
		{
			return $"{Name}";
		}
	}

	[GeneratedRegex(@"^(?:(?<Owner>[A-Za-z][\w.]*)\|)?(?<Name>[A-Za-z][\w.]*)$")]
	private static partial Regex ParseRegex();
}