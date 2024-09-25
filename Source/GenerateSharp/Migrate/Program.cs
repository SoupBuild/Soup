// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Threading.Tasks;
using Path = Opal.Path;

namespace Soup.Build.Migrate;

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

			if (args.Length != 1)
			{
				PrintUsage();
				return -1;
			}

			var target = new Path(args[0]);
			await MigrationManager.RunAsync(target);

			return 0;
		}
		catch (HandledException)
		{
			return -2;
		}
	}

	private static void PrintUsage()
	{
		Log.Info("migrate <PATH>");
	}
}