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
			Log::Trace("RunCommand::Run");

			auto runtimeCompiler = std::make_shared<Compiler::Clang::Compiler>();

			auto workingDirectory = Path::GetCurrentDirectory();
			auto recipePath = 
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadFromFile(recipePath, recipe))
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
			auto binaryDirectory = RecipeExtensions::GetBinaryDirectory(*runtimeCompiler, configuration);
			auto executablePath = workingDirectory + binaryDirectory + Path(recipe.GetName() + ".exe");
			Log::Verbose(executablePath.ToString());
			if (!IFileSystem::Current().Exists(executablePath))
			{
				Log::Error("The executable does not exist");
				return;
			}

			// Execute the requested target
			auto result = IProcessManager::Current().Execute(
				executablePath,
				_options.Arguments,
				workingDirectory);

			// TODO: Directly pipe to output and make sure there is no extra newline
			if (!result.StdOut.empty())
			{
				Log::Info(result.StdOut);
			}

			if (!result.StdErr.empty())
			{
				Log::Error(result.StdErr);
			}

			if (result.ExitCode != 0)
			{
				// TODO: Return error code
				Log::Verbose("FAILED");
			}

			Log::Verbose("Done");
		}

	private:
		RunOptions _options;
	};
}
