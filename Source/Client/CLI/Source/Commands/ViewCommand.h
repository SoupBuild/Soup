// <copyright file="ViewCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "ViewOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// View Command
	/// </summary>
	class ViewCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ViewCommand"/> class.
		/// </summary>
		ViewCommand(ViewOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("ViewsCommand::Run");

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

			auto recipePath =
				workingDirectory +
				Core::BuildConstants::RecipeFileName();

			auto arguments = std::vector<std::string>({
				recipePath.ToString(),
			});

			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();
			auto soupViewFolder = moduleFolder + Path("View/");
			#if defined(_WIN32)
			auto executable = soupViewFolder + Path("SoupView.exe");
			#elif defined(__linux__)
			auto executable = soupViewFolder + Path("SoupView");
			#else
			#error "Unknown platform"
			#endif

			// Execute the requested target
			Log::Info("CreateProcess");
			Log::Diag(executable.ToString());
			auto process = System::IProcessManager::Current().CreateProcess(
				executable,
				std::move(arguments),
				workingDirectory,
				false);
			process->Start();
			process->WaitForExit();

			auto exitCode = process->GetExitCode();

			if (exitCode != 0)
				throw Core::HandledException(exitCode);
		}

	private:
		ViewOptions _options;
	};
}
