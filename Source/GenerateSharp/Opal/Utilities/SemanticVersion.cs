// <copyright file="SemanticVersion.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;

namespace Opal;

/// <summary>
/// The semantic version class.
/// </summary>
public class SemanticVersion : IEquatable<SemanticVersion>
{
	/// <summary>
	/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
	/// </summary>
	public SemanticVersion()
		: this(0, null, null)
	{
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
	/// </summary>
	/// <param name="major">The major version.</param>
	public SemanticVersion(int major)
		: this(major, null, null)
	{
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
	/// </summary>
	/// <param name="major">The major version.</param>
	/// <param name="minor">The minor version.</param>
	/// <param name="patch">The patch version.</param>
	public SemanticVersion(int major, int? minor, int? patch)
	{
		this.Major = major;
		this.Minor = minor;
		this.Patch = patch;
	}

	/// <summary>
	/// Gets or sets the version major.
	/// </summary>
	public int Major { get; set; }

	/// <summary>
	/// Gets or sets the version minor.
	/// </summary>
	public int? Minor { get; set; }

	/// <summary>
	/// Gets or sets the version patch.
	/// </summary>
	public int? Patch { get; set; }

	/// <summary>
	/// Try parse the value.
	/// </summary>
	/// <param name="value">The value.</param>
	/// <param name="result">The output parameter result.</param>
	public static bool TryParse(string value, [MaybeNullWhen(false)] out SemanticVersion result)
	{
		result = null;

		// Parse the integer values
		// TODO: Perform my own search to save string creation
		var stringValues = value.Split('.');
		if (stringValues.Length is < 1 or > 3)
		{
			return false;
		}

		var intValues = new List<int>();
		foreach (var stringValue in stringValues)
		{
			if (int.TryParse(stringValue, out var intValue))
			{
				intValues.Add(intValue);
			}
			else
			{
				return false;
			}
		}

		var major = intValues[0];

		int? minor = null;
		if (intValues.Count >= 2)
			minor = intValues[1];

		int? patch = null;
		if (intValues.Count >= 3)
			patch = intValues[2];

		result = new SemanticVersion(
			major,
			minor,
			patch);
		return true;
	}

	/// <summary>
	/// Parse the value.
	/// </summary>
	/// <param name="value">The value.</param>
	public static SemanticVersion Parse(string value)
	{
		if (TryParse(value, out var result))
		{
			return result;
		}
		else
		{
			throw new ArgumentException("Invalid semantic version");
		}
	}

	public static bool IsUpCompatible(SemanticVersion requested, SemanticVersion target)
	{
		// The target version must be fully qualified
		if (!target.Minor.HasValue)
			throw new ArgumentException("Target must have minor version");
		if (!target.Patch.HasValue)
			throw new ArgumentException("Target must have patch version");

		if (requested.Major == target.Major)
		{
			if (!requested.Minor.HasValue || target.Minor > requested.Minor)
			{
				// If the Minor version is acceptable increase then allow it
				return true;
			}
			else if (requested.Minor == target.Minor)
			{
				// Check that the patch is not backtracking
				return !requested.Patch.HasValue || target.Patch >= requested.Patch;
			}
			else
			{
				// Minor version drops not allowed
				return false;
			}
		}
		else
		{
			// The Major version must match
			return false;
		}
	}

	public bool Equals(SemanticVersion? other)
	{
		if (other is null)
			return false;
		return this.Major == other.Major &&
			this.Minor == other.Minor &&
			this.Patch == other.Patch;
	}

	public override bool Equals(object? obj)
	{
		return this.Equals(obj as SemanticVersion);
	}

	public override int GetHashCode()
	{
		var value = this.Major * 0x100000;
		value += this.Minor * 0x1000 ?? 0;
		value += this.Patch ?? 0;
		return value;
	}

	public static bool operator ==(SemanticVersion? lhs, SemanticVersion? rhs)
	{
		if (lhs is null)
			return rhs is null;
		return lhs.Equals(rhs);
	}

	public static bool operator !=(SemanticVersion? lhs, SemanticVersion? rhs)
	{
		return !(lhs == rhs);
	}

	/// <summary>
	/// Comparison operator.
	/// </summary>
	/// <param name="lhs">The left hand side.</param>
	/// <param name="rhs">The right hand side.</param>
	public static bool operator <(SemanticVersion lhs, SemanticVersion rhs)
	{
		return lhs.Major < rhs.Major ||
			lhs.Minor < rhs.Minor ||
			lhs.Patch < rhs.Patch;
	}

	public static bool operator >(SemanticVersion lhs, SemanticVersion rhs)
	{
		return lhs.Major > rhs.Major ||
			lhs.Minor > rhs.Minor ||
			lhs.Patch > rhs.Patch;
	}

	/// <summary>
	/// Convert to string.
	/// </summary>
	public override string ToString()
	{
		if (this.Minor is null)
		{
			return $"{this.Major}";
		}
		else if (this.Patch is null)
		{
			return $"{this.Major}.{this.Minor}";
		}
		else
		{
			return $"{this.Major}.{this.Minor}.{this.Patch}";
		}
	}
}