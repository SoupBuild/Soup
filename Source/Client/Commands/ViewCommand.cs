// <copyright company="Soup" file="ViewCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.Net;
	using System.Threading.Tasks;
	using Newtonsoft.Json;
	using Soup.Api;

	/// <summary>
	/// View Command
	/// </summary>
	internal class ViewCommand
	{
		private ISoupApi _soupApi;

		public ViewCommand(
			ISoupApi soupApi)
		{
			_soupApi = soupApi;
		}

		/// <summary>
		/// Invoke the view command
		/// </summary>
		public async Task InvokeAsync(ViewOptions options)
		{
			var packageName = options.Package;
			try
			{
				var package = await _soupApi.GetPackageAsync(packageName);
				var output = JsonConvert.SerializeObject(package);
				Log.Info(output);
			}
			catch (ApiException ex)
			{
				if (ex.StatusCode == HttpStatusCode.NotFound)
				{
					Log.Warning("The requested package does not exist.");
				}
				else
				{
					Log.Error(ex.ToString());
				}
			}
		}
	}
}
