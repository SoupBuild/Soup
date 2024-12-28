// <copyright file="DotNetSDKUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Xml;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class DotNetSDKUtilities
{
	public static async Task<(
		Path DotNetExecutable,
		IList<(string Version, Path InstallDirectory)> SDKVersions,
		IDictionary<string, IList<(string Version, Path InstallDirectory)>> Runtimes,
		IDictionary<string, IList<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)>> TargetingPacks,
		IList<Path> SourceDirectories)> FindDotNetAsync(OSPlatform platform)
	{
		var dotnetExecutablePath = await WhereIsUtilities.FindExecutableAsync(platform, "dotnet");
		Log.HighPriority($"Using DotNet: {dotnetExecutablePath}");
		Path dotnetInstallPath = dotnetExecutablePath.GetParent();
		string newline = platform switch
		{
			OSPlatform.Windows => "\r\n",
			OSPlatform.Linux => "\n",
			_ => throw new InvalidOperationException("Unsupported operating system"),
		};

		// Grant access to the install folder
		var sourceDirectories = new HashSet<Path>()
		{
			dotnetInstallPath,
		};

		var dotnetSDKs = await FindDotNetSDKVersionsAsync(sourceDirectories, dotnetExecutablePath, newline);
		var dotnetRuntimes = await FindDotNetRuntimeVersionsAsync(sourceDirectories, dotnetExecutablePath, newline);
		var dotnetTargetingPacks = FindDotNetTargetingPacksVersions(sourceDirectories, dotnetRuntimes);

		return (dotnetExecutablePath, dotnetSDKs, dotnetRuntimes, dotnetTargetingPacks, sourceDirectories.ToList());
	}

	private static async Task<IList<(string Version, Path InstallDirectory)>> FindDotNetSDKVersionsAsync(
		HashSet<Path> sourceDirectories,
		Path dotnetExecutablePath,
		string newline)
	{
		// Check the default tools version
		Log.HighPriority("Find DotNet SDK Versions");
		var sdksOutput = await ExecutableUtilities.RunExecutableAsync(
			dotnetExecutablePath.ToString(),
			["--list-sdks"]);
		var sdks = new List<(string, Path)>();
		foreach (var sdkValue in sdksOutput.Split(newline).SkipLast(1))
		{
			var splitIndex = sdkValue.IndexOf(' ', StringComparison.InvariantCulture);
			if (splitIndex == -1)
			{
				Log.Error($"DotNet SDK format invalid: {sdkValue}");
				throw new HandledException();
			}

			var version = sdkValue[..splitIndex];
			var installationValue = sdkValue.Substring(splitIndex + 2, sdkValue.Length - splitIndex - 3);
			var installationPath = Path.Parse(installationValue);
			var installationVersionPath = installationPath + new Path($"./{version}/");

			// Ensure we have read access to the sdk
			SourceSetUtilities.Add(sourceDirectories, installationPath);

			Log.Info($"Found SDK: {version} {installationVersionPath}");
			sdks.Add((version, installationVersionPath));
		}

		return sdks;
	}

	private static async Task<IDictionary<string, IList<(string Version, Path InstallDirectory)>>> FindDotNetRuntimeVersionsAsync(
		HashSet<Path> sourceDirectories,
		Path dotnetExecutablePath,
		string newline)
	{
		// Check the default tools version
		Log.HighPriority("Find DotNet Runtime Versions");
		var runtimesOutput = await ExecutableUtilities.RunExecutableAsync(
			dotnetExecutablePath.ToString(),
			["--list-runtimes"]);
		var runtimes = new Dictionary<string, IList<(string, Path)>>();
		foreach (var runtimeValue in runtimesOutput.Split(newline).SkipLast(1))
		{
			var split1Index = runtimeValue.IndexOf(' ', StringComparison.InvariantCulture);
			var split2Index = runtimeValue.IndexOf(' ', split1Index + 1);
			if (split1Index == -1 || split2Index == -1)
			{
				Log.Error($"DotNet Runtime format invalid: {runtimeValue}");
				throw new HandledException();
			}

			var name = runtimeValue[..split1Index];
			var version = runtimeValue.Substring(split1Index + 1, split2Index - split1Index - 1);
			var installationValue = runtimeValue.Substring(split2Index + 2, runtimeValue.Length - split2Index - 3);
			var installationPath = Path.Parse($"{installationValue}/");

			// Ensure we have read access to the runtime
			SourceSetUtilities.Add(sourceDirectories, installationPath);

			Log.Info($"Found Runtime: {name} {version} {installationPath}");

			if (runtimes.TryGetValue(name, out var runtime))
			{
				runtime.Add((version, installationPath));
			}
			else
			{
				runtimes.Add(name, [(version, installationPath)]);
			}
		}

		return runtimes;
	}

	private static Dictionary<string, IList<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)>> FindDotNetTargetingPacksVersions(
		HashSet<Path> sourceDirectories,
		IDictionary<string, IList<(string Version, Path InstallDirectory)>> dotnetRuntimes)
	{
		var result = new Dictionary<string, IList<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)>>();
		foreach (var (runtime, versions) in dotnetRuntimes)
		{
			var packName = $"{runtime}.Ref";
			var packageVersions = FindDotNetTargetingPackVersions(sourceDirectories, packName, versions);
			if (packageVersions.Count > 0)
			{
				result.Add(packName, packageVersions);
			}
		}

		return result;
	}

	private static List<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)> FindDotNetTargetingPackVersions(
		HashSet<Path> sourceDirectories,
		string packName,
		IList<(string Version, Path InstallDirectory)> runtimeVersions)
	{
		var versions = new List<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)>();
		foreach (var (version, installDirectory) in runtimeVersions)
		{
			var rootDirectory = installDirectory.GetParent().GetParent();
			var packPath = rootDirectory + new Path($"./packs/");
			var packVersionPath = packPath + new Path($"./{packName}/{version}/");

			// Check the default tools version
			Log.HighPriority("FindDotNetPack: " + packVersionPath.ToString());
			if (LifetimeManager.Get<IFileSystem>().Exists(packVersionPath))
			{
				// Ensure we have read access to the pack
				SourceSetUtilities.Add(sourceDirectories, packPath);

				var frameworkList = LoadFrameworkList(packVersionPath);
				versions.Add((version, packVersionPath, frameworkList));
			}
			else
			{
				Log.Warning("Directory does not exist");
			}
		}

		return versions;
	}

	private static FrameworkFileList? LoadFrameworkList(
		Path dotnetPackPath)
	{
		var fileSystem = LifetimeManager.Get<IFileSystem>();

		// Load the XML file
		var frameworkListFile = dotnetPackPath + new Path("./data/FrameworkList.xml");
		if (!fileSystem.Exists(frameworkListFile))
		{
			Log.Warning($"Missing FrameworkList file: {frameworkListFile}");
			return null;
		}

		using var frameworkList = fileSystem.OpenRead(frameworkListFile);
		using var reader = XmlReader.Create(frameworkList.GetInStream(), new XmlReaderSettings() { Async = true });

		FrameworkFileList fileList;
		try
		{
			fileList = FrameworkFileList.Deserialize(reader);
		}
		catch (InvalidOperationException)
		{
			Log.Warning($"Failed to parse Framework List file: {frameworkListFile}");
			return null;
		}

		return fileList;
	}
}