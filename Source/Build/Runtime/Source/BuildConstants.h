// <copyright file="BuildConstants.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The constants used throughout the build
	/// </summary>
	export class BuildConstants
	{
	public:
		/// <summary>
		/// Gets the Recipe file name
		/// </summary>
		static const Path& RecipeFileName()
		{
			static const Path value = Path("Recipe.toml");
			return value;
		}

		/// <summary>
		/// Gets the Generate Parameters Value Table file name
		/// </summary>
		static const Path GenerateParametersFileName()
		{
			static const Path value = Path("GenerateParameters.bvt");
			return value;
		}

		/// <summary>
		/// Gets the Generate Shared State Value Table file name
		/// </summary>
		static const Path GenerateSharedStateFileName()
		{
			static const Path value = Path("GenerateSharedState.bvt");
			return value;
		}

		/// <summary>
		/// Gets the Generate Parameters Value Table file name
		/// </summary>
		static const Path EvaluateOperationGraphFileName()
		{
			static const Path value = Path("EvaluateGraph.bog");
			return value;
		}
	};
}
