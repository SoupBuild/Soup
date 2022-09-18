// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Discover
{
	using System;
	using System.Threading.Tasks;
	using Opal;
	using Opal.System;

	public class Program
	{
		public static async Task<int> Main(string[] args)
		{
			try
			{
				var traceFlags =
					TraceEventFlag.Information |
					TraceEventFlag.HighPriority |
					TraceEventFlag.Critical |
					TraceEventFlag.Warning |
					TraceEventFlag.Error;
				Log.RegisterListener(new ConsoleTraceListener("", new EventTypeFilter(traceFlags), false, false));
				LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();

				LifetimeManager.RegisterSingleton<IProcessManager, RuntimeProcessManager>();

				if (args.Length != 0)
				{
					PrintUsage();
					return -1;
				}

				bool includePrerelease = true;

				await SwhereManager.DiscoverAsync(includePrerelease);

				return 0;
			}
			catch (HandledException)
			{
				return -1;
			}
			catch (Exception ex)
			{
				Log.HighPriority($"Unhandled Error: {ex}");
				return -2;
			}
		}

		private static void PrintUsage()
		{
			Log.Info("Soup.Build.Discover.exe");
		}
	}
}
