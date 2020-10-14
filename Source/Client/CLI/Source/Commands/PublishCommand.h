// <copyright file="PublishCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "PublishOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Publish Command
	/// </summary>
	class PublishCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PublishCommand"/> class.
		/// </summary>
		PublishCommand(PublishOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("PublishCommand::Run");

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

			PackageManager::PublishPackage(workingDirectory);
		}

	private:
		PublishOptions _options;
	};
}
