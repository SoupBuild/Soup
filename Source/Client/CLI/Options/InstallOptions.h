// <copyright file="InstallOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SharedOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Install Command
	/// </summary>
	// TODO: [Verb("install")]
	class InstallOptions : public SharedOptions
	{
	public:
		/// <summary>
		/// Gets or sets the path to install
		/// </summary>
		[[Args::Option("path", Index = 0, HelpText = "Path to the package to install.")]]
		std::string Path;

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		[[Args::Option(Index = 0, HelpText = "The package reference to install.")]]
		std::string PackageReference;

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		static void ShowUsage()
		{
			std::cout << std::endl;
			std::cout << "Usage: soup install <package_file>" << std::endl;
			std::cout << "\tpackage_file: Must be a zip file." << std::endl;
		}

	private:
		std::string _package;
	};
}
