// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Path = Opal.Path;

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

			Path rootDirectory;
			if (args.Length == 1)
			{
				rootDirectory = new Path(args[0]);
			}
			else
			{
				PrintUsage();
				return -1;
			}

			var script = rootDirectory + new Path("build");
			var builder = new ScriptBuilder(script);
			await builder.BuildScriptAsync();

			return 0;
		}
		catch (HandledException)
		{
			return -1;
		}
	}

	private static void PrintUsage()
	{
		Log.Info("soupbootstrap [path]");
	}
}