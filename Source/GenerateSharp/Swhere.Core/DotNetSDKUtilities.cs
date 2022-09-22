// <copyright file="DotNetSDKUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;

namespace Soup.Build.Discover
{
	public static class DotNetSDKUtilities
	{
		public static (string Version, Path Path) FindDotNet6Refs()
		{
			var dotnetSDKInstallPath = new Path("C:/Program Files/dotnet/");
			var dotNetRuntimeVersion = FindNewestDotNet6RuntimeVersion(dotnetSDKInstallPath);

			return (dotNetRuntimeVersion, dotnetSDKInstallPath);
		}

		private static string FindNewestDotNet6RuntimeVersion(Path dotnetSDKInstallPath)
		{
			var dotnetAppRefsPath = dotnetSDKInstallPath + new Path("/packs/Microsoft.NETCore.App.Ref/");

			// Check the default tools version
			Log.HighPriority("FindNewestDotNet6RuntimeVersion: " + dotnetAppRefsPath.ToString());
			var currentVersion = new SemanticVersion(0, 0, 0);
			foreach (var child in LifetimeManager.Get<IFileSystem>().GetDirectoryChildren(dotnetAppRefsPath))
			{
				var name = child.Path.GetFileName();
				Log.Info("CheckFile: " + name);

				// Attempt to parse the version string
				if (SemanticVersion.TryParse(name, out var version))
				{
					if (version.Major == 6 && version > currentVersion)
						currentVersion = version;
				}
			}

			if (currentVersion == new SemanticVersion(0, 0, 0))
			{
				Log.Error("Could not find a minimum DotNet 6 Runtime Version");
				throw new HandledException();
			}

			// The first line is the version
			var result = currentVersion.ToString();
			return result;
		}
	}
}