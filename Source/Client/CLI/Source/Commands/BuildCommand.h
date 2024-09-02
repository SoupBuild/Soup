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
				workingDirectory = Path(std::format("{}/", _options.Path));

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

			// Platform specific defaults
			#if defined(_WIN32)
			arguments.HostPlatform = "Windows";
			#elif defined(__linux__)
			arguments.HostPlatform = "Linux";
			#else
			#error "Unknown Platform"
			#endif

			// Process well known parameters
			if (!_options.Flavor.empty())
				arguments.GlobalParameters.emplace("Flavor", Core::Value(_options.Flavor));
			if (!_options.Architecture.empty())
				arguments.GlobalParameters.emplace("Architecture", Core::Value(_options.Architecture));

			// TODO: Generic parameters

			// Now build the current project
			Log::Info("Begin Build:");

			// Find the built in folder root
			auto processFilename = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto processDirectory = processFilename.GetParent();
			auto builtInPackageDirectory = processDirectory + Path("BuiltIn/");

			// Load user config state
			auto userDataPath = Core::BuildEngine::GetSoupUserDataPath();
			
			auto recipeCache = Core::RecipeCache();

			auto packageProvider = Core::BuildEngine::LoadBuildGraph(
				builtInPackageDirectory,
				arguments.WorkingDirectory,
				arguments.GlobalParameters,
				userDataPath,
				recipeCache);

			Core::BuildEngine::Execute(
				packageProvider,
				std::move(arguments),
				userDataPath,
				recipeCache);

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
