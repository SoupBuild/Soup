// <copyright file="CompilerArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

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
	public class TranslationUnitCompileArguments : IEquatable<TranslationUnitCompileArguments>
	{
		/// <summary>
		/// Gets or sets the source file
		/// </summary>
		public Path SourceFile { get; set; } = new Path();

		/// <summary>
		/// Gets or sets the target file
		/// </summary>
		public Path TargetFile { get; set; } = new Path();

		public override bool Equals(object? obj) => this.Equals(obj as TranslationUnitCompileArguments);

		public bool Equals(TranslationUnitCompileArguments? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.SourceFile == rhs.SourceFile &&
				this.TargetFile == rhs.TargetFile;
		}

		public override int GetHashCode() => (SourceFile, TargetFile).GetHashCode();

		public static bool operator ==(TranslationUnitCompileArguments? lhs, TranslationUnitCompileArguments? rhs)
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

		public static bool operator !=(TranslationUnitCompileArguments? lhs, TranslationUnitCompileArguments? rhs) => !(lhs == rhs);
	}

	/// <summary>
	/// The set of file specific compiler arguments
	/// </summary>
	public class InterfaceUnitCompileArguments : TranslationUnitCompileArguments, IEquatable<InterfaceUnitCompileArguments>
	{
		/// <summary>
		/// Gets or sets the source file
		/// </summary>
		public Path ModuleInterfaceTarget { get; set; } = new Path();

		public override bool Equals(object? obj) => this.Equals(obj as InterfaceUnitCompileArguments);

		public bool Equals(InterfaceUnitCompileArguments? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.SourceFile == rhs.SourceFile &&
				this.TargetFile == rhs.TargetFile && 
				this.ModuleInterfaceTarget == rhs.ModuleInterfaceTarget;
		}

		public override int GetHashCode() => (SourceFile, TargetFile).GetHashCode();

		public static bool operator ==(InterfaceUnitCompileArguments? lhs, InterfaceUnitCompileArguments? rhs)
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

		public static bool operator !=(InterfaceUnitCompileArguments? lhs, InterfaceUnitCompileArguments? rhs) => !(lhs == rhs);

		public override string ToString()
		{
			return $"InterfaceUnitCompileArguments {{ SourceFile=\"{SourceFile}\", TargetFile=\"{TargetFile}\", ModuleInterfaceTarget=\"{ModuleInterfaceTarget}\" }}";
		}
	}

	/// <summary>
	/// The set of shared compiler arguments
	/// </summary>
	public class SharedCompileArguments : IEquatable<SharedCompileArguments>
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
		/// Gets or sets the source directory
		/// </summary>
		public Path SourceRootDirectory { get; init; } = new Path();

		/// <summary>
		/// Gets or sets the target directory
		/// </summary>
		public Path TargetRootDirectory { get; init; } = new Path();

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

		public override bool Equals(object? obj) => this.Equals(obj as SharedCompileArguments);

		public bool Equals(SharedCompileArguments? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.Standard == rhs.Standard &&
				this.Optimize == rhs.Optimize &&
				this.SourceRootDirectory == rhs.SourceRootDirectory &&
				this.TargetRootDirectory == rhs.TargetRootDirectory &&
				this.ObjectDirectory == rhs.ObjectDirectory &&
				Enumerable.SequenceEqual(this.PreprocessorDefinitions, rhs.PreprocessorDefinitions) &&
				Enumerable.SequenceEqual(this.IncludeDirectories, rhs.IncludeDirectories) &&
				Enumerable.SequenceEqual(this.IncludeModules, rhs.IncludeModules) &&
				this.GenerateSourceDebugInfo == rhs.GenerateSourceDebugInfo &&
				this.InterfaceUnit == rhs.InterfaceUnit &&
				Enumerable.SequenceEqual(this.ImplementationUnits, rhs.ImplementationUnits) &&
				this.EnableWarningsAsErrors == rhs.EnableWarningsAsErrors &&
				Enumerable.SequenceEqual(this.DisabledWarnings, rhs.DisabledWarnings) &&
				Enumerable.SequenceEqual(this.EnabledWarnings, rhs.EnabledWarnings) &&
				Enumerable.SequenceEqual(this.CustomProperties, rhs.CustomProperties);
		}

		public override int GetHashCode() => (Standard, Optimize, SourceRootDirectory, TargetRootDirectory, ObjectDirectory, InterfaceUnit).GetHashCode();

		public static bool operator ==(SharedCompileArguments? lhs, SharedCompileArguments? rhs)
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

		public static bool operator !=(SharedCompileArguments? lhs, SharedCompileArguments? rhs) => !(lhs == rhs);

		public override string ToString()
		{
			return $"SharedCompileArguments {{ Standard={Standard}, Optimize={Optimize}, SourceRootDirectory=\"{SourceRootDirectory}\", TargetRootDirectory=\"{TargetRootDirectory}\", ObjectDirectory=\"{ObjectDirectory}\", PreprocessorDefinitions=[{string.Join(",", PreprocessorDefinitions)}], IncludeDirectories=[{string.Join(",", IncludeDirectories)}], IncludeModules=[{string.Join(",", IncludeModules)}], GenerateSourceDebugInfo=\"{GenerateSourceDebugInfo}\", InterfaceUnit={InterfaceUnit}, ImplementationUnits=[{string.Join(",", ImplementationUnits)}], EnableWarningsAsErrors=\"{EnableWarningsAsErrors}\", DisabledWarnings=[{string.Join(",", DisabledWarnings)}], EnabledWarnings=[{string.Join(",", EnabledWarnings)}], CustomProperties=[{string.Join(",", CustomProperties)}]}}";
		}
	}
}
