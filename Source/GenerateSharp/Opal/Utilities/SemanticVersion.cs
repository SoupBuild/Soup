// <copyright file="SemanticVersion.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Opal
{
	/// <summary>
	/// The semantic version class
	/// </summary>
	public class SemanticVersion : IEquatable<SemanticVersion>
	{
		/// <summary>
		/// Try parse the value
		/// </summary>
		public static bool TryParse(string value, out SemanticVersion result)
		{
			// TODO: Invert this so TryParse is the default and Parse adds an exception
			// it is way faster in the fail case
			try
			{
				result = Parse(value);
				return true;
			}
			catch
			{
			}

			result = new SemanticVersion();
			return false;
		}

		/// <summary>
		/// Parse the value
		/// </summary>
		public static SemanticVersion Parse(string value)
		{
			// Parse the integer values
			// TODO: Perform my own search to save string creation
			var stringValues = value.Split('.');
			if (stringValues.Length != 3)
			{
				throw new ArgumentException("The version string must have three values.");
			}

			var intValues = new List<int>();
			foreach (var stringValue in stringValues)
			{
				var intValue = int.Parse(stringValue);
				intValues.Add(intValue);
			}

			return new SemanticVersion(
				intValues[0],
				intValues[1],
				intValues[2]);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
		/// </summary>
		public  SemanticVersion() :
			this(0, 0, 0)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SemanticVersion"/> class.
		/// </summary>
		public SemanticVersion(int major, int minor, int patch)
		{
			Major = major;
			Minor = minor;
			Patch = patch;
		}

		/// <summary>
		/// Gets or sets the version major
		/// </summary>
		public int Major { get; set; }

		/// <summary>
		/// Gets or sets the version minor
		/// </summary>
		public int Minor { get; set; }

		/// <summary>
		/// Gets or sets the version patch
		/// </summary>
		public int Patch { get; set; }

		public bool Equals(SemanticVersion? rhs)
		{
			if (ReferenceEquals(rhs, null))
				return false;
			return Major == rhs.Major &&
				Minor == rhs.Minor &&
				Patch == rhs.Patch;
		}

		public override bool Equals(object? rhs)
		{
			return Equals(rhs as SemanticVersion);
		}

		public override int GetHashCode()
		{
			return (Major * 0x100000) + (Minor * 0x1000) + Patch;
		}

		public static bool operator ==(SemanticVersion lhs, SemanticVersion rhs)
		{
			if (ReferenceEquals(lhs, null))
				return ReferenceEquals(rhs, null);
			return lhs.Equals(rhs);
		}

		public static bool operator !=(SemanticVersion lhs, SemanticVersion rhs)
		{
			return !(lhs == rhs);
		}

		/// <summary>
		/// Comparison operator
		/// </summary>
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
		/// Convert to string
		/// </summary>
		public override string ToString()
		{
			return $"{Major}.{Minor}.{Patch}";
		}
	}
}
