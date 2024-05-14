// <copyright file="PackageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Text.RegularExpressions;
using Path = Opal.Path;

namespace Soup.Build;

/// <summary>
/// A package reference object which will consist of a name version pair that
/// refers to a published package or a path to a local recipe
/// </summary>
public partial class PackageReference : IEquatable<PackageReference>
{
	private readonly PackageIdentifier? _packageIdentifier;
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
		this._packageIdentifier = null;
		this._version = null;
		this._path = null;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageReference"/> class.
	/// </summary>
	public PackageReference(string? language, string? owner, string name, SemanticVersion? version)
	{
		this._packageIdentifier = new PackageIdentifier(language, owner, name);
		this._version = version;
		this._path = null;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageReference"/> class.
	/// </summary>
	public PackageReference(Path path)
	{
		this._packageIdentifier = null;
		this._version = null;
		this._path = path;
	}

	/// <summary>
	/// Gets a value indicating whether the reference is local or not
	/// </summary>
	public bool IsLocal => this._packageIdentifier is null;

	/// <summary>
	/// Gets or sets the Language.
	/// </summary>
	public string? Language
	{
		get
		{
			if (this._packageIdentifier is null)
				throw new InvalidOperationException("Cannot get the language of a local reference.");
			return this._packageIdentifier.Language;
		}
	}

	/// <summary>
	/// Gets or sets the Owner.
	/// </summary>
	public string? Owner
	{
		get
		{
			if (this._packageIdentifier is null)
				throw new InvalidOperationException("Cannot get the owner of a local reference.");
			return this._packageIdentifier.Owner;
		}
	}

	/// <summary>
	/// Gets or sets the Name.
	/// </summary>
	public string Name
	{
		get
		{
			if (this._packageIdentifier is null)
				throw new InvalidOperationException("Cannot get the name of a local reference.");
			return this._packageIdentifier.Name;
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
			return this._version;
		}
	}

	/// <summary>
	/// Gets or sets the Path.
	/// </summary>
	public Path Path
	{
		get
		{
			if (this._path is null)
				throw new InvalidOperationException("Cannot get the path of a non-local reference.");
			return this._path;
		}
	}

	/// <summary>
	/// Equality operator
	/// </summary>
	public bool Equals(PackageReference? other)
	{
		if (other is null)
			return false;
		return this._packageIdentifier == other._packageIdentifier &&
			this._version == other._version &&
			this._path == other._path;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as PackageReference);
	}

	public override int GetHashCode()
	{
		var identifierHash = this._packageIdentifier is null ? 0 : this._packageIdentifier.GetHashCode() * 0x100000;
		var versionHash = this._version is null ? 0 : this._version.GetHashCode() * 0x1000;
		var pathHash = this._path is null ? 0 : this._path.GetHashCode();
		return identifierHash + versionHash + pathHash;
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
		if (this._path is not null)
		{
			return this._path.ToString();
		}
		else
		{
			// Build up the language/owner/name/version reference
			if (this._version is not null)
			{
				return $"{this._packageIdentifier}@{this._version}";
			}
			else
			{
				return $"{this._packageIdentifier}";
			}
		}
	}

	[GeneratedRegex(@"^(?:\[(?<Language>[\w#+]+)\])?(?:(?<Owner>[A-Za-z][\w.]*)\|)?(?<Name>[A-Za-z][\w.]*)(?:@(?<Version>\d+(?:.\d+)?(?:.\d+)?))?$")]
	private static partial Regex ParseRegex();
}