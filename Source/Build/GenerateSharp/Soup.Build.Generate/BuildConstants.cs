// <copyright file="BuildConstants.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
		public static string RecipeFileName => "Recipe.toml";

		/// <summary>
		/// Gets the Generate Parameters Value Table file name
		/// </summary>
		public static string GenerateParametersFileName => "GenerateParameters.bvt";

		/// <summary>
		/// Gets the Generate Shared State Value Table file name
		/// </summary>
		public static string GenerateSharedStateFileName => "GenerateSharedState.bvt";

		/// <summary>
		/// Gets the Generate Parameters Value Table file name
		/// </summary>
		public static string GenerateEvaluateOperationGraphFileName => "GenerateEvaluateGraph.bog";
	}
}
