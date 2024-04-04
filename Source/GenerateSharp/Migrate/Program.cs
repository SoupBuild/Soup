// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;

namespace Soup.Build.Discover;

public static class Program
{
	public static async Task<int> Main(string[] args)
	{
		try
		{
			var traceFlags =
				TraceEvents.Information |
				TraceEvents.HighPriority |
				TraceEvents.Critical |
				TraceEvents.Warning |
				TraceEvents.Error;
			Log.RegisterListener(new ConsoleTraceListener(new EventTypeFilter(traceFlags), false, false));
			LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();

			LifetimeManager.RegisterSingleton<IProcessManager, RuntimeProcessManager>();

			if (args.Length == 0)
			{
			}
			else
			{
				PrintUsage();
				return -1;
			}

			await Task.CompletedTask;
			// await SwhereManager.DiscoverAsync(includePrerelease);

			return 0;
		}
		catch (HandledException)
		{
			return -1;
		}
	}

	private static void PrintUsage()
	{
		Log.Info("swhere [-prerelease]");
	}
}