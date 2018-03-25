// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Reflection;
using System.Threading.Tasks;

namespace Soup.Client
{
	internal class VersionCommand : ICommand
	{
		public string Name => "version";

		public Task InvokeAsync(string[] args)
		{
			var version = Assembly.GetExecutingAssembly().GetName().Version;
			Log.Message($"{version.Major}.{version.Minor}.{version.Build}");
			return Task.CompletedTask;
		}
	}
}
