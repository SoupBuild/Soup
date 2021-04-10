// <copyright file="LinkArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Cpp.Compiler
{
	/// <summary>
	/// The enumeration of link targets
	/// </summary>
	public enum LinkTarget
	{
		/// <summary>
		/// Static Library
		/// </summary>
		StaticLibrary,

		/// <summary>
		/// Dynamic Library
		/// </summary>
		DynamicLibrary,

		/// <summary>
		/// Executable
		/// </summary>
		Executable,
	}

	/// <summary>
	/// The shared link arguments
	/// </summary>
	public class LinkArguments : IEquatable<LinkArguments>
	{
		/// <summary>
		/// Gets or sets the target file
		/// </summary>
		public Path TargetFile { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the target type
		/// </summary>
		public LinkTarget TargetType { get; set; }

		/// <summary>
		/// Gets or sets the implementation file
		/// </summary>
		public Path ImplementationFile { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the root directory
		/// </summary>
		public Path RootDirectory { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the target architecture
		/// </summary>
		public string TargetArchitecture { get; set; } = string.Empty;

		/// <summary>
		/// Gets or sets the list of object files
		/// </summary>
		public IReadOnlyList<Path> ObjectFiles { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of library files
		/// </summary>
		public IReadOnlyList<Path> LibraryFiles { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of external library files
		/// </summary>
		public IReadOnlyList<Path> ExternalLibraryFiles { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of library paths
		/// </summary>
		public IReadOnlyList<Path> LibraryPaths { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets a value indicating whether to generate source debug information
		/// </summary>
		public bool GenerateSourceDebugInfo { get; set; }

		public override bool Equals(object? obj) => this.Equals(obj as LinkArguments);

		public bool Equals(LinkArguments? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.TargetFile == rhs.TargetFile &&
				this.TargetType == rhs.TargetType &&
				this.ImplementationFile == rhs.ImplementationFile &&
				this.RootDirectory == rhs.RootDirectory &&
				this.TargetArchitecture == rhs.TargetArchitecture &&
				Enumerable.SequenceEqual(this.ObjectFiles, rhs.ObjectFiles) &&
				Enumerable.SequenceEqual(this.LibraryFiles, rhs.LibraryFiles) &&
				Enumerable.SequenceEqual(this.ExternalLibraryFiles, rhs.ExternalLibraryFiles) &&
				Enumerable.SequenceEqual(this.LibraryPaths, rhs.LibraryPaths) &&
				this.GenerateSourceDebugInfo == rhs.GenerateSourceDebugInfo;
		}

		public override int GetHashCode() => (TargetFile, TargetType, ImplementationFile, RootDirectory, TargetArchitecture).GetHashCode();

		public static bool operator ==(LinkArguments? lhs, LinkArguments? rhs)
		{
			if (lhs is null)
			{
				if (rhs is null)
					return true;
				else
					return false;
			}

			return lhs.Equals(rhs);
		}

		public static bool operator !=(LinkArguments? lhs, LinkArguments? rhs) => !(lhs == rhs);
	}
}
