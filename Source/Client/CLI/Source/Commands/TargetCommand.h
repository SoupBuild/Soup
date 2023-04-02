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

			std::cout << targetDirectory.ToString() << std::flush;
		}

	private:
		TargetOptions _options;
	};
}
