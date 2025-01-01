// <copyright file="WindowsSDKUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Linq;
using System.Runtime.Versioning;
using Path = Opal.Path;

namespace Soup.Build.Discover;

[SupportedOSPlatform("windows")]
internal static class WindowsSDKUtilities
{
	private static string WindowsKitsRegistryKey => @"HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Microsoft SDKs\Windows\v10.0";

	public static (string Version, Path InstallPath)? FindWindows10Kit()
	{
		var installFolder = (string?)LifetimeManager.Get<ISystem>().GetRegistryValue(
			WindowsKitsRegistryKey, "InstallationFolder", null);

		if (installFolder is null)
			return null;

		var windowsSDKInstallPath = Path.Parse(installFolder);
		var windowsSDKVersion = FindNewestWindows10KitVersion(windowsSDKInstallPath);

		return (windowsSDKVersion, windowsSDKInstallPath);
	}

	public static Path FindNetFXTools()
	{
		return new Path("C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/");
	}

	private static string FindNewestWindows10KitVersion(Path windowsSDKInstallPath)
	{
		var windowsSDKIncludePath = windowsSDKInstallPath + new Path("./include/");

		// Check the default tools version
		Log.HighPriority("FindNewestWindows10KitVersion: " + windowsSDKInstallPath.ToString());
		var currentVersion = new SemanticVersion(0, 0, 0);
		foreach (var child in LifetimeManager.Get<IFileSystem>().GetChildDirectories(windowsSDKIncludePath))
		{
			var name = child.Path.DecomposeDirectories().Last();
			Log.Info("CheckFile: " + name);
			var platformVersion = name[..3];
			if (platformVersion == "10.")
			{
				// Parse the version string
				var version = SemanticVersion.Parse(name[3..]);
				if (version > currentVersion)
					currentVersion = version;
			}
			else
			{
				Log.Warning("Unexpected Kit Version: " + name);
			}
		}

		if (currentVersion == new SemanticVersion(0, 0, 0))
		{
			Log.Error("Could not find a minimum Windows 10 Kit Version");
			throw new HandledException();
		}

		// The first line is the version
		var result = "10." + currentVersion.ToString();
		return result;
	}
}