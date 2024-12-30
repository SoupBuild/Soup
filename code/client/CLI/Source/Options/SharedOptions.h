// <copyright file="SharedOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Client
{
	/// <summary>
	/// Shared Options for all Commands
	/// </summary>
	class SharedOptions
	{
	public:
		virtual ~SharedOptions()
		{
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		// TODO: [Option('v', "verbose", Default = false, HelpText = "Enable verbose logging.")]
		TraceEventFlag Verbosity;
	};
}
