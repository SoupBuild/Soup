// <copyright file="CompilerArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System.Collections.Generic;

namespace Soup.Build.Cpp.Compiler
{
	/// <summary>
	/// The enumeration of language standards
	/// </summary>
	public enum LanguageStandard
	{
		/// <summary>
		/// C++ 11
		/// </summary>
		CPP11,

		/// <summary>
		/// C++ 14
		/// </summary>
		CPP14,

		/// <summary>
		/// C++ 17
		/// </summary>
		CPP17,

		/// <summary>
		/// C++ 20
		/// </summary>
		CPP20,
	}

	/// <summary>
	/// The enumeration of optimization levels
	/// </summary>
	public enum OptimizationLevel
	{
		/// <summary>
		/// Disable all optimization for build speed and debugability
		/// </summary>
		None,

		/// <summary>
		/// Optimize for speed
		/// </summary>
		Speed,

		/// <summary>
		/// Optimize for size
		/// </summary>
		Size,
	}

	/// <summary>
	/// The set of file specific compiler arguments
	/// </summary>
	public class TranslationUnitCompileArguments
	{
		/// <summary>
		/// Gets or sets the source file
		/// </summary>
		public Path SourceFile { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the target file
		/// </summary>
		public Path TargetFile { get; set; } = new Path();
	}

	/// <summary>
	/// The set of file specific compiler arguments
	/// </summary>
	public class InterfaceUnitCompileArguments : TranslationUnitCompileArguments
	{
		/// <summary>
		/// Gets or sets the source file
		/// </summary>
		public Path ModuleInterfaceTarget { get; init; } = new Path();
	}

	/// <summary>
	/// The set of shared compiler arguments
	/// </summary>
	public class SharedCompileArguments
	{
		/// <summary>
		/// Gets or sets the language standard
		/// </summary>
		public LanguageStandard Standard { get; init; }

		/// <summary>
		/// Gets or sets the optimization level
		/// </summary>
		public OptimizationLevel Optimize { get; init; }

		/// <summary>
		/// Gets or sets the root directory
		/// </summary>
		public Path RootDirectory { get; init; } = new Path();

		/// <summary>
		/// Gets or sets the object directory
		/// </summary>
		public Path ObjectDirectory { get; init; } = new Path();

		/// <summary>
		/// Gets or sets the list of preprocessor definitions
		/// </summary>
		public IReadOnlyList<string> PreprocessorDefinitions { get; init; } = new List<string>();

		/// <summary>
		/// Gets or sets the list of include directories
		/// </summary>
		public IReadOnlyList<Path> IncludeDirectories { get; init; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of modules
		/// </summary>
		public IReadOnlyList<Path> IncludeModules { get; init; } = new List<Path>();

		/// <summary>
		/// Gets or sets a value indicating whether to generate source debug information
		/// </summary>
		public bool GenerateSourceDebugInfo { get; init; }

		/// <summary>
		/// Gets or sets the single optional interface unit to compile
		/// </summary>
		public InterfaceUnitCompileArguments? InterfaceUnit { get; set; }

		/// <summary>
		/// Gets or sets the list of individual translation units to compile
		/// </summary>
		public IReadOnlyList<TranslationUnitCompileArguments> ImplementationUnits { get; set; } = new List<TranslationUnitCompileArguments>();

		/// <summary>
		/// Gets or sets a value indicating whether to enable warnings as errors
		/// </summary>
		public bool EnableWarningsAsErrors { get; init; }

		/// <summary>
		/// Gets or sets the list of disabled warnings
		/// </summary>
		public IReadOnlyList<string> DisabledWarnings { get; set; } = new List<string>();

		/// <summary>
		/// Gets or sets the list of enabled warnings
		/// </summary>
		public IReadOnlyList<string> EnabledWarnings { get; init; } = new List<string>();

		/// <summary>
		/// Gets or sets the set of custom properties for the known compiler
		/// </summary>
		public IReadOnlyList<string> CustomProperties { get; init; } = new List<string>();
	}
}
