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

			// Load the user config
			auto config = LocalUserConfigExtensions::LoadFromFile();

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

			auto recipePath = 
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadFromFile(recipePath, recipe))
			{
				Log::Error("Could not load the recipe file.");
				return;
			}

			// Setup the build arguments
			auto arguments = RecipeBuildArguments();
			arguments.ForceRebuild = _options.Force;
			arguments.SkipRun = _options.SkipRun;

			if (!_options.Flavor.empty())
				arguments.Flavor = _options.Flavor;
			else
				arguments.Flavor = "debug";

			if (!_options.System.empty())
				arguments.System = _options.System;
			else
				arguments.System = "win32";

			if (!_options.Architecture.empty())
				arguments.Architecture = _options.Architecture;
			else
				arguments.Architecture = "x64";

			std::string runtimeCompiler = config.GetRuntimeCompiler();
			std::string systemCompiler = runtimeCompiler;

			// Now build the current project
			Log::Info("Begin Build:");
			auto startTime = std::chrono::high_resolution_clock::now();

			auto buildManager = Build::Runtime::RecipeBuildManager(systemCompiler, runtimeCompiler);
			buildManager.Execute(workingDirectory, recipe, arguments);

			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime -startTime);

			Log::HighPriority(std::to_string(duration.count()) + " seconds.");
		}

	private:
		BuildOptions _options;
	};
}
