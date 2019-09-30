// <copyright file="PublishCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "PublishOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Publish Command
	/// </summary>
	class PublishCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PublishCommand"/> class.
		/// </summary>
		PublishCommand(PublishOptions options) :
			_options(std::move(options))
			// ISoupIdentity soupIdentity,
			// ISoupApi soupApi)
		{
			// _soupIdentity = soupIdentity;
			// _soupApi = soupApi;
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Trace("PublishCommand::Run");
			// var projectDirectory = Directory.GetCurrentDirectory();
			// var recipe = await RecipeManager.LoadFromFileAsync(projectDirectory);
			// Log.Info($"Publish Project: {recipe.Name}@{recipe.Version}");

			// var result = await _soupIdentity.AuthenticateUserAsync();

			// using (var stream = new MemoryStream())
			// {
			//	 // Pack the project into a memory stream
			//	 await PackageManager.PackAsync(recipe, projectDirectory, stream);

			//	 // Reset the stream so we can read from it
			//	 stream.Seek(0, SeekOrigin.Begin);

			//	 // Publish the package to the service
			//	 try
			//	 {
			//		 bool created = await _soupApi.PublishPackageAsync(recipe.Name, stream);
			//		 if (!created)
			//		 {
			//			 Log.Warning("The package version already existed! No change was made.");
			//		 }
			//	 }
			//	 catch (HttpRequestException ex)
			//	 {
			//		 Log.Error($"Failed request: {ex.ToString()}");
			//	 }
			// }
		}

	private:
		PublishOptions _options;
		// ISoupIdentity _soupIdentity;
		// ISoupApi _soupApi;
	};
}
