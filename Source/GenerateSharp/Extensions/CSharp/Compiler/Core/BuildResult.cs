// <copyright file="BuildResult.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System.Collections.Generic;

namespace Soup.Build.CSharp.Compiler
{
	/// <summary>
	/// The build result
	/// </summary>
	public class BuildResult
	{
		/// <summary>
		/// Gets or sets the resulting root build operations
		/// </summary>
		public IList<BuildOperation> BuildOperations { get; set; } = new List<BuildOperation>();

		/// <summary>
		/// Gets or sets the list of link libraries that downstream builds should use when linking
		/// </summary>
		public IList<Path> LinkDependencies { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of internal link libraries that were used to link the final result
		/// </summary>
		public IList<Path> InternalLinkDependencies { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the list of runtime dependencies
		/// </summary>
		public IList<Path> RuntimeDependencies { get; set; } = new List<Path>();

		/// <summary>
		/// Gets or sets the target file for the build
		/// </summary>
		public Path TargetFile { get; set; } = new Path();
	}
}
