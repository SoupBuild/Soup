// <copyright file="BuildConstants.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Utilities;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The constants used throughout the build
	/// </summary>
	internal static class BuildConstants
	{
		/// <summary>
		/// Gets the Recipe file name
		/// </summary>
		public static Path RecipeFileName => new Path("Recipe.toml");

		/// <summary>
		/// Gets the Generate Parameters Value Table file name
		/// </summary>
		public static Path GenerateParametersFileName => new Path("GenerateParameters.bvt");

		/// <summary>
		/// Gets the Generate Shared State Value Table file name
		/// </summary>
		public static Path GenerateSharedStateFileName => new Path("GenerateSharedState.bvt");

		/// <summary>
		/// Gets the Generate Parameters Value Table file name
		/// </summary>
		public static Path GenerateEvaluateOperationGraphFileName => new Path("GenerateEvaluateGraph.bog");
	}
}
