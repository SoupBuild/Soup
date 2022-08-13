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

				bool includePrerelease = true;

				// Load up the Local User Config
				var localUserConfigPath = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
					new Path(".soup/LocalUserConfig.toml");
				var (loadConfigResult, userConfig) = await LocalUserConfigExtensions.TryLoadLocalUserConfigFromFileAsync(localUserConfigPath);
				if (!loadConfigResult)
				{
					Log.Info("No existing local user config.");
				}

				// Find the Roslyn SDKs
				var roslynInstallPath = await VSWhereUtilities.FindRoslynInstallAsync(includePrerelease);

				var roslynSDK = userConfig.EnsureSDK("Roslyn");
				roslynSDK.SourceDirectories = new List<Path>()
				{
					roslynInstallPath,
				};
				roslynSDK.SetProperties(
					new Dictionary<string, string>()
					{
						{ "ToolsRoot", roslynInstallPath.ToString() },
					});

				var (dotnetRuntimeVersion, dotnetSDKInstallPath) = DotNetSDKUtilities.FindDotNet6Refs();
				var dotnetSDK = userConfig.EnsureSDK("DotNet");
				dotnetSDK.SourceDirectories = new List<Path>()
				{
					dotnetSDKInstallPath,
				};
				dotnetSDK.SetProperties(
					new Dictionary<string, string>()
					{
						{ "RuntimeVersion", dotnetRuntimeVersion },
						{ "RootPath", dotnetSDKInstallPath.ToString() },
					});

				var (msvcVersion, msvcInstallPath) = await VSWhereUtilities.FindMSVCInstallAsync(includePrerelease);
				var msvcSDK = userConfig.EnsureSDK("MSVC");
				msvcSDK.SourceDirectories = new List<Path>()
				{
					msvcInstallPath,
				};
				msvcSDK.SetProperties(
					new Dictionary<string, string>()
					{
						{ "Version", msvcVersion },
						{ "VCToolsRoot", msvcInstallPath.ToString() },
					});

				var (windowsSDKVersion, windowsSDKInstallPath) = WindowsSDKUtilities.FindWindows10Kit();
				var windowsSDK = userConfig.EnsureSDK("Windows");
				windowsSDK.SourceDirectories = new List<Path>()
				{
					windowsSDKInstallPath,
				};
				windowsSDK.SetProperties(
					new Dictionary<string, string>()
					{
						{ "Version", windowsSDKVersion },
						{ "RootPath", windowsSDKInstallPath.ToString() },
					});

				var netFXToolsPath = WindowsSDKUtilities.FindNetFXTools();
				var netFXToolsSDK = userConfig.EnsureSDK("NetFXTools");
				netFXToolsSDK.SourceDirectories = new List<Path>()
				{
					netFXToolsPath,
				};
				netFXToolsSDK.SetProperties(
					new Dictionary<string, string>()
					{
						{ "ToolsRoot", netFXToolsPath.ToString() },
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
