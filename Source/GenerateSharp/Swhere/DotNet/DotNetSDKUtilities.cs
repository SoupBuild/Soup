﻿// <copyright file="DotNetSDKUtilities.cs" company="Soup">
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
		Path dotnetInstallPath;
		string newline;
		switch (platform)
		{
			case OSPlatform.Windows:
				dotnetInstallPath = new Path("C:/Program Files/dotnet/");
				newline = "\r\n";
				break;
			case OSPlatform.Linux:
				dotnetInstallPath = new Path("/usr/lib/dotnet/");
				newline = "\n";
				break;
			default:
				throw new InvalidOperationException("Unsupported operating system");
		};

		// Grant access to the install folder
		var sourceDirectories = new List<Path>()
		{
			dotnetInstallPath,
		};

		var dotnetSDKs = await FindDotNetSDKVersionsAsync(dotnetExecutablePath, newline);
		var dotnetRuntimes = await FindDotNetRuntimeVersionsAsync(dotnetExecutablePath, newline);
		var dotnetTargetingPacks = FindDotNetTargetingPacksVersions(dotnetInstallPath);

		return (dotnetExecutablePath, dotnetSDKs, dotnetRuntimes, dotnetTargetingPacks, sourceDirectories);
	}

	private static async Task<IList<(string Version, Path InstallDirectory)>> FindDotNetSDKVersionsAsync(
		Path dotnetExecutablePath,
		string newline)
	{
		// Check the default tools version
		Log.HighPriority("Find DotNet SDK Versions");
		var sdksOutput = await ExecutableUtilities.RunExecutableAsync(
			dotnetExecutablePath,
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
			var installationPath = Path.Parse($"{installationValue}\\{version}\\");

			Log.Info($"Found SDK: {version} {installationPath}");
			sdks.Add((version, installationPath));
		}

		return sdks;
	}

	private static async Task<IDictionary<string, IList<(string Version, Path InstallDirectory)>>> FindDotNetRuntimeVersionsAsync(
		Path dotnetExecutablePath,
		string newline)
	{
		// Check the default tools version
		Log.HighPriority("Find DotNet Runtime Versions");
		var runtimesOutput = await ExecutableUtilities.RunExecutableAsync(
			dotnetExecutablePath,
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
			var installationPath = Path.Parse($"{installationValue}\\");

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
		Path dotnetInstallPath)
	{
		var knownPacks = new List<string>()
		{
			"Microsoft.NETCore.App.Ref",
		};

		var result = new Dictionary<string, IList<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)>>();
		foreach (var packageName in knownPacks)
		{
			var packageVersions = FindDotNetTargetingPackVersions(dotnetInstallPath, packageName);
			result.Add(packageName, packageVersions);
		}

		return result;
	}

	private static List<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)> FindDotNetTargetingPackVersions(
		Path dotnetInstallPath,
		string packageName)
	{
		var dotnetPacksPath = dotnetInstallPath + new Path($"./packs/{packageName}/");

		// Check the default tools version
		Log.HighPriority("FindDotNetPackVersions: " + dotnetPacksPath.ToString());
		var versions = new List<(string Version, Path InstallDirectory, FrameworkFileList? FrameworkList)>();
		if (LifetimeManager.Get<IFileSystem>().Exists(dotnetPacksPath))
		{
			foreach (var child in LifetimeManager.Get<IFileSystem>().GetChildDirectories(dotnetPacksPath))
			{
				var folderName = child.Path.DecomposeDirectories().Last();
				var frameworkList = LoadFrameworkList(child.Path);
				versions.Add((folderName, child.Path, frameworkList));
			}
		}
		else
		{
			Log.Warning("Directory does not exist");
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