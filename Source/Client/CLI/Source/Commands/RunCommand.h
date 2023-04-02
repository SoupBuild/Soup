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
				// Build in the current directory
				workingDirectory = System::IFileSystem::Current().GetCurrentDirectory();
			}
			else
			{
				workingDirectory = Path(_options.Path);

				// Check if this is relative to current directory
				if (!workingDirectory.HasRoot())
				{
					workingDirectory = System::IFileSystem::Current().GetCurrentDirectory() + workingDirectory;
				}
			}

			// Load the recipe
			auto recipeCache = Core::RecipeCache();
			auto recipePath =
				workingDirectory +
				Core::BuildConstants::RecipeFileName();
			const Core::Recipe* recipe;
			if (!recipeCache.TryGetOrLoadRecipe(recipePath, recipe))
			{
				Log::Error("The Recipe does not exist: " + recipePath.ToString());
				Log::HighPriority("Make sure the path is correct and try again");

				// Nothing we can do, exit
				throw Core::HandledException(1234);
			}

			// Setup the build parameters
			auto globalParameters = Core::ValueTable();

			// Process well known parameters
			if (!_options.Flavor.empty())
				globalParameters.emplace("Flavor", Core::Value(_options.Flavor));
			if (!_options.Architecture.empty())
				globalParameters.emplace("Architecture", Core::Value(_options.Architecture));

			// TODO: Generic parameters

			// Load the value table to get the exe path
			auto knownLanguages = Core::BuildEngine::GetKnownLanguages();
			auto locationManager = Core::RecipeBuildLocationManager(knownLanguages);
			auto targetDirectory = locationManager.GetOutputDirectory(
				workingDirectory,
				*recipe,
				globalParameters,
				recipeCache);
			auto soupTargetDirectory = targetDirectory + Core::BuildConstants::SoupTargetDirectory();

			// Load the shared state file
			auto generateInputFile = soupTargetDirectory + Core::BuildConstants::GenerateInputFileName();
			auto generateInputTable = Core::ValueTable();
			if (!Core::ValueTableManager::TryLoadState(generateInputFile, generateInputTable))
			{
				Log::Error("Failed to load the generate input file: " + generateInputFile.ToString());
				return;
			}

			// Load the input macro definition
			auto macros = std::map<std::string, std::string>();
			for (auto& [key, value] : generateInputTable.at("EvaluateMacros").AsTable())
				macros.emplace(key, value.AsString());

			// Setup a macro manager to resolve macros
			auto macroManager = Core::MacroManager( macros);

			// Load the shared state file
			auto sharedStateFile = soupTargetDirectory + Core::BuildConstants::GenerateSharedStateFileName();
			auto sharedStateTable = Core::ValueTable();
			if (!Core::ValueTableManager::TryLoadState(sharedStateFile, sharedStateTable))
			{
				Log::Error("Failed to load the shared state file: " + sharedStateFile.ToString());
				return;
			}

			// Get the executable from the build target file property
			// Check for any dynamic libraries in the shared state
			if (!sharedStateTable.contains("Build"))
			{
				Log::Error("Shared state does not have a build table");
				return;
			}

			auto& buildTable = sharedStateTable.at("Build").AsTable();
			if (!buildTable.contains("RunExecutable"))
			{
				Log::Error("Build table does not have a RunExecutable property");
				return;
			}

			if (!buildTable.contains("RunArguments"))
			{
				Log::Error("Build table does not have a RunArguments property");
				return;
			}

			auto runExecutable = Path(macroManager.ResolveMacros(buildTable.at("RunExecutable").AsString()));
			Log::Info("Executable: " + runExecutable.ToString());
			if (!System::IFileSystem::Current().Exists(runExecutable))
			{
				Log::Error("The run executable does not exist");
				return;
			}

			auto arguments = std::vector<std::string>();
			auto runArguments = buildTable.at("RunArguments").AsList();
			for (auto& value : runArguments)
			{
				arguments.push_back(macroManager.ResolveMacros(value.AsString()));
			}

			for (auto& argument : _options.Arguments)
			{
				arguments.push_back(argument);
			}

			// Execute the requested target
			Log::Info("CreateProcess");
			auto process = System::IProcessManager::Current().CreateProcess(
				runExecutable,
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
		RunOptions _options;
	};
}
