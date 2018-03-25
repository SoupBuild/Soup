// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using Newtonsoft.Json;
using System;

namespace Soup
{
	[JsonObject]
	[JsonConverter(typeof(PackageReferenceJsonConverter))]
	public class PackageReference
	{
		public static bool operator ==(PackageReference lhs, PackageReference rhs)
		{
			return lhs.Name == rhs.Name &&
				lhs.Version == rhs.Version;
		}

		public static bool operator !=(PackageReference lhs, PackageReference rhs)
		{
			return lhs.Name != rhs.Name ||
				lhs.Version != rhs.Version;
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
			if (tokens.Length != 2)
			{
				throw new ArgumentException("The package must be split by the @ symbol");
			}

			return new PackageReference(tokens[0], SemanticVersion.Parse(tokens[1]));
		}

		public PackageReference() :
			this(null, null)
		{
		}

		public PackageReference(string name, SemanticVersion version)
		{
			Name = name;
			Version = version;
		}

		[JsonProperty("name")]
		public string Name { get; set; }

		[JsonProperty("version")]
		public SemanticVersion Version { get; set; }

		public override bool Equals(Object obj)
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
			return Name.GetHashCode() ^ Version.GetHashCode();
		}

		public override string ToString()
		{
			return $"{Name}@{Version}";
		}
	}
}
