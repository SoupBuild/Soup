﻿// <copyright file="RecipeBuildArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Path.h"

namespace Soup
{
	/// <summary>
	/// The set of build arguments for building a recipe
	/// </summary>
	export struct RecipeBuildArguments
	{
		/// <summary>
		/// Gets or sets the configuration name
		/// </summary>
		std::string Configuration;

		/// <summary>
		/// Gets or sets a value indicating whether to force a rebuild
		/// </summary>
		bool ForceRebuild;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const RecipeBuildArguments& rhs) const
		{
			return Configuration == rhs.Configuration &&
				ForceRebuild == rhs.ForceRebuild;
		}

		bool operator !=(const RecipeBuildArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
