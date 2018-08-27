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
		/// <summary>
		/// Invoke the view command
		/// </summary>
		public async Task InvokeAsync(ViewOptions options)
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
	}
}
