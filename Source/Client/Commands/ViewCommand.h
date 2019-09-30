// <copyright file="ViewCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "ViewOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// View Command
	/// </summary>
	class ViewCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ViewCommand"/> class.
		/// </summary>
		ViewCommand(ViewOptions options) ://ISoupApi soupApi)
			_options(std::move(options))
		{
			//_soupApi = soupApi;
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Trace("ViewCommand::Run");
			// var packageName = options.Package;
			// try
			// {
			//	 var package = await _soupApi.GetPackageAsync(packageName);
			//	 var output = JsonConvert.SerializeObject(package);
			//	 Log.Info(output);
			// }
			// catch (ApiException ex)
			// {
			//	 if (ex.StatusCode == HttpStatusCode.NotFound)
			//	 {
			//		 Log.Warning("The requested package does not exist.");
			//	 }
			//	 else
			//	 {
			//		 Log.Error(ex.ToString());
			//	 }
			// }
		}

	private:
		ViewOptions _options;
		// ISoupApi _soupApi;
	};
}
