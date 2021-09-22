// <copyright file="Discovery.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Discover
{
	using System;
	using System.Collections.Generic;
	using System.Threading.Tasks;
	using Opal;
	using Opal.System;
	using Soup.Build.Utilities;

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

				// Load up the Local User Config
				var localUserConfigPath = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
					new Path(".soup/LocalUserConfig2.toml");
				var (loadConfigResult, userConfig) = await LocalUserConfigExtensions.TryLoadLocalUserConfigFromFileAsync(localUserConfigPath);
				if (!loadConfigResult)
				{
					Log.Info("No existing local user config.");
				}

				// Find the Roslyn SDKs
				var path = await VSWhereUtilities.FindRoslynInstallAsync();

				var roslynSDK = userConfig.EnsureSDK("Roslyn");
				roslynSDK.SourceDirectories = new List<Path>()
				{
					path,
				};
				roslynSDK.SetProperties(
					new Dictionary<string, string>()
					{
						{ "ToolsRoot", path.ToString() }
					});

				// Save the result
				await LocalUserConfigExtensions.SaveToFileAsync(localUserConfigPath, userConfig);

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
