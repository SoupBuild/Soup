// <copyright file="MockProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Soup
{
    /// <summary>
    /// The mock process manager
    /// TODO: Move into test project
    /// </summary>
    export class MockProcessManager : public IProcessManager
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref='MockProcessManager'/> class.
        /// </summary>
        MockProcessManager() :
            _requests()
        {
        }

        /// <summary>
        /// Get the load requests
        /// </summary>
        const std::vector<std::string>& GetRequests() const
        {
            return _requests;
        }

        /// <summary>
        /// Creates a process for the provided executable path
        /// </summary>
        virtual bool Execute(const Path& path, const std::vector<std::string>& arguments) override final
        {
            _requests.push_back(path.ToString());
            return true;
        }

    private:
        std::vector<std::string> _requests;
    };
}
