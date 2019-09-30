// <copyright file="PackCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "PackOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Pack Command
	/// </summary>
	class PackCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PackCommand"/> class.
		/// </summary>
		PackCommand(PackOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Trace("PackCommand::Run");
			// var recipe = await RecipeManager.LoadFromFileAsync(@"./");
			// Log.Info($"Packaging Project: {recipe.Name}@{recipe.Version}");

			// await PackageManager.PackAsync(recipe, Directory.GetCurrentDirectory());
		}

	private:
		PackOptions _options;
	};
}
