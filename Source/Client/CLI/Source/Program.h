// <copyright file="Program.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentsParser.h"
#include "BuildCommand.h"
#include "InitializeCommand.h"
#include "InstallCommand.h"
#include "PublishCommand.h"
#include "RestoreCommand.h"
#include "RunCommand.h"
#include "TargetCommand.h"
#include "VersionCommand.h"

namespace Soup::Client
{
	/// <summary>
	/// The root of all evil
	/// </summary>
	class Program
	{
	public:
		Program() :
			_filter(nullptr)
		{
		}

		/// <summary>
		/// The main entry point of the program
		/// </summary>
		int Run(std::vector<std::string> args)
		{
			try
			{
				// Setup the filter
				auto defaultTypes =
					static_cast<uint32_t>(TraceEventFlag::Warning) |
					static_cast<uint32_t>(TraceEventFlag::Error) |
					static_cast<uint32_t>(TraceEventFlag::Critical);
				_filter = std::make_shared<EventTypeFilter>(
						static_cast<TraceEventFlag>(defaultTypes));

				// Setup the console listener
				Log::RegisterListener(
					std::make_shared<ConsoleTraceListener>(
						"Log",
						_filter,
						false,
						false));

				// Setup the real services
				System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());
				System::IProcessManager::Register(std::make_shared<System::WindowsProcessManager>());
				Monitor::IDetourProcessManager::Register(std::make_shared<Monitor::WindowsDetourProcessManager>());
				IO::IConsoleManager::Register(std::make_shared<IO::SystemConsoleManager>());

				// Attempt to parse the provided arguments
				Log::Diag("ProgramStart");
				ArgumentsParser arguments;

				try
				{
					arguments = ArgumentsParser::Parse(args);
				}
				catch(const std::runtime_error& ex)
				{
					// Failed to parse the arguments
					Log::Error(ex.what());
					WriteUsage();
					return -1;
				}

				// Map the individual commands
				// TODO: Clang is having troubles with functional
				// Use functions to map the commands
				std::shared_ptr<ICommand> command;
				if (arguments.IsA<BuildOptions>())
					command = Setup(arguments.ExtractResult<BuildOptions>());
				else if (arguments.IsA<RunOptions>())
					command = Setup(arguments.ExtractResult<RunOptions>());
				else if (arguments.IsA<InitializeOptions>())
					command = Setup(arguments.ExtractResult<InitializeOptions>());
				else if (arguments.IsA<InstallOptions>())
					command = Setup(arguments.ExtractResult<InstallOptions>());
				else if (arguments.IsA<PublishOptions>())
					command = Setup(arguments.ExtractResult<PublishOptions>());
				else if (arguments.IsA<RestoreOptions>())
					command = Setup(arguments.ExtractResult<RestoreOptions>());
				else if (arguments.IsA<TargetOptions>())
					command = Setup(arguments.ExtractResult<TargetOptions>());
				else if (arguments.IsA<VersionOptions>())
					command = Setup(arguments.ExtractResult<VersionOptions>());
				else
					throw std::runtime_error("Unknown arguments");

				// Run the requested command
				Log::Diag("Run Command");
				command->Run();

				return 0;
			}
			catch (const Core::HandledException& ex)
			{
				Log::Diag("Exception Handled: Exiting");
				return ex.GetExitCode();
			}
			catch (const std::exception& ex)
			{
				Log::Error("Exception Caught: Exiting");
				Log::Error(ex.what());
				return -2;
			}
			catch (...)
			{
				Log::Error("Unknown exception encountered");
				return -3;
			}
		}

	private:
		static void WriteUsage()
		{
			Log::Info("Expected commands:");
			Log::Info("	build - Build the provided recipe.");
			Log::Info("	run - Run the provided recipe.");
			Log::Info("	initialize - Initialize wizard for creating a new recipe.");
			Log::Info("	install - Install a dependency to the target recipes.");
			Log::Info("	publish - Publish the contents of a recipe to the target feed.");
			Log::Info("	restore - Install al dependencies in the target recipes.");
			Log::Info("	version - Display the version of the command line application.");
		}

		void SetupShared(SharedOptions& options)
		{
			Log::Diag("Setup SharedOptions");
			_filter->Set(options.Verbosity);
		}

		std::shared_ptr<ICommand> Setup(BuildOptions options)
		{
			Log::Diag("Setup BuildOptions");
			SetupShared(options);
			return std::make_shared<BuildCommand>(
				std::move(options));
		}

		std::shared_ptr<ICommand> Setup(RunOptions options)
		{
			Log::Diag("Setup RunCommand");
			SetupShared(options);
			return std::make_shared<RunCommand>(
				std::move(options));
		}

		std::shared_ptr<ICommand> Setup(InitializeOptions options)
		{
			Log::Diag("Setup InitializeCommand");
			SetupShared(options);
			return std::make_shared<InitializeCommand>(
				std::move(options));
		}

		std::shared_ptr<ICommand> Setup(InstallOptions options)
		{
			Log::Diag("Setup InstallCommand");
			SetupShared(options);
			return std::make_shared<InstallCommand>(
				std::move(options));
		}

		std::shared_ptr<ICommand> Setup(PublishOptions options)
		{
			Log::Diag("Setup PublishCommand");
			SetupShared(options);
			return std::make_shared<PublishCommand>(
				std::move(options));
		}

		std::shared_ptr<ICommand> Setup(RestoreOptions options)
		{
			Log::Diag("Setup RestoreCommand");
			SetupShared(options);
			return std::make_shared<RestoreCommand>(
				std::move(options));
		}

		std::shared_ptr<ICommand> Setup(TargetOptions options)
		{
			Log::Diag("Setup TargetOptions");
			SetupShared(options);
			return std::make_shared<TargetCommand>(
				std::move(options));
		}

		std::shared_ptr<ICommand> Setup(VersionOptions options)
		{
			Log::Diag("Setup VersionCommand");
			SetupShared(options);
			return std::make_shared<VersionCommand>(
				std::move(options));
		}

	private:
		std::shared_ptr<EventTypeFilter> _filter;
	};
}
