// <copyright file="PlatformProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Soup
{
    /// <summary>
    /// A platform specific process executable using system
    /// </summary>
    export class PlatformProcessManager : public IProcessManager
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref='PlatformProcessManager'/> class.
        /// </summary>
        PlatformProcessManager()
        {
        }

        /// <summary>
        /// Creates a process for the provided executable path
        /// </summary>
        virtual int Execute(
            const Path& application,
            const std::vector<std::string>& arguments,
            const Path& workingDirectory) override final
        {
            std::stringstream argumentsValue;
            for (auto& value : arguments)
                argumentsValue << " " << value;

            std::string argumentsString = argumentsValue.str();
            Log::Verbose(workingDirectory.ToString() + ": " + application.ToString() + " " + argumentsString);
            return PlatformProcessManagerImpl::Execute(
                application.ToString().c_str(),
                const_cast<char*>(argumentsString.c_str()),
                workingDirectory.ToString().c_str());
        }
    };
}
