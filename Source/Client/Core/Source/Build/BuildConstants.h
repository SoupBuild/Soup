// <copyright file="BuildConstants.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
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
		/// Gets the Package Lock file name
		/// </summary>
		static const Path& PackageLockFileName()
		{
			static const Path value = Path("PackageLock.toml");
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
		/// Gets the Generate Read Access file name
		/// </summary>
		static const Path GenerateReadAccessFileName()
		{
			static const Path value = Path("GenerateReadAccess.txt");
			return value;
		}

		/// <summary>
		/// Gets the Generate Write Access file name
		/// </summary>
		static const Path GenerateWriteAccessFileName()
		{
			static const Path value = Path("GenerateWriteAccess.txt");
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
		static const Path GenerateEvaluateOperationGraphFileName()
		{
			static const Path value = Path("GenerateEvaluateGraph.bog");
			return value;
		}

		/// <summary>
		/// Gets the Local User Config file name
		/// </summary>
		static const Path LocalUserConfigFileName()
		{
			static const Path value = Path("LocalUserConfig.toml");
			return value;
		}

		/// <summary>
		/// Gets the soup target directory
		/// </summary>
		static Path GetSoupLocalStoreDirectory()
		{
			static const auto value = Path(".soup/");
			return value;
		}

		static Path GetSoupTargetDirectory()
		{
			static const auto value = Path(".soup/");
			return value;
		}

		static Path GetGenerateGraphFileName()
		{
			static const auto value = Path("GenerateGraph.bog");
			return value;
		}

		static Path GetEvaluateResultGraphFileName()
		{
			static const auto value = Path("EvaluateResultGraph.bog");
			return value;
		}

		static Path GetTemporaryFolderName()
		{
			static const auto value = Path("temp/");
			return value;
		}
	};
}
