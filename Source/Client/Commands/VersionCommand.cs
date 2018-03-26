// <copyright company="Soup" file="VersionCommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.Reflection;
	using System.Threading.Tasks;

	/// <summary>
	/// Version Command
	/// </summary>
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
