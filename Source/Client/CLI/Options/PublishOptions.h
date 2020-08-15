// <copyright file="PublishOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SharedOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Publish Command Options
	/// </summary>
	// TODO: [Verb("publish")]
	class PublishOptions : public SharedOptions
	{
		/// <summary>
		/// Gets or sets the path to publish
		/// </summary>
		[[Args::Option("path", Index = 0, HelpText = "Path to the package to publish.")]]
		std::string Path;
	};
}
