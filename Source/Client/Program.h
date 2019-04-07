// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentsParser.h"
#include "BuildCommand.h"
#include "InitializeCommand.h"
#include "InstallCommand.h"
#include "PackCommand.h"
#include "PublishCommand.h"
#include "RunCommand.h"
#include "VersionCommand.h"
#include "ViewCommand.h"

namespace Soup::Client
{
    /// <summary>
    /// The root of all evil
    /// </summary>
    class Program
    {
    public:
        /// <summary>
        /// The main entry point of the program
        /// </summary>
        static int Main(std::vector<std::string> args)
        {
            try
            {
                // Enable full diagnostics
                // Log::SetDiagnosticEnabled(true);

                Log::Trace("ProgramStart");
                auto arguments = ArgumentsParser::Parse(args);

                // Map the individual commands
                // TODO: Clang is having troubles with functional
                // Use functions to map the commands
                std::shared_ptr<ICommand> command;
                if (arguments.IsA<BuildOptions>())
                    command = Setup(arguments.ExtractsResult<BuildOptions>());
                else if (arguments.IsA<RunOptions>())
                    command = Setup(arguments.ExtractsResult<RunOptions>());
                else if (arguments.IsA<InitializeOptions>())
                    command = Setup(arguments.ExtractsResult<InitializeOptions>());
                else if (arguments.IsA<InstallOptions>())
                    command = Setup(arguments.ExtractsResult<InstallOptions>());
                else if (arguments.IsA<PackOptions>())
                    command = Setup(arguments.ExtractsResult<PackOptions>());
                else if (arguments.IsA<PublishOptions>())
                    command = Setup(arguments.ExtractsResult<PublishOptions>());
                else if (arguments.IsA<VersionOptions>())
                    command = Setup(arguments.ExtractsResult<VersionOptions>());
                else if (arguments.IsA<ViewOptions>())
                    command = Setup(arguments.ExtractsResult<ViewOptions>());
                else
                    throw std::runtime_error("Unknown arguments.");

                // Run the requested command
                Log::Trace("Run Command");
                command->Run();

                return 0;
            }
            catch (const std::exception& ex)
            {
                Log::Error(ex.what());
                return -1;
            }
        }

    private:
        static void SetupShared(SharedOptions& options)
        {
            Log::Trace("Setup SharedOptions");
            Log::SetVerboseEnabled(options.EnableVerbose);
        }

        static std::shared_ptr<ICommand> Setup(BuildOptions options)
        {
            Log::Trace("Setup BuildOptions");
            SetupShared(options);
            return std::make_shared<BuildCommand>(
                std::move(options));
        }

        static std::shared_ptr<ICommand> Setup(RunOptions options)
        {
            Log::Trace("Setup RunCommand");
            SetupShared(options);
            return std::make_shared<RunCommand>(
                std::move(options));
        }

        static std::shared_ptr<ICommand> Setup(InitializeOptions options)
        {
            Log::Trace("Setup InitializeCommand");
            SetupShared(options);
            return std::make_shared<InitializeCommand>(
                std::move(options));
        }

        static std::shared_ptr<ICommand> Setup(InstallOptions options)
        {
            Log::Trace("Setup InstallCommand");
            SetupShared(options);
            return std::make_shared<InstallCommand>(
                std::move(options));
        }

        static std::shared_ptr<ICommand> Setup(PackOptions options)
        {
            Log::Trace("Setup PackCommand");
            SetupShared(options);
            return std::make_shared<PackCommand>(
                std::move(options));
        }

        static std::shared_ptr<ICommand> Setup(PublishOptions options)
        {
            Log::Trace("Setup PublishCommand");
            SetupShared(options);
            return std::make_shared<PublishCommand>(
                std::move(options));
        }

        static std::shared_ptr<ICommand> Setup(VersionOptions options)
        {
            Log::Trace("Setup VersionCommand");
            SetupShared(options);
            return std::make_shared<VersionCommand>(
                std::move(options));
        }

        static std::shared_ptr<ICommand> Setup(ViewOptions options)
        {
            Log::Trace("Setup ViewCommand");
            SetupShared(options);
            return std::make_shared<ViewCommand>(
                std::move(options));
        }

        // /// <summary>
        // /// Load the local user config
        // /// </summary>
        // static LocalUserConfig GetUserConfig()
        // {
        //     return new LocalUserConfig();
        // }

        // /// <summary>
        // /// Create the soup api helper
        // /// </summary>
        // static ISoupIdentity GetSoupIdentity()
        // {
        //     return new SoupIdentity();
        // }

        // /// <summary>
        // /// Create the soup api helper
        // /// </summary>
        // static ISoupApi GetSoupApi()
        // {
        //     return new SoupApi();
        // }

        // /// <summary>
        // /// Setup the required Compiler
        // /// </summary>
        // static ICompiler GetCompiler()
        // {
        //     if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        //     {
        //         // return new Compiler.MSVC.Compiler();
        //         return new Compiler.Clang.Compiler();
        //     }
        //     else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        //     {
        //         return new Compiler.Clang.Compiler();
        //     }
        //     else
        //     {
        //         throw new NotSupportedException("Unknown platform.");
        //     }
        // }
    };
}
