// <copyright file="RecipeBuildArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	/// <summary>
	/// The set of build arguments for building a recipe
	/// </summary>
	export struct RecipeBuildArguments
	{
		/// <summary>
		/// Gets or sets the flavor name
		/// </summary>
		std::string Flavor;

		/// <summary>
		/// Gets or sets the platform name
		/// </summary>
		std::string Platform;

		/// <summary>
		/// Gets or sets the platform library paths
		/// TODO: Pull this from somewhere when we implement platforms
		/// </summary>
		std::vector<std::string> PlatformIncludePaths;

		/// <summary>
		/// Gets or sets the platform library paths
		/// TODO: Pull this from somewhere when we implement platforms
		/// </summary>
		std::vector<std::string> PlatformLibraryPaths;

		/// <summary>
		/// Gets or sets the platform preprocessor definitions
		/// TODO: Pull this from somewhere when we implement platforms
		/// </summary>
		std::vector<std::string> PlatformPreprocessorDefinitions;

		/// <summary>
		/// Gets or sets the platform libraries
		/// TODO: Pull this from somewhere when we implement platforms
		/// </summary>
		std::vector<std::string> PlatformLibraries;

		/// <summary>
		/// Gets or sets a value indicating whether to force a rebuild
		/// </summary>
		bool ForceRebuild;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const RecipeBuildArguments& rhs) const
		{
			return Flavor == rhs.Flavor &&
				Platform == rhs.Platform &&
				PlatformIncludePaths == rhs.PlatformIncludePaths &&
				PlatformLibraryPaths == rhs.PlatformLibraryPaths &&
				PlatformPreprocessorDefinitions == rhs.PlatformPreprocessorDefinitions &&
				PlatformLibraries == rhs.PlatformLibraries &&
				ForceRebuild == rhs.ForceRebuild;
		}

		bool operator !=(const RecipeBuildArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
