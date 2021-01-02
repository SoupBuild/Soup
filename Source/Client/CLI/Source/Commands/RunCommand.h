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
			auto config = LocalUserConfigExtensions::LoadFromFile();

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

			auto recipePath = 
				workingDirectory +
				Build::Runtime::BuildConstants::RecipeFileName();
			Build::Runtime::Recipe recipe = {};
			if (!Build::Runtime::RecipeExtensions::TryLoadRecipeFromFile(recipePath, recipe))
			{
				Log::Error("Could not load the recipe file");
				return;
			}

			// Ensure that this is an executable
			// TODO: 
			// if (recipe.GetType() != "Executable")
			// {
			// 	Log::Error("Cannot run a project not of type executable");
			// 	return;
			// }

			// Ensure the executable exists
			auto flavor = "debug";
			auto system = "win32";
			auto architecture = "x64";
			auto compilerName = config.GetRuntimeCompiler();

			// Set the default output directory to be relative to the package
			auto rootOutput = recipePath + Path("out/");

			// Check for root recipe file with overrides
			Path rootRecipeFile;
			if (Build::RootRecipeExtensions::TryFindRootRecipeFile(recipePath, rootRecipeFile))
			{
				Log::Info("Found Root Recipe: '" + rootRecipeFile.ToString() + "'");
				Build::RootRecipe rootRecipe;
				if (!Build::RootRecipeExtensions::TryLoadRootRecipeFromFile(rootRecipeFile, rootRecipe))
				{
					// Nothing we can do, exit
					Log::Error("Failed to load the root recipe file: " + rootRecipeFile.ToString());
					throw HandledException(222);
				}

				// Today the only unique thing it can do is set the shared output directory
				if (rootRecipe.HasOutputRoot())
				{
					// Relative to the root recipe file itself
					rootOutput = rootRecipe.GetOutputRoot() + Path(recipe.GetName() + "/");
					if (!rootOutput.HasRoot())
					{
						rootOutput = rootRecipeFile.GetParent() + rootOutput;
					}

					Log::Info("Override root output: " + rootOutput.ToString());
				}
			}

			// Load the value table to get the exe path
			auto targetDirectory = rootOutput + Build::RecipeBuildRunner::GetConfigurationDirectory(
				compilerName,
				flavor,
				system,
				architecture);
			auto soupTargetDirectory = targetDirectory + Build::RecipeBuildRunner::GetSoupTargetDirectory();
			auto sharedStateFile = soupTargetDirectory + Build::Runtime::BuildConstants::GenerateSharedStateFileName();

			// Load the shared state file
			auto sharedStateTable = Build::Runtime::ValueTable();
			if (!Build::Runtime::ValueTableManager::TryLoadState(sharedStateFile, sharedStateTable))
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
			if (!buildTable.HasValue("TargetFile"))
			{
				Log::Error("Build table does not have a TargetFile property");
				return;
			}

			auto targetFile = Path(buildTable.GetValue("TargetFile").AsString().ToString());
			Log::Info(targetFile.ToString());
			if (!System::IFileSystem::Current().Exists(targetFile))
			{
				Log::Error("The target does not exist");
				return;
			}

			auto arguments = std::stringstream();
			for (auto& argument : _options.Arguments)
			{
				arguments << argument << " ";
			}

			// Execute the requested target
			auto process = System::IProcessManager::Current().CreateProcess(
				targetFile,
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
				throw HandledException(exitCode);
		}

	private:
		RunOptions _options;
	};
}
