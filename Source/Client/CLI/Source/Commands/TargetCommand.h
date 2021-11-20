// <copyright file="TargetCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "TargetOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Target Command
	/// </summary>
	class TargetCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="TargetCommand"/> class.
		/// </summary>
		TargetCommand(TargetOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("TargetCommand::Run");

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

			// Load the recipe
			auto buildManager = Build::RecipeBuildManager();
			auto recipePath =
				workingDirectory +
				Build::Runtime::BuildConstants::RecipeFileName();
			Build::Runtime::Recipe recipe = {};
			if (!buildManager.TryGetRecipe(recipePath, recipe))
			{
				Log::Error("The Recipe does not exist: " + recipePath.ToString());
				Log::HighPriority("Make sure the path is correct and try again");

				// Nothing we can do, exit
				throw HandledException(1234);
			}

			// Setup the build parameters
			auto flavor = std::string("debug");
			if (!_options.Flavor.empty())
				flavor = _options.Flavor;

			auto system = std::string("win32");
			if (!_options.System.empty())
				system = _options.System;

			auto architecture = std::string("x64");
			if (!_options.Architecture.empty())
				architecture = _options.Architecture;

			auto compiler = std::string("MSVC");

			auto globalParameters = Build::Runtime::ValueTable();
			globalParameters.SetValue("Architecture", Build::Runtime::Value(std::string(architecture)));
			globalParameters.SetValue("Compiler", Build::Runtime::Value(std::string(compiler)));
			globalParameters.SetValue("Flavor", Build::Runtime::Value(std::string(flavor)));
			globalParameters.SetValue("System", Build::Runtime::Value(std::string(system)));

			// Load the value table to get the exe path
			auto isHostBuild = false;
			auto targetDirectory = Build::RecipeBuildRunner::GetOutputDirectory(
				workingDirectory,
				recipe,
				globalParameters,
				isHostBuild,
				buildManager);
			
			std::cout << targetDirectory.ToString() << std::flush;
		}

	private:
		TargetOptions _options;
	};
}
