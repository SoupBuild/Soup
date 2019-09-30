// <copyright file="BuildOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SharedOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Build Command Options
	/// </summary>
	// TODO: [[Verb("build")]]
	class BuildOptions : public SharedOptions
	{
	public:
		/// <summary>
		/// Gets or sets a value indicating whether to force a build
		/// </summary>
		[[Option('f', "force", Default = false, HelpText = "Force a rebuild.")]]
		bool Force;
	};
}
