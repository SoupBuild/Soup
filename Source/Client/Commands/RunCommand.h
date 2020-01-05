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

			std::shared_ptr<ICompiler> runtimeCompiler = nullptr;
			if (config.GetRuntimeCompiler() == "clang")
			{
				runtimeCompiler = std::make_shared<Compiler::Clang::Compiler>(
					Path(config.GetClangToolPath()));
			}
			else if (config.GetRuntimeCompiler() == "msvc")
			{
				runtimeCompiler = std::make_shared<Compiler::MSVC::Compiler>(
					Path(config.GetMSVCRootPath()) + Path("bin/Hostx64/x64/"),
					Path("cl.exe"),
					Path("link.exe"),
					Path("lib.exe"));
			}
			else
			{
				throw std::runtime_error("Unknown compiler.");
			}

			auto workingDirectory = System::IFileSystem::Current().GetCurrentDirectory2();
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
			auto binaryDirectory = RecipeExtensions::GetBinaryDirectory(std::string(runtimeCompiler->GetName()), configuration);
			auto executablePath = workingDirectory + binaryDirectory + Path(recipe.GetName() + ".exe");
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
