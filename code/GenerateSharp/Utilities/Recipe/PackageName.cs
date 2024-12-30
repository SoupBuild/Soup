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
		this.Owner = owner;
		this.Name = name;
	}

	/// <summary>
	/// Gets a value indicating whether the name has an owner or not
	/// </summary>
	public bool HasOwner => this.Owner is not null;

	/// <summary>
	/// Gets or sets the Owner.
	/// </summary>
	public string? Owner { get; set; }

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
		return this.Owner == other.Owner &&
			this.Name == other.Name;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as PackageName);
	}

	public override int GetHashCode()
	{
		var ownerHash = this.Owner is null ? 0 : this.Owner.GetHashCode(StringComparison.Ordinal) * 0x1000;
		var nameHash = this.Name.GetHashCode(StringComparison.Ordinal);
		return ownerHash + nameHash;
	}

	public int CompareTo(PackageName? other)
	{
		return string.Compare(ToString(), other?.ToString(), StringComparison.Ordinal);
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
		if (this.Owner is not null)
		{
			return $"{this.Owner}|{this.Name}";
		}
		else
		{
			return $"{this.Name}";
		}
	}

	[GeneratedRegex(@"^(?:(?<Owner>[A-Za-z][\w.]*)\|)?(?<Name>[A-Za-z][\w.]*)$")]
	private static partial Regex ParseRegex();

	public static bool operator <(PackageName left, PackageName right)
	{
		return left is null ? right is not null : left.CompareTo(right) < 0;
	}

	public static bool operator <=(PackageName left, PackageName right)
	{
		return left is null || left.CompareTo(right) <= 0;
	}

	public static bool operator >(PackageName left, PackageName right)
	{
		return left is not null && left.CompareTo(right) > 0;
	}

	public static bool operator >=(PackageName left, PackageName right)
	{
		return left is null ? right is null : left.CompareTo(right) >= 0;
	}
}