// <copyright file="RestoreOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SharedOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Restore Command
	/// </summary>
	// TODO: [Verb("restore")]
	class RestoreOptions : public SharedOptions
	{
	public:
		/// <summary>
		/// Gets or sets the path to restore
		/// </summary>
		// [[Args::Option("path", Index = 0, HelpText = "Path to the package to restore.")]]
		std::string Path;

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		static void ShowUsage()
		{
			std::cout << std::endl;
			std::cout << "Usage: soup restore <package_file>" << std::endl;
			std::cout << "\tpackage_file: Must be a zip file." << std::endl;
		}

	private:
		std::string _package;
	};
}
