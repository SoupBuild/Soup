// <copyright file="BuildOperation.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	public class BuildOperation
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
	}
}
