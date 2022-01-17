// <copyright file="RestoreCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "RestoreOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Restore Command
	/// </summary>
	class RestoreCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RestoreCommand"/> class.
		/// </summary>
		RestoreCommand(RestoreOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("RestoreCommand::Run");

			auto workingDirectory = Path();
			if (_options.Path.empty())
			{
				// Build in the current directory
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

			Core::PackageManager::RestorePackages(
				workingDirectory);
		}

	private:
		RestoreOptions _options;
	};
}
