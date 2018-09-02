// <copyright company="Soup" file="SemanticVersion.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;

	/// <summary>
	/// The semantic version class
	/// </summary>
	public class SemanticVersion
	{
		public SemanticVersion(short major, short minor, short patch)
		{
			Major = major;
			Minor = minor;
			Patch = patch;
		}

		public short Major { get; set; }

		public short Minor { get; set; }

		public short Patch { get; set; }

		public static bool operator ==(SemanticVersion lhs, SemanticVersion rhs)
		{
			if (object.ReferenceEquals(lhs, null))
			{
				return object.ReferenceEquals(rhs, null);
			}
			else if (object.ReferenceEquals(rhs, null))
			{
				return false;
			}
			else
			{
				return lhs.Major == rhs.Major &&
					lhs.Minor == rhs.Minor &&
					lhs.Patch == rhs.Patch;
			}
		}

		public static bool operator !=(SemanticVersion lhs, SemanticVersion rhs)
		{
			if (object.ReferenceEquals(lhs, null))
			{
				return !object.ReferenceEquals(rhs, null);
			}
			else if (object.ReferenceEquals(rhs, null))
			{
				return true;
			}
			else
			{
				return 
					lhs.Major != rhs.Major ||
					lhs.Minor != rhs.Minor ||
					lhs.Patch != rhs.Patch;
			}
		}

		public static bool TryParse(string value, out SemanticVersion result)
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

		public static SemanticVersion Parse(string value)
		{
			var values = value.Split('.');
			if (values.Length != 3)
			{
				new ArgumentException("The version string must have three values.");
			}

			return new SemanticVersion(
				short.Parse(values[0]),
				short.Parse(values[1]),
				short.Parse(values[2]));
		}

		public override bool Equals(object obj)
		{
			var other = obj as SemanticVersion;
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
			return Major ^ Minor ^ Patch;
		}

		public override string ToString()
		{
			return $"{Major}.{Minor}.{Patch}";
		}
	}
}
