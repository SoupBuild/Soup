// <copyright file="BuildCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "BuildOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Build Command
	/// </summary>
	class BuildCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildCommand"/> class.
		/// </summary>
		BuildCommand(BuildOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("BuildCommand::Run");
			auto startTime = std::chrono::high_resolution_clock::now();

			auto workingDirectory = Path();
			if (_options.Path.empty())
			{
				// Build in the current directory
				workingDirectory = System::IFileSystem::Current().GetCurrentDirectory();
			}
			else
			{
				workingDirectory = Path(_options.Path + "/");

				// Check if this is relative to current directory
				if (!workingDirectory.HasRoot())
				{
					workingDirectory = System::IFileSystem::Current().GetCurrentDirectory() + workingDirectory;
				}
			}

			// Setup the build arguments
			auto arguments = Core::RecipeBuildArguments();
			arguments.WorkingDirectory = std::move(workingDirectory);
			arguments.ForceRebuild = _options.Force;
			arguments.SkipGenerate = _options.SkipGenerate;
			arguments.SkipEvaluate = _options.SkipEvaluate;

			auto flavor = std::string("debug");
			auto architecture = std::string("x64");

			#if defined(_WIN32)
				auto system = std::string("win32");
				auto compiler = std::string("MSVC");
			#elif defined(__linux__)
				auto system = std::string("win32");
				auto compiler = std::string("GCC");
			#else
			#error "Unknown Platform"
			#endif

			if (!_options.Flavor.empty())
				flavor = _options.Flavor;
			if (!_options.Architecture.empty())
				architecture = _options.Architecture;
			if (!_options.System.empty())
				system = _options.System;

			arguments.GlobalParameters.emplace("Architecture", Core::Value(std::string(architecture)));
			arguments.GlobalParameters.emplace("Compiler", Core::Value(std::string(compiler)));
			arguments.GlobalParameters.emplace("Flavor", Core::Value(std::string(flavor)));
			arguments.GlobalParameters.emplace("System", Core::Value(std::string(system)));

			// Now build the current project
			Log::Info("Begin Build:");

			Core::BuildEngine::Execute(std::move(arguments));

			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime -startTime);

			std::ostringstream durationMessage;
			if (duration.count() >= 60)
			{
				durationMessage << std::fixed << std::setprecision(2);
				durationMessage << duration.count() / 60  << " minutes";
			}
			else if (duration.count() >= 10)
			{
				durationMessage << std::fixed << std::setprecision(0);
				durationMessage << duration.count() << " seconds";
			}
			else
			{
				durationMessage << std::fixed << std::setprecision(3);
				durationMessage << duration.count() << " seconds";
			}

			Log::HighPriority(durationMessage.str());
		}

	private:
		BuildOptions _options;
	};
}
