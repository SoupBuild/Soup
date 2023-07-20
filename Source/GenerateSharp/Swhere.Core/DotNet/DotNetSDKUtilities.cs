// <copyright file="DotNetSDKUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Opal;

namespace Soup.Build.Discover
{
	public static class DotNetSDKUtilities
	{
		public static async Task<(
			Path DotNetExecutable,
			IList<(string Version, Path InstallDirectory)> SDKVersions,
			IDictionary<string, IList<(string Version, Path InstallDirectory)>> Runtimes)> FindDotNetAsync()
		{
			var dotnetExecutablePath = await WhereIsUtilities.FindExecutableAsync("dotnet");
			Log.HighPriority($"Using DotNet: {dotnetExecutablePath}");

			var dotnetSDKs = await FindDotNetSDKVersionsAsync(dotnetExecutablePath);
			var dotnetRuntimes = await FindDotNetRuntimeVersionsAsync(dotnetExecutablePath);

			return (dotnetExecutablePath, dotnetSDKs, dotnetRuntimes);
		}

		private static async Task<IList<(string Version, Path InstallDirectory)>> FindDotNetSDKVersionsAsync(
			Path dotnetExecutablePath)
		{
			// Check the default tools version
			Log.HighPriority("Find DotNet SDK Versions");
			var sdksOutput = await ExecutableUtilities.RunExecutableAsync(
				dotnetExecutablePath,
				new List<string>() { "--list-sdks" });
			var sdks = new List<(string, Path)>();
			foreach (var sdkValue in sdksOutput.Split(Environment.NewLine).SkipLast(1))
			{
				var splitIndex = sdkValue.IndexOf(' ');
				if (splitIndex == -1)
				{
					Log.Error($"DotNet SDK format invalid: {sdkValue}");
					throw new HandledException();
				}

				var version = sdkValue.Substring(0, splitIndex);
				var installationValue = sdkValue.Substring(splitIndex + 2, sdkValue.Length - splitIndex - 3);
				var installationPath = new Path(installationValue);

				Log.Info($"Found SDK: {version} {installationPath}");
				sdks.Add((version, installationPath));
			}

			return sdks;
		}

		private static async Task<IDictionary<string, IList<(string Version, Path InstallDirectory)>>> FindDotNetRuntimeVersionsAsync(
			Path dotnetExecutablePath)
		{
			// Check the default tools version
			Log.HighPriority("Find DotNet Runtime Versions");
			var runtimesOutput = await ExecutableUtilities.RunExecutableAsync(
				dotnetExecutablePath,
				new List<string>() { "--list-runtimes" });
			var runtimes = new Dictionary<string, IList<(string, Path)>>();
			foreach (var runtimeValue in runtimesOutput.Split(Environment.NewLine).SkipLast(1))
			{
				var split1Index = runtimeValue.IndexOf(' ');
				var split2Index = runtimeValue.IndexOf(' ', split1Index + 1);
				if (split1Index == -1 || split2Index == -1)
				{
					Log.Error($"DotNet Runtime format invalid: {runtimeValue}");
					throw new HandledException();
				}

				var name = runtimeValue.Substring(0, split1Index);
				var version = runtimeValue.Substring(split1Index + 1, split2Index - split1Index - 1);
				var installationValue = runtimeValue.Substring(split2Index + 2, runtimeValue.Length - split2Index - 3);
				var installationPath = new Path(installationValue.Substring(1, installationValue.Length - 2));

				Log.Info($"Found Runtime: {name} {version} {installationPath}");

				if (runtimes.TryGetValue(name, out var runtime))
				{
					runtime.Add((version, installationPath));
				}
				else
				{
					runtimes.Add(name, new List<(string, Path)>() { (version, installationPath) });
				}
			}

			return runtimes;
		}
	}
}