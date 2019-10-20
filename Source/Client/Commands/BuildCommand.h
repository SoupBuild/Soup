// <copyright file="BuildCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "BuildOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Build Command
	/// </summary>
	class BuildCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildCommand"/> class.
		/// </summary>
		BuildCommand(BuildOptions options, std::shared_ptr<ICompiler> compiler) :
			_options(std::move(options)),
			_compiler(std::move(compiler))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Trace("BuildCommand::Run");
			auto workingDirectory = Path();
			if (_options.Path.empty())
			{
				// Buildin the current directory
				workingDirectory = Path::GetCurrentDirectory();
			}
			else
			{
				workingDirectory = Path(_options.Path);

				// Check if this is relative to current directory
				if (!workingDirectory.HasRoot())
				{
					workingDirectory = Path::GetCurrentDirectory() + workingDirectory;
				}
			}
			
			auto recipePath = 
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadFromFile(recipePath, recipe))
			{
				Log::Error("Could not load the recipe file.");
				return;
			}

			// Setup the build arguments
			auto arguments = RecipeBuildArguments();
			arguments.ForceRebuild = _options.Force;
			if (!_options.Configuration.empty())
				arguments.Configuration = _options.Configuration;
			else
				arguments.Configuration = "release";

			// Now build the current project
			Log::Verbose("Begin Build:");
			auto buildManager = RecipeBuildManager(_compiler);
			buildManager.Execute(workingDirectory, recipe, arguments);
		}

	private:
		BuildOptions _options;
		std::shared_ptr<ICompiler> _compiler;
	};
}
