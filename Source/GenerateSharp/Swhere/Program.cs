// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System;
using System.Threading.Tasks;

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

			bool includePrerelease = false;
			if (args.Length == 0)
			{
			}
			else if (args.Length == 1 && args[0] == "-prerelease")
			{
				includePrerelease = true;
			}
			else
			{
				PrintUsage();
				return -1;
			}

			if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(System.Runtime.InteropServices.OSPlatform.Windows))
			{
				await SwhereManager.DiscoverAsync(OSPlatform.Windows, includePrerelease);
			}
			else if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(System.Runtime.InteropServices.OSPlatform.Linux))
			{
				await SwhereManager.DiscoverAsync(OSPlatform.Linux, includePrerelease);
			}
			else
			{
				throw new NotSupportedException("Unknown OS Platform");
			}

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