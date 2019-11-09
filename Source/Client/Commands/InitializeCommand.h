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
			Log::Info("The initialize utility will walk through the creation of the most basic Console recipe.\n");

			// Use the current directory as the default names
			auto workingDirectory = Path::GetCurrentDirectory();
			auto recipePath = 
				workingDirectory +
				Path(Constants::RecipeFileName);

			auto recipe = Recipe(workingDirectory.GetFileName(), SemanticVersion(1, 0, 0));

			Log::Info("Name: (" + recipe.GetName() + ")");
			auto newName = std::string();
			std::getline(std::cin, newName);
			if (!newName.empty())
			{
				recipe.SetName(newName);
			}

			bool setVersion = false;
			while (!setVersion)
			{
				Log::Info("Version: (" + recipe.GetVersion().ToString() + ")");
				auto newVersion = std::string();
				std::getline(std::cin, newVersion);
				if (newVersion.empty())
				{
					// Use the default
					setVersion = true;
				}
				else
				{
					auto value = SemanticVersion();
					if (SemanticVersion::TryParse(newVersion, value))
					{
						recipe.SetVersion(value);
						setVersion = true;
					}
					else
					{
						Log::Warning("Invalid version: \"" + newVersion + "\"");
					}
				}
			}

			// Save the state of the recipe if it has changed
			RecipeExtensions::SaveToFile(recipePath, recipe);
		}

	private:
		InitializeOptions _options;
	};
}
