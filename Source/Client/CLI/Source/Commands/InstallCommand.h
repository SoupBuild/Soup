// <copyright file="InstallCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "InstallOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Install Command
	/// </summary>
	class InstallCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="InstallCommand"/> class.
		/// </summary>
		InstallCommand(InstallOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("InstallCommand::Run");

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

			if (_options.PackageReference.empty())
			{
				PackageManager::InstallPackages(
					workingDirectory);
			}
			else
			{
				PackageManager::InstallPackageReference(
					workingDirectory,
					_options.PackageReference);
			}
		}

	private:
		InstallOptions _options;
	};
}
