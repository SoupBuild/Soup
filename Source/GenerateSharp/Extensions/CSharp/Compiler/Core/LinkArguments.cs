// <copyright file="LinkArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Utilities;
using System.Collections.Generic;

namespace Soup.Build.CSharp.Compiler
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
	public class LinkArguments
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
	}
}
