// <copyright file="RunCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "RunOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Run Command
	/// </summary>
	class RunCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RunCommand"/> class.
		/// </summary>
		RunCommand(RunOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("RunCommand::Run");

			// Load the user config
			auto config =  LocalUserConfigExtensions::LoadFromFile();

			auto workingDirectory = System::IFileSystem::Current().GetCurrentDirectory2();
			auto recipePath = 
				workingDirectory +
				Path(Constants::RecipeFileName);
			Build::Recipe recipe = {};
			if (!Build::RecipeExtensions::TryLoadFromFile(recipePath, recipe))
			{
				Log::Error("Could not load the recipe file");
				return;
			}

			// Ensure that this is an executable
			if (recipe.GetType() != RecipeType::Executable)
			{
				Log::Error("Cannot run a project not of type executable");
				return;
			}

			// Ensure the executable exists
			auto configuration = "release";
			auto compilerName = config.GetRuntimeCompiler();
			auto binaryDirectory = Build::RecipeExtensions::GetBinaryDirectory(compilerName, configuration);
			auto executablePath = workingDirectory + binaryDirectory + Path(std::string(recipe.GetName()) + ".exe");
			Log::Info(executablePath.ToString());
			if (!System::IFileSystem::Current().Exists(executablePath))
			{
				Log::Error("The executable does not exist");
				return;
			}

			auto arguments = std::stringstream();
			for (auto& argument : _options.Arguments)
			{
				arguments << argument << " ";
			}

			// Execute the requested target
			auto result = System::IProcessManager::Current().Execute(
				executablePath,
				arguments.str(),
				workingDirectory);

			// TODO: Directly pipe to output and make sure there is no extra newline
			if (!result.StdOut.empty())
			{
				Log::HighPriority(result.StdOut);
			}

			if (!result.StdErr.empty())
			{
				Log::Error(result.StdErr);
			}

			if (result.ExitCode != 0)
			{
				// TODO: Return error code
				Log::HighPriority("FAILED");
			}

			Log::HighPriority("Done");
		}

	private:
		RunOptions _options;
	};
}
