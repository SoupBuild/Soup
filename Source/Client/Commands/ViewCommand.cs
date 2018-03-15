// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace Soup
{
	class ViewCommand : ICommand
	{
		public string Name => "views";

		/// <summary>
		/// Invoke the view command
		/// </summary>
		public async Task InvokeAsync(string[] args, LocalUserConfig userConfig)
		{
			if (args.Length < 1)
			{
				ShowUsage();
				return;
			}

			await Task.CompletedTask;
		}

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		private static void ShowUsage()
		{
			Log.Message("");
			Log.Message("Usage: soup view <package>");
			Log.Message("\name: The unique package name.");
		}
	}
}
