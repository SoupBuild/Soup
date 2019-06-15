// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildOptions.h"
#include "InitializeOptions.h"
#include "InstallOptions.h"
#include "PackOptions.h"
#include "PublishOptions.h"
#include "RunOptions.h"
#include "VersionOptions.h"
#include "ViewOptions.h"

namespace Soup::Client
{
    /// <summary>
    /// Arguments parser custom made for Soup,
    /// TODO: Create awesome generated version
    /// </summary>
    class ArgumentsParser
    {
    public:
        static ArgumentsParser Parse(const std::vector<std::string>& args)
        {
            Log::Trace("ArgumentsParser::Parse");
            if (args.size() <= 1)
            {
                throw std::runtime_error("No arguments provided.");
            }

            // The first argument must be the requested command
            auto& commandType = args[1];

            // Copy the set of unused args to ensure we consume everythings
            std::vector<std::string> unusedArgs;
            std::copy(args.begin()+2, args.end(), std::back_inserter(unusedArgs));

            // Handle the individual commands and their expected arguments
            std::any result = nullptr;
            if (commandType == "build")
            {
                Log::Trace("Parse build");

                auto options = BuildOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else if (commandType == "init")
            {
                Log::Trace("Parse initialize");

                auto options = InitializeOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else if (commandType == "install")
            {
                Log::Trace("Parse install");

                auto options = InstallOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else if (commandType == "pack")
            {
                Log::Trace("Parse pack");

                auto options = PackOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else if (commandType == "publish")
            {
                Log::Trace("Parse publish");

                auto options = PublishOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else if (commandType == "run")
            {
                Log::Trace("Parse run");

                auto options = RunOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else if (commandType == "version")
            {
                Log::Trace("Parse version");

                auto options = VersionOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else if (commandType == "view")
            {
                Log::Trace("Parse view");

                auto options = ViewOptions();
                options.EnableVerbose = IsFlagSet("v", unusedArgs);

                result = std::move(options);
            }
            else
            {
                throw std::runtime_error("Unknown command argument: " + commandType);
            }

            if (!unusedArgs.empty())
            {
                std::stringstream message;
                message << "Unrecognized argument(s): ";
                for (int i = 0; i < unusedArgs.size(); i++)
                {
                    if (i > 0)
                    {
                        message << ", ";
                    }

                    message << unusedArgs[i];
                }

                throw std::runtime_error(message.str());
            }

            return ArgumentsParser(
                std::move(result));
        }

    private:
        ArgumentsParser(std::any result) :
            _result(std::move(result))
        {
        }

    public:
        ArgumentsParser() :
            _result(nullptr)
        {
        }

        template<typename T>
        bool IsA()
        {
            return _result.type() == typeid(T);
        }

        template<typename T>
        auto ExtractsResult() -> T
        {
            return std::any_cast<T>( _result);
        }

    private:
        static bool IsFlagSet(const char* value, std::vector<std::string>& unusedArgs)
        {
            auto flagValue = std::string("-") + value;
            Log::Trace("IsFlagSet: " + flagValue);
            auto flagLocation = std::find(unusedArgs.begin(), unusedArgs.end(), flagValue);
            if (flagLocation != unusedArgs.end())
            {
                // Consume the flag value
                unusedArgs.erase(flagLocation);
                return true;
            }
            else
            {
                return false;
            }
        }

    private:
        std::any _result;
    };
}