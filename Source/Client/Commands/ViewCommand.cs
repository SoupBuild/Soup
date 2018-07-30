// <copyright company="Soup" file="ViewCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.Net;
	using System.Net.Http;
	using System.Threading.Tasks;
	using Newtonsoft.Json;
	using Soup.Api;

	/// <summary>
	/// View Command
	/// </summary>
	internal class ViewCommand : ICommand
	{
		public string Name => "view";

		/// <summary>
		/// Invoke the view command
		/// </summary>
		public async Task InvokeAsync(string[] args)
		{
			if (args.Length < 2)
			{
				ShowUsage();
				return;
			}

			var packageName = args[1];
			try
			{
				var package = await Singleton<ISoupApi>.Instance.GetPackageAsync(packageName);
				var output = JsonConvert.SerializeObject(package);
				Log.Message(output);
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

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		private static void ShowUsage()
		{
			Log.Message("");
			Log.Message("Usage: soup view <package>");
			Log.Message("\tpackage: The unique package name.");
		}
	}
}
