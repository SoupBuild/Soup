// <copyright file="BuildConstants.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The constants used throughout the build
	/// </summary>
	public static class BuildConstants
	{
		/// <summary>
		/// Gets the Recipe file name
		/// </summary>
		public static Path RecipeFileName => new Path("Recipe.sml");

		/// <summary>
		/// Gets the Package Lock file name
		/// </summary>
		public static Path PackageLockFileName => new Path("PackageLock.sml");

		/// <summary>
		/// Gets the Generate Parameters Value Table file name
		/// </summary>
		public static Path GenerateParametersFileName => new Path("GenerateParameters.bvt");

		/// <summary>
		/// Gets the Generate Read Access file name
		/// </summary>
		public static Path GenerateReadAccessFileName => new Path("GenerateReadAccess.txt");

		/// <summary>
		/// Gets the Generate Write Access file name
		/// </summary>
		public static Path GenerateWriteAccessFileName => new Path("GenerateWriteAccess.txt");

		/// <summary>
		/// Gets the Generate Shared State Value Table file name
		/// </summary>
		public static Path GenerateSharedStateFileName => new Path("GenerateSharedState.bvt");

		/// <summary>
		/// Gets the Evaluate Graph file name
		/// </summary>
		public static Path EvaluateGraphFileName => new Path("Evaluate.bog");

		/// <summary>
		/// Gets the Evaluate Results file name
		/// </summary>
		public static Path EvaluateResultsFileName => new Path("Evaluate.bor");

		/// <summary>
		/// Gets the Generate task info Value Table file name
		/// </summary>
		public static Path GenerateTaskInfoFileName => new Path("GenerateTaskInfo.bvt");
	}
}
