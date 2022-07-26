// <copyright file="SemanticVersion.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal
{
    using global::System;
    using global::System.Collections.Generic;

    /// <summary>
    /// The semantic version class.
    /// </summary>
    public class SemanticVersion : IEquatable<SemanticVersion>
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SemanticVersion"/> class.
        /// </summary>
        public SemanticVersion()
            : this(0, 0, 0)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="SemanticVersion"/> class.
        /// </summary>
        /// <param name="major">The major version.</param>
        /// <param name="minor">The minor version.</param>
        /// <param name="patch">The patch version.</param>
        public SemanticVersion(int major, int minor, int patch)
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
        public int Minor { get; set; }

        /// <summary>
        /// Gets or sets the version patch.
        /// </summary>
        public int Patch { get; set; }

        /// <summary>
        /// Try parse the value.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <param name="result">The output parameter result.</param>
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
        /// Parse the value.
        /// </summary>
        /// <param name="value">The value.</param>
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
                if (int.TryParse(stringValue, out var intValue))
                {
                    intValues.Add(intValue);
                }
                else
                {
                    throw new ArgumentException($"Invalid version string: {value}");
                }
            }

            return new SemanticVersion(
                intValues[0],
                intValues[1],
                intValues[2]);
        }

        public bool Equals(SemanticVersion? rhs)
        {
            if (ReferenceEquals(rhs, null))
                return false;
            return this.Major == rhs.Major &&
                this.Minor == rhs.Minor &&
                this.Patch == rhs.Patch;
        }

        public override bool Equals(object? rhs)
        {
            return this.Equals(rhs as SemanticVersion);
        }

        public override int GetHashCode()
        {
            return (this.Major * 0x100000) + (this.Minor * 0x1000) + this.Patch;
        }

        public static bool operator ==(SemanticVersion? lhs, SemanticVersion? rhs)
        {
            if (ReferenceEquals(lhs, null))
                return ReferenceEquals(rhs, null);
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
            return $"{this.Major}.{this.Minor}.{this.Patch}";
        }
    }
}
