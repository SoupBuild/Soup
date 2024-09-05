// <copyright file="InitializeCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "InitializeOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Initialize Command
	/// </summary>
	class InitializeCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="InitializeCommand"/> class.
		/// </summary>
		InitializeCommand(InitializeOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("InitializeCommand::Run");

			auto workingDirectory = Path();
			if (_options.Path.empty())
			{
				// Build in the current directory
				workingDirectory = System::IFileSystem::Current().GetCurrentDirectory();
			}
			else
			{
				// Parse the path in any system valid format
				workingDirectory = Path::Parse(std::format("{}/", _options.Path));

				// Check if this is relative to current directory
				if (!workingDirectory.HasRoot())
				{
					workingDirectory = System::IFileSystem::Current().GetCurrentDirectory() + workingDirectory;
				}
			}

			Core::PackageManager::InitializePackage(workingDirectory);
		}

	private:
		InitializeOptions _options;
	};
}
