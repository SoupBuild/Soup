// <copyright file="BuildArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;

namespace Soup.Build.Cpp.Compiler
{
	/// <summary>
	/// The enumeration of build optimization levels
	/// </summary>
	public enum BuildOptimizationLevel
	{
		/// <summary>
		/// Debug
		/// </summary>
		None,

		/// <summary>
		/// Optimize for runtime speed, may sacrifice size
		/// </summary>
		Speed,

		/// <summary>
		/// Optimize for speed and size
		/// </summary>
		Size,
	}

	/// <summary>
	/// The enumeration of target types
	/// </summary>
	public enum BuildTargetType
	{
		/// <summary>
		/// Executable
		/// </summary>
		Executable,

		/// <summary>
		/// Static Library
		/// </summary>
		StaticLibrary,

		/// <summary>
		/// Dynamic Library
		/// </summary>
		DynamicLibrary,
	}

	/// <summary>
	/// The source file definition
	/// </summary>
	public class SourceFile
	{
		public Path File { get; set; } = new Path();
		public bool IsModule { get; set; }
	}

	/// <summary>
	/// The set of build arguments
	/// </summary>
	public class BuildArguments
	{
		/// <summary>
		/// Gets or sets the target name
		/// </summary>
		public string TargetName { get; set; } = string.Empty;

		/// <summary>
		/// Gets or sets the target architecture
		/// </summary>
		public string TargetArchitecture { get; set; } = string.Empty;

		/// <summary>
		/// Gets or sets the target type
		/// </summary>
		public BuildTargetType TargetType { get; set; }

		/// <summary>
		/// Gets or sets the language standard
		/// </summary>
		public LanguageStandard LanguageStandard { get; set; }

		/// <summary>
		/// Gets or sets the working directory
		/// </summary>
		public Path WorkingDirectory { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the output object directory
		/// </summary>
		public Path ObjectDirectory { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the output binary directory
		/// </summary>
		public Path BinaryDirectory { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the single module interface source file
		/// </summary>
		public Path ModuleInterfaceSourceFile { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the list of source files
		/// Note: These files can be plain old translation units 
		/// or they can be module implementation units
		/// </summary>
		public IReadOnlyList<Path> SourceFiles { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of include directories
		/// </summary>
		public IReadOnlyList<Path> IncludeDirectories { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of module dependencies
		/// </summary>
		public IReadOnlyList<Path> ModuleDependencies { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of platform link libraries
		/// Note: These libraries will be included at link time, but will not be an input
		/// for the incremental builds
		/// </summary>
		public IReadOnlyList<Path> PlatformLinkDependencies { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of link libraries
		/// </summary>
		public IReadOnlyList<Path> LinkDependencies { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of library paths
		/// </summary>
		public IReadOnlyList<Path> LibraryPaths { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of preprocessor definitions
		/// </summary>
		public IReadOnlyList<string> PreprocessorDefinitions { get; set; } = new List<string>();

		/// <summary>
		/// Gets or sets the list of runtime dependencies
		/// </summary>
		public IReadOnlyList<Path> RuntimeDependencies { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the optimization level
		/// </summary>
		public BuildOptimizationLevel OptimizationLevel { get; set; }

		/// <summary>
		/// Gets or sets a value indicating whether to generate source debug information
		/// </summary>
		public bool GenerateSourceDebugInfo { get; set; }

		/// <summary>
		/// Gets or sets a value indicating whether to enable warnings as errors
		/// </summary>
		public bool EnableWarningsAsErrors { get; set; }

		/// <summary>
		/// Gets or sets the list of disabled warnings
		/// </summary>
		public IReadOnlyList<string> DisabledWarnings { get; set; } = new List<string>();

		/// <summary>
		/// Gets or sets the list of enabled warnings
		/// </summary>
		public IReadOnlyList<string> EnabledWarnings { get; set; } = new List<string>();

		/// <summary>
		/// Gets or sets the set of custom properties for the known compiler
		/// </summary>
		public IReadOnlyList<string> CustomProperties { get; set; } = new List<string>();
	}
}
