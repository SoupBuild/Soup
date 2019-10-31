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
		/// Gets or sets the executable path
		/// </summary>
		[[Args::ToolPath]]
		std::string ExecutablePath;

		/// <summary>
		/// Gets or sets the path to build
		/// </summary>
		[[Args::Option("path", Index = 0, HelpText = "Path to the package to build.")]]
		std::string Path;

		/// <summary>
		/// Gets or sets a value indicating whether to force a build
		/// </summary>
		[[Args::Option('f', "force", Default = false, HelpText = "Force a rebuild.")]]
		bool Force;

		/// <summary>
		/// Gets or sets a value indicating what configuration to use
		/// </summary>
		[[Args::Option('c', "config", Default = false, HelpText = "Configuration.")]]
		std::string Configuration;
	};
}
