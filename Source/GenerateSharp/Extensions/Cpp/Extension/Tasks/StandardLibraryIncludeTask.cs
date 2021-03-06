// <copyright file="StandardLibraryIncludeTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build.Cpp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	public class StandardLibraryIncludeTask : IBuildTask
	{
		/// <summary>
		/// Get the run before list
		/// </summary>
		public IReadOnlyList<string> RunBeforeList => new List<string>()
		{
		};

		/// <summary>
		/// Get the run after list
		/// </summary>
		public IReadOnlyList<string> RunAfterList => new List<string>()
		{
		};

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
		}
	}
}
