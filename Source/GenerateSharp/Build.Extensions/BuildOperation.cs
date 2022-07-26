// <copyright file="BuildOperation.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build
{
	public class BuildOperation : IEquatable<BuildOperation>
	{
		public BuildOperation(
			string title,
			Path workingDirectory,
			Path executable,
			string arguments,
			IList<Path> declaredInput,
			IList<Path> declaredOutput)
		{
			Title = title;
			WorkingDirectory = workingDirectory;
			Executable = executable;
			Arguments = arguments;
			DeclaredInput = declaredInput;
			DeclaredOutput = declaredOutput;
		}

		public string Title { get; init; }
		public Path WorkingDirectory { get; init; }
		public Path Executable { get; init; }
		public string Arguments { get; init; }
		public IList<Path> DeclaredInput { get; init; }
		public IList<Path> DeclaredOutput { get; init; }

		public override bool Equals(object? obj) => this.Equals(obj as BuildOperation);

		public bool Equals(BuildOperation? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.Title == rhs.Title &&
				this.WorkingDirectory == rhs.WorkingDirectory &&
				this.Executable == rhs.Executable &&
				this.Arguments == rhs.Arguments &&
				Enumerable.SequenceEqual(this.DeclaredInput, rhs.DeclaredInput) &&
				Enumerable.SequenceEqual(this.DeclaredOutput, rhs.DeclaredOutput);
		}

		public override int GetHashCode() => (Title, WorkingDirectory, Executable, Arguments).GetHashCode();

		public static bool operator ==(BuildOperation? lhs, BuildOperation? rhs)
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

		public static bool operator !=(BuildOperation? lhs, BuildOperation? rhs) => !(lhs == rhs);

		public override string ToString()
		{
			return $"BuildOperation {{ Title=\"{Title}\", WorkingDirectory=\"{WorkingDirectory}\", Executable=\"{Executable}\", Arguments=\"{Arguments}\", DeclaredInput=[{string.Join(",", DeclaredInput)}], DeclaredOutput=[{string.Join(",", DeclaredOutput)}] }}";
		}
	}
}
