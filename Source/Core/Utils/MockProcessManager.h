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
        virtual ProcessResult Execute(
            const Path& application,
            const std::vector<std::string>& arguments,
            const Path& workingDirectory) override final
        {
            std::stringstream message;
            message << workingDirectory.ToString() << ": " << application.ToString();
            for (auto& value : arguments)
                message << " " << value;

            _requests.push_back(message.str());
            return {
                0,
                std::string(),
                std::string(),
            };
        }

    private:
        std::vector<std::string> _requests;
    };
}
