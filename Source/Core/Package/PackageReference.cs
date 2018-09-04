// <copyright file="PackageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
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
        /// <summary>
        /// Initializes a new instance of the <see cref="PackageReference"/> class.
        /// </summary>
        public PackageReference()
        {
            Name = null;
            Version = null;
            Path = null;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PackageReference"/> class.
        /// </summary>
        public PackageReference(string name, SemanticVersion version)
        {
            Name = name;
            Version = version;
            Path = null;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PackageReference"/> class.
        /// </summary>
        public PackageReference(string path)
        {
            Name = null;
            Version = null;
            Path = path;
        }

        /// <summary>
        /// Gets or sets the Name.
        /// </summary>
        [JsonProperty("name")]
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the Version.
        /// </summary>
        [JsonProperty("version")]
        public SemanticVersion Version { get; set; }

        /// <summary>
        /// Gets or sets the Path.
        /// </summary>
        [JsonProperty("path")]
        public string Path { get; set; }

        /// <summary>
        /// Equality operator
        /// </summary>
        public static bool operator ==(PackageReference lhs, PackageReference rhs)
        {
            return lhs.Name == rhs.Name &&
                lhs.Version == rhs.Version &&
                lhs.Path == rhs.Path;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        public static bool operator !=(PackageReference lhs, PackageReference rhs)
        {
            return lhs.Name != rhs.Name ||
                lhs.Version != rhs.Version ||
                lhs.Path != rhs.Path;
        }

        /// <summary>
        /// Try parse a package reference from the provided string
        /// </summary>
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

        /// <summary>
        /// Parse a package reference from the provided string.
        /// </summary>
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

        /// <summary>
        /// Check if the provided object is equal
        /// </summary>
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

        /// <summary>
        /// Get hash code
        /// </summary>
        public override int GetHashCode()
        {
            return Name.GetHashCode() ^ Version.GetHashCode() ^ Path.GetHashCode();
        }

        /// <summary>
        /// Convert to string
        /// </summary>
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
