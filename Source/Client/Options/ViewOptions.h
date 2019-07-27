// <copyright file="ViewOptions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SharedOptions.h"

namespace Soup::Client
{
    /// <summary>
    /// View Command Options
    /// </summary>
    // TODO: [Verb("version")]
    class ViewOptions : public SharedOptions
    {
    public:
        /// <summary>
        /// Gets or sets the package name
        /// </summary>
        const std::string& GetPackage()
        {
            return _package;
        }

        /// <summary>
        /// Show the usage details for this command
        /// </summary>
        static void ShowUsage()
        {
            std::cout << std::endl;
            std::cout << "Usage: soup view <package>" << std::endl;
            std::cout << "\tpackage: The unique package name." << std::endl;
        }

    private:
        std::string _package;
    };
}
