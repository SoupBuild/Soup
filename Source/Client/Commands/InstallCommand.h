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
			
			if (_options.Package.empty())
			{
				PackageManager::InstallPackages();
			}
			else
			{
				PackageManager::InstallPackageReference(_options.Package);
			}
		}

	private:
		InstallOptions _options;
	};
}
