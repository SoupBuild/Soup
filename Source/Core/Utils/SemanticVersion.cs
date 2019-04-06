// <copyright file="SemanticVersion.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System;

    /// <summary>
    /// The semantic version class
    /// </summary>
    public class SemanticVersion
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SemanticVersion"/> class.
        /// </summary>
        public SemanticVersion(short major, short minor, short patch)
        {
            Major = major;
            Minor = minor;
            Patch = patch;
        }

        /// <summary>
        /// Gets or sets the version major
        /// </summary>
        public short Major { get; set; }

        /// <summary>
        /// Gets or sets the version minor
        /// </summary>
        public short Minor { get; set; }

        /// <summary>
        /// Gets or sets the version patch
        /// </summary>
        public short Patch { get; set; }

        /// <summary>
        /// Equality operator
        /// </summary>
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

        /// <summary>
        /// Inequality operator
        /// </summary>
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

        /// <summary>
        /// Try parse the value
        /// </summary>
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

        /// <summary>
        /// Parse the value
        /// </summary>
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

        /// <summary>
        /// Equals check
        /// </summary>
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

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Major ^ Minor ^ Patch;
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
