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
		/// Gets or sets the system name
		/// </summary>
		std::string System;

		/// <summary>
		/// Gets or sets the architecture name
		/// </summary>
		std::string Architecture;

		/// <summary>
		/// Gets or sets a value indicating whether to skip running the build operations
		/// </summary>
		bool SkipRun;

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
				System == rhs.System &&
				Architecture == rhs.Architecture &&
				SkipRun == rhs.SkipRun &&
				ForceRebuild == rhs.ForceRebuild;
		}

		bool operator !=(const RecipeBuildArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
