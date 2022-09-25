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
			auto packageProvider = Core::PackageProvider();
			auto recipePath =
				workingDirectory +
				Core::BuildConstants::RecipeFileName();
			Core::Recipe recipe = {};
			if (!packageProvider.TryGetRecipe(recipePath, recipe))
			{
				Log::Error("The Recipe does not exist: " + recipePath.ToString());
				Log::HighPriority("Make sure the path is correct and try again");

				// Nothing we can do, exit
				throw Core::HandledException(1234);
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

			auto globalParameters = Core::ValueTable();
			globalParameters.SetValue("Architecture", Core::Value(std::string(architecture)));
			globalParameters.SetValue("Compiler", Core::Value(std::string(compiler)));
			globalParameters.SetValue("Flavor", Core::Value(std::string(flavor)));
			globalParameters.SetValue("System", Core::Value(std::string(system)));

			// Load the value table to get the exe path
			auto targetDirectory = Core::RecipeBuildLocationManager::GetOutputDirectory(
				workingDirectory,
				recipe,
				globalParameters,
				packageProvider);
			auto soupTargetDirectory = targetDirectory + Core::BuildConstants::GetSoupTargetDirectory();
			auto sharedStateFile = soupTargetDirectory + Core::BuildConstants::GenerateSharedStateFileName();

			// Load the shared state file
			auto sharedStateTable = Core::ValueTable();
			if (!Core::ValueTableManager::TryLoadState(sharedStateFile, sharedStateTable))
			{
				Log::Error("Failed to load the shared state file: " + sharedStateFile.ToString());
				return;
			}

			// Get the executable from the build target file property
			// Check for any dynamic libraries in the shared state
			if (!sharedStateTable.HasValue("Build"))
			{
				Log::Error("Shared state does not have a build table");
				return;
			}

			auto& buildTable = sharedStateTable.GetValue("Build").AsTable();
			if (!buildTable.HasValue("RunExecutable"))
			{
				Log::Error("Build table does not have a RunExecutable property");
				return;
			}

			if (!buildTable.HasValue("RunArguments"))
			{
				Log::Error("Build table does not have a RunArguments property");
				return;
			}

			auto runExecutable = Path(buildTable.GetValue("RunExecutable").AsString().ToString());
			Log::Info("Executable: " + runExecutable.ToString());
			if (!System::IFileSystem::Current().Exists(runExecutable))
			{
				Log::Error("The run executable does not exist");
				return;
			}

			auto runArguments = buildTable.GetValue("RunArguments").AsString().ToString();
			auto arguments = std::stringstream();
			arguments << runArguments << " ";
			for (auto& argument : _options.Arguments)
			{
				arguments << argument << " ";
			}

			// Execute the requested target
			Log::Info("Arguments: " + arguments.str());
			auto process = System::IProcessManager::Current().CreateProcess(
				runExecutable,
				arguments.str(),
				workingDirectory);
			process->Start();
			process->WaitForExit();

			auto stdOut = process->GetStandardOutput();
			auto stdErr = process->GetStandardError();
			auto exitCode = process->GetExitCode();

			// TODO: Directly pipe to output and make sure there is no extra newline
			if (!stdOut.empty())
			{
				Log::HighPriority(stdOut);
			}

			if (!stdErr.empty())
			{
				Log::Error(stdErr);
			}

			if (exitCode != 0)
				throw Core::HandledException(exitCode);
		}

	private:
		RunOptions _options;
	};
}
