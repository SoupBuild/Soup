// <copyright file="PackageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Utilities;
using System;

namespace Soup.Build.Generate
{
	/// <summary>
	/// A package reference object which will consist of a name version pair that
	/// refers to a published package or a path to a local recipe
	/// </summary>
	internal class PackageReference : IEquatable<PackageReference>
	{
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
			// Check if there is the @ separator
			var separatorLocation = value.IndexOf('@');
			if (separatorLocation != -1)
			{
				// The package is a published reference
				var name = value.Substring(0, separatorLocation);
				var versionString = value.Substring(separatorLocation + 1);
				var version = SemanticVersion.Parse(versionString);
				return new PackageReference(name, version);
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
			_name = string.Empty;
			_version = new SemanticVersion();
			_path = new Path();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageReference"/> class.
		/// </summary>
		public PackageReference(string name, SemanticVersion version)
		{
			_name = name;
			_version = version;
			_path = new Path();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageReference"/> class.
		/// </summary>
		public PackageReference(Path path)
		{
			_name = string.Empty;
			_version = new SemanticVersion();
			_path = path;
		}

		/// <summary>
		/// Gets a value indicating whether the reference is local or not
		/// </summary>
		public bool IsLocal => string.IsNullOrEmpty(_name);

		/// <summary>
		/// Gets or sets the Name.
		/// </summary>
		public string GetName
		{
			get
			{
				if (IsLocal)
					throw new InvalidOperationException("Cannot get the name of a local reference.");
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
				if (IsLocal)
					throw new InvalidOperationException("Cannot get the version of a local reference.");
				if (ReferenceEquals(_version, null))
					throw new InvalidOperationException("No version.");
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
				if (!IsLocal)
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
			return (_name.GetHashCode() * 0x100000) + (_version.GetHashCode() * 0x1000) + _path.GetHashCode();
		}

		public static bool operator ==(PackageReference lhs, PackageReference rhs)
		{
			if (ReferenceEquals(lhs, null))
				return ReferenceEquals(rhs, null);
			return lhs.Equals(rhs);
		}

		public static bool operator !=(PackageReference lhs, PackageReference rhs)
		{
			return !(lhs == rhs);
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		public override string ToString()
		{
			// If the reference is a path then just return that
			if (IsLocal)
			{
				return _path.ToString();
			}
			else
			{
				// Build up the name/version reference
				return $"{_name}@{_version}";
			}
		}

		private string _name;
		private SemanticVersion _version;
		private Path _path;
	}
}
