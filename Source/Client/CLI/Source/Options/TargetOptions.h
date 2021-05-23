// <copyright file="TargetOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SharedOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Target Command Options
	/// </summary>
	// TODO: [[Verb("target")]]
	class TargetOptions : public SharedOptions
	{
	public:
		/// <summary>
		/// Gets or sets the path to target
		/// </summary>
		// [[Args::Option("path", Index = 0, HelpText = "Path to the package to target.")]]
		std::string Path;

		/// <summary>
		/// Gets or sets a value indicating what flavor to use
		/// </summary>
		// [[Args::Option('f', "flavor", Default = false, HelpText = "Flavor.")]]
		std::string Flavor;

		/// <summary>
		/// Gets or sets a value indicating what target system
		/// </summary>
		// [[Args::Option('s', "system", Default = false, HelpText = "System.")]]
		std::string System;

		/// <summary>
		/// Gets or sets a value indicating what target architecture
		/// </summary>
		// [[Args::Option('a', "architecture", Default = false, HelpText = "Architecture.")]]
		std::string Architecture;
	};
}
