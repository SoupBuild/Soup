// <copyright file="IBuildStateExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Linq;

namespace Soup.Build
{
	public static class IBuildStateExtensions
	{
		public static void CreateOperation(this IBuildState buildState, BuildOperation operation)
		{
			buildState.CreateOperation(
				operation.Title,
				operation.Executable.ToString(),
				operation.Arguments,
				operation.WorkingDirectory.ToString(),
				operation.DeclaredInput.Select(value => value.ToString()).ToArray(),
				operation.DeclaredOutput.Select(value => value.ToString()).ToArray());
		}
	}
}
