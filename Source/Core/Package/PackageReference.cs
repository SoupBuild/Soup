// <copyright company="Soup" file="PackageReference.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;
	using Newtonsoft.Json;

	/// <summary>
	/// A package reference object which will consist of a name version pair that 
	/// refers to a published package or a path to a local recipe
	/// </summary>
	[JsonObject]
	[JsonConverter(typeof(PackageReferenceJsonConverter))]
	public class PackageReference
	{
		public PackageReference()
		{
			Name = null;
			Version = null;
			Path = null;
		}

		public PackageReference(string name, SemanticVersion version)
		{
			Name = name;
			Version = version;
			Path = null;
		}

		public PackageReference(string path)
		{
			Name = null;
			Version = null;
			Path = path;
		}

		[JsonProperty("name")]
		public string Name { get; set; }

		[JsonProperty("version")]
		public SemanticVersion Version { get; set; }

		[JsonProperty("path")]
		public string Path { get; set; }

		public static bool operator ==(PackageReference lhs, PackageReference rhs)
		{
			return lhs.Name == rhs.Name &&
				lhs.Version == rhs.Version &&
				lhs.Path == rhs.Path;
		}

		public static bool operator !=(PackageReference lhs, PackageReference rhs)
		{
			return lhs.Name != rhs.Name ||
				lhs.Version != rhs.Version ||
				lhs.Path != rhs.Path;
		}

		public static bool TryParse(string value, out PackageReference result)
		{
			try
			{
				result = Parse(value);
				return true;
			}
			catch (ArgumentException)
			{
			}

			result = null;
			return false;
		}

		public static PackageReference Parse(string value)
		{
			var tokens = value.Split('@');
			if (tokens.Length == 2)
			{
				// The package is a published reference
				return new PackageReference(tokens[0], SemanticVersion.Parse(tokens[1]));
			}
			else
			{
				// Assum that this package is a relative reference
				return new PackageReference(value);
			}
		}

		public override bool Equals(object obj)
		{
			var other = obj as PackageReference;
			if (other == null)
			{
				return false;
			}
			else
			{
				return this == other;
			}
		}

		public override int GetHashCode()
		{
			return Name.GetHashCode() ^ Version.GetHashCode() ^ Path.GetHashCode();
		}

		public override string ToString()
		{
			if (Path != null)
			{
				return Path;
			}
			else
			{
				return $"{Name}@{Version}";
			}
		}
	}
}
