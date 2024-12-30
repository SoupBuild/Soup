// <copyright file="ViewOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SharedOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// View Command
	/// </summary>
	// TODO: [Verb("view")]
	class ViewOptions : public SharedOptions
	{
	public:
		/// <summary>
		/// Gets or sets the path to run
		/// </summary>
		// [[Args::Option("path", Index = 0, HelpText = "Path to the package to run.")]]
		std::string Path;
	};
}
