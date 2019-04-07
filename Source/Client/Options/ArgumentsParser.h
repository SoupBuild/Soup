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

            auto& commandType = args[1];
            std::any result = nullptr;
            if (commandType == "build")
            {
                Log::Trace("Parse build");
                result = BuildOptions();
            }
            else if (commandType == "init")
            {
                Log::Trace("Parse initialize");
                result = InitializeOptions();
            }
            else if (commandType == "install")
            {
                Log::Trace("Parse install");
                result = InstallOptions();
            }
            else if (commandType == "pack")
            {
                Log::Trace("Parse pack");
                result = PackOptions();
            }
            else if (commandType == "publish")
            {
                Log::Trace("Parse publish");
                result = PublishOptions();
            }
            else if (commandType == "run")
            {
                Log::Trace("Parse run");
                result = RunOptions();
            }
            else if (commandType == "version")
            {
                Log::Trace("Parse version");
                result = VersionOptions();
            }
            else if (commandType == "view")
            {
                Log::Trace("Parse view");
                result = ViewOptions();
            }
            else
            {
                throw std::runtime_error("Unknown command argument.");
            }

            return ArgumentsParser(
                std::move(result));
        }

    private:
        ArgumentsParser(std::any result) :
            m_result(std::move(result))
        {
        }

    public:
        template<typename T>
        bool IsA()
        {
            return m_result.type() == typeid(T);
        }

        template<typename T>
        auto ExtractsResult() -> T
        {
            return std::any_cast<T>(m_result);
        }

    private:
        std::any m_result;
    };
}