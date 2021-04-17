// <copyright file="CompilerArguments.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

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
	/// The set of compiler arguments
	/// </summary>
	public class CompileArguments : IEquatable<CompileArguments>
	{
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
		/// Gets or sets the list of reference libraries
		/// </summary>
		public IReadOnlyList<Path> ReferenceLibraries { get; init; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of source files
		/// </summary>
		public IReadOnlyList<Path> SourceFiles { get; init; } = new List<Path>();

		/// <summary>
		/// Gets or sets a value indicating whether to generate source debug information
		/// </summary>
		public bool GenerateSourceDebugInfo { get; init; }

		/// <summary>
		/// Gets or sets the target file
		/// </summary>
		public Path Target { get; set; }

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
				this.RootDirectory == rhs.RootDirectory &&
				this.ObjectDirectory == rhs.ObjectDirectory &&
				Enumerable.SequenceEqual(this.PreprocessorDefinitions, rhs.PreprocessorDefinitions) &&
				Enumerable.SequenceEqual(this.ReferenceLibraries, rhs.ReferenceLibraries) &&
				Enumerable.SequenceEqual(this.IncludeModules, rhs.IncludeModules) &&
				this.GenerateSourceDebugInfo == rhs.GenerateSourceDebugInfo &&
				this.InterfaceUnit == rhs.InterfaceUnit &&
				Enumerable.SequenceEqual(this.ImplementationUnits, rhs.ImplementationUnits) &&
				this.EnableWarningsAsErrors == rhs.EnableWarningsAsErrors &&
				Enumerable.SequenceEqual(this.DisabledWarnings, rhs.DisabledWarnings) &&
				Enumerable.SequenceEqual(this.EnabledWarnings, rhs.EnabledWarnings) &&
				Enumerable.SequenceEqual(this.CustomProperties, rhs.CustomProperties);
		}

		public override int GetHashCode() => (Standard, Optimize, RootDirectory, ObjectDirectory, InterfaceUnit).GetHashCode();

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
			return $"SharedCompileArguments {{ Standard={Standard}, Optimize={Optimize}, RootDirectory=\"{RootDirectory}\", ObjectDirectory=\"{ObjectDirectory}\", PreprocessorDefinitions=[{string.Join(",", PreprocessorDefinitions)}], ReferenceLibraries=[{string.Join(",", ReferenceLibraries)}], IncludeModules=[{string.Join(",", IncludeModules)}], GenerateSourceDebugInfo=\"{GenerateSourceDebugInfo}\", InterfaceUnit={InterfaceUnit}, ImplementationUnits=[{string.Join(",", ImplementationUnits)}], EnableWarningsAsErrors=\"{EnableWarningsAsErrors}\", DisabledWarnings=[{string.Join(",", DisabledWarnings)}], EnabledWarnings=[{string.Join(",", EnabledWarnings)}], CustomProperties=[{string.Join(",", CustomProperties)}]}}";
		}
	}
}
