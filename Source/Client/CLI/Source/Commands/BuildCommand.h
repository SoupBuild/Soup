﻿// <copyright file="BuildCommand.h" company="Soup">
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

			auto workingDirectory = Path();
			if (_options.Path.empty())
			{
				// Buildin the current directory
				workingDirectory = System::IFileSystem::Current().GetCurrentDirectory2();
			}
			else
			{
				workingDirectory = Path(_options.Path);

				// Check if this is relative to current directory
				if (!workingDirectory.HasRoot())
				{
					workingDirectory = System::IFileSystem::Current().GetCurrentDirectory2() + workingDirectory;
				}
			}

			// Setup the build arguments
			auto arguments = RecipeBuildArguments();
			arguments.ForceRebuild = _options.Force;
			arguments.SkipGenerate = _options.SkipGenerate;
			arguments.SkipEvaluate = _options.SkipEvaluate;

			auto flavor = std::string("debug");
			if (!_options.Flavor.empty())
				flavor = _options.Flavor;

			auto system = std::string("win32");
			if (!_options.System.empty())
				system = _options.System;

			auto architecture = std::string("x64");
			if (!_options.Architecture.empty())
				architecture = _options.Architecture;

			auto compiler = std::string("MSVC");

			arguments.GlobalParameters.SetValue("Architecture", Build::Runtime::Value(std::string(architecture)));
			arguments.GlobalParameters.SetValue("Compiler", Build::Runtime::Value(std::string(compiler)));
			arguments.GlobalParameters.SetValue("Flavor", Build::Runtime::Value(std::string(flavor)));
			arguments.GlobalParameters.SetValue("System", Build::Runtime::Value(std::string(system)));

			// Now build the current project
			Log::Info("Begin Build:");
			auto startTime = std::chrono::high_resolution_clock::now();

			auto buildManager = Build::RecipeBuildRunner(std::move(arguments));
			buildManager.Execute(workingDirectory);

			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime -startTime);

			Log::HighPriority(std::to_string(duration.count()) + " seconds.");
		}

	private:
		BuildOptions _options;
	};
}
