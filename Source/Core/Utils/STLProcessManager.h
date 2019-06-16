// <copyright file="STLProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Soup
{
    /// <summary>
    /// The standard library fake process executable using system
    /// </summary>
    export class STLProcessManager : public IProcessManager
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref='STLProcessManager'/> class.
        /// </summary>
        STLProcessManager()
        {
        }

        /// <summary>
        /// Creates a process for the provided executable path
        /// </summary>
        virtual bool Execute(const Path& path, const std::vector<std::string>& arguments) override final
        {
            // TODO: Switch to platform specific CreateProcess
            return std::filesystem::exists(path.ToString());
        }
    };
}
