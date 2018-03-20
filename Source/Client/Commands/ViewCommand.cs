// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using Newtonsoft.Json;
using Soup.Api;
using System.Net.Http;
using System.Threading.Tasks;

namespace Soup
{
	class ViewCommand : ICommand
	{
		public string Name => "view";

		/// <summary>
		/// Invoke the view command
		/// </summary>
		public async Task InvokeAsync(string[] args, LocalUserConfig userConfig)
		{
			if (args.Length < 2)
			{
				ShowUsage();
				return;
			}

			var packageName = args[1];
			var api = new SoupApi();
			try
			{
				var package = await api.GetPackageAsync(packageName);
				var output = JsonConvert.SerializeObject(package);
				Log.Message(output);
			}
			catch (HttpRequestException ex)
			{
				Log.Error(ex.ToString());
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
