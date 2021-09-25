// <copyright file="WindowsSDKUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;

namespace Soup.Build.Discover
{
	internal static class WindowsSDKUtilities
	{
		public static (string Version, Path Path) FindWindows10Kit()
		{
			var windowsSDKInstallPath = new Path("C:/Program Files (x86)/Windows Kits/10/");
			var windowsSDKVersion = FindNewestWindows10KitVersion(windowsSDKInstallPath);

			return (windowsSDKVersion, windowsSDKInstallPath);
		}

		private static string FindNewestWindows10KitVersion(Path windowsSDKInstallPath)
		{
			var windowsSDKIncludePath = windowsSDKInstallPath + new Path("/include/");

			// Check the default tools version
			Log.HighPriority("FindNewestWindows10KitVersion: " + windowsSDKInstallPath.ToString());
			var currentVersion = new SemanticVersion(0, 0, 0);
			foreach (var child in System.IO.Directory.EnumerateDirectories(windowsSDKIncludePath.ToString()))
			{
				var name = new Path(child).GetFileName();
				Log.Info("CheckFile: " + name);
				var platformVersion = name.Substring(0, 3);
				if (platformVersion == "10.")
				{
					// Parse the version string
					var version = SemanticVersion.Parse(name.Substring(3));
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
}