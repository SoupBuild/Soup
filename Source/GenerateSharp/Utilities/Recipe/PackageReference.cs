// <copyright file="PackageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Text.RegularExpressions;

namespace Soup.Build
{
	/// <summary>
	/// A package reference object which will consist of a name version pair that
	/// refers to a published package or a path to a local recipe
	/// </summary>
	public class PackageReference : IEquatable<PackageReference>
	{
		private string? _language;
		private string? _name;
		private SemanticVersion? _version;
		private Path? _path;

		/// <summary>
		/// Try parse a package reference from the provided string
		/// </summary>
		public static bool TryParse(string value, out PackageReference result)
		{
			// TODO: Invert try parse to be the default and parse to add the exception
			// Way faster on the failed case and this could eat OOM
			try
			{
				result = Parse(value);
				return true;
			}
			catch
			{
			}

			result = new PackageReference();
			return false;
		}

		/// <summary>
		/// Parse a package reference from the provided string.
		/// </summary>
		public static PackageReference Parse(string value)
		{
			var nameRegex = new Regex(@"^(?:(?<Language>[\w#+]+)\|)?(?<Name>[A-Za-z]\w*)(?:@(?<Version>\d+.\d+.\d+))?$");
			var matchName = nameRegex.Match(value);
			if(matchName.Success)
			{
				// The package is a published reference
				var language = matchName.Groups.ContainsKey("Language") && matchName.Groups["Language"].Success ? matchName.Groups["Language"].Value : null;
				var name = matchName.Groups["Name"].Value;
				var version = matchName.Groups.ContainsKey("Version") && matchName.Groups["Version"].Success ?
						SemanticVersion.Parse(matchName.Groups["Version"].Value) :
						null;
				return new PackageReference(language, name, version);
			}
			else
			{
				// Assume that this package is a relative path reference
				return new PackageReference(new Path(value));
			}
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageReference"/> class.
		/// </summary>
		public PackageReference()
		{
			_language = null;
			_name = null;
			_version = null;
			_path = null;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageReference"/> class.
		/// </summary>
		public PackageReference(string? language, string name, SemanticVersion? version)
		{
			_language = language;
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
		/// Gets or sets the Name.
		/// </summary>
		public string Name
		{
			get
			{
				if (string.IsNullOrEmpty(_name))
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
				if (ReferenceEquals(_path, null))
					throw new InvalidOperationException("Cannot get the path of a non-local reference.");
				return _path;
			}
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		public bool Equals(PackageReference? rhs)
		{
			if (ReferenceEquals(rhs, null))
				return false;
			return _name == rhs._name &&
				_version == rhs._version &&
				_path == rhs._path;
		}

		public override bool Equals(object? rhs)
		{
			return Equals(rhs as PackageReference);
		}

		public override int GetHashCode()
		{
			var nameHash = string.IsNullOrEmpty(_name) ? 0 : _name.GetHashCode() * 0x100000;
			var versionHash = ReferenceEquals(_version, null) ? 0 : _version.GetHashCode() * 0x1000;
			var pathHash = ReferenceEquals(_path, null) ? 0 : _path.GetHashCode();
			return nameHash + versionHash + pathHash;
		}

		public static bool operator ==(PackageReference? lhs, PackageReference? rhs)
		{
			if (ReferenceEquals(lhs, null))
				return ReferenceEquals(rhs, null);
			return lhs.Equals(rhs);
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
			if (!ReferenceEquals(_path, null))
			{
				return _path.ToString();
			}
			else
			{
				// Build up the language/name/version reference
				if (!ReferenceEquals(_language, null))
				{
					if (!ReferenceEquals(_version, null))
					{
						return $"{_language}|{_name}@{_version}";
					}
					else
					{
						return $"{_language}|{_name}";
					}
				}
				else
				{
					if (!ReferenceEquals(_version, null))
					{
						return $"{_name}@{_version}";
					}
					else
					{
						return $"{_name}";
					}
				}
			}
		}
	}
}
