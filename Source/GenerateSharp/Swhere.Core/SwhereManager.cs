// <copyright file="SwhereManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Discover
{
	using System.Collections.Generic;
	using System.Threading.Tasks;
	using Opal;
	using Opal.System;
	using Soup.Build.Utilities;

	public class SwhereManager
	{
		public static async Task DiscoverAsync(bool includePrerelease)
		{
			// Load up the Local User Config
			var localUserConfigPath = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
				new Path(".soup/LocalUserConfig.sml");
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
		}

		private static void PrintUsage()
		{
			Log.Info("Soup.Build.Discover.exe");
		}
	}
}
