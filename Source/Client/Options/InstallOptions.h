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
        /// Gets or sets the package name
        /// </summary>
        const std::string& GetPackage()
        {
            return m_package;
        }

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
        std::string m_package;
    };
}
