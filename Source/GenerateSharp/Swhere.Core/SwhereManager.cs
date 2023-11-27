// <copyright file="SwhereManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace Soup.Build.Discover;

public static class SwhereManager
{
	public static async Task DiscoverAsync(bool includePrerelease)
	{
		// Load up the Local User Config
		var localUserConfigPath = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
			new Path(".soup/LocalUserConfig.sml");
		var (loadConfigResult, userConfig) =
			await LocalUserConfigExtensions.TryLoadLocalUserConfigFromFileAsync(localUserConfigPath);
		if (!loadConfigResult)
		{
			Log.Info("No existing local user config.");
		}

		await DiscoverSharedPlatformAsync(userConfig);
		if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
		{
			await DiscoverWindowsPlatformAsync(includePrerelease, userConfig);
		}
		else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
		{
			await DiscoverLinuxPlatformAsync(includePrerelease, userConfig);
		}

		// Save the result
		await LocalUserConfigExtensions.SaveToFileAsync(localUserConfigPath, userConfig);
	}

	private static async Task DiscoverSharedPlatformAsync(LocalUserConfig userConfig)
	{
		var (dotNetExecutable, dotnetSDKs, dotnetRuntimes, dotnetTargetingPacks, sourceDirectories) =
			await DotNetSDKUtilities.FindDotNetAsync();
		var dotnetSDK = userConfig.EnsureSDK("DotNet");
		dotnetSDK.SourceDirectories = sourceDirectories;
		dotnetSDK.SetProperties(
			new Dictionary<string, string>()
			{
				{ "DotNetExecutable", dotNetExecutable.ToString() },
			});

		var sdksTable = dotnetSDK.Properties.EnsureTableWithSyntax("SDKs", 3);
		foreach (var sdk in dotnetSDKs)
		{
			sdksTable.AddItemWithSyntax(sdk.Version, sdk.InstallDirectory.ToString(), 4);
		}

		var runtimesTable = dotnetSDK.Properties.EnsureTableWithSyntax("Runtimes", 3);
		foreach (var runtime in dotnetRuntimes)
		{
			var runtimeTable = runtimesTable.EnsureTableWithSyntax(runtime.Key, 4);
			foreach (var runtimeVersion in runtime.Value)
			{
				runtimeTable.AddItemWithSyntax(runtimeVersion.Version, runtimeVersion.InstallDirectory.ToString(), 5);
			}
		}

		var packsTable = dotnetSDK.Properties.EnsureTableWithSyntax("TargetingPacks", 3);
		foreach (var pack in dotnetTargetingPacks)
		{
			var packTable = packsTable.EnsureTableWithSyntax(pack.Key, 4);
			foreach (var packVersion in pack.Value)
			{
				packTable.AddItemWithSyntax(packVersion.Version, packVersion.InstallDirectory.ToString(), 5);
			}
		}
	}

	private static async Task DiscoverWindowsPlatformAsync(bool includePrerelease, LocalUserConfig userConfig)
	{
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

		var (hasNuget, nugetPackagesPath, nugetPackages) = NugetSDKUtilities.FindNugetPackages();
		if (hasNuget)
		{
			var nugetSDK = userConfig.EnsureSDK("Nuget");
			nugetSDK.SourceDirectories.AddRange(new List<Path>()
				{
					nugetPackagesPath,
				});

			nugetSDK.SetProperties(
				new Dictionary<string, string>()
				{
						{ "PackagesDirectory", nugetPackagesPath.ToString() },
				});

			var packagesTable = nugetSDK.Properties.EnsureTableWithSyntax("Packages", 3);
			packagesTable.Values.Clear();
			foreach (var package in nugetPackages)
			{
				var packageTable = packagesTable.EnsureTableWithSyntax(package.Id, 4);
				foreach (var packageVersion in package.Versions)
				{
					var packageVersionTable = packageTable.EnsureTableWithSyntax(packageVersion.Version, 5);
					if (packageVersion.TargetFrameworks.Count > 0)
					{
						var targetFrameworksTable = packageVersionTable.EnsureTableWithSyntax("TargetFrameworks", 6);
						foreach (var targetFramework in packageVersion.TargetFrameworks)
						{
							var targetFrameworkTable = targetFrameworksTable.EnsureTableWithSyntax(targetFramework.Name, 7);

							if (targetFramework.Dependencies.Count > 0)
							{
								var dependenciesArray = targetFrameworkTable.EnsureArrayWithSyntax("Dependencies", 8);
								foreach (var dependency in targetFramework.Dependencies)
								{
									var dependencyTable = dependenciesArray.AddInlineTableWithSyntax(9);
									dependencyTable.AddInlineItemWithSyntax("Id", dependency.Id);
									dependencyTable.AddInlineItemWithSyntax("Version", dependency.Version);
								}
							}

							if (targetFramework.Libraries.Count > 0)
							{
								var librariesArray = targetFrameworkTable.EnsureArrayWithSyntax("Libraries", 8);
								foreach (var library in targetFramework.Libraries)
								{
									librariesArray.AddItemWithSyntax(library, 9);
								}
							}
						}
					}
				}
			}
		}
	}

	private static async Task DiscoverLinuxPlatformAsync(bool includePrerelease, LocalUserConfig userConfig)
	{
		await DiscoverGCCAsync(userConfig);
		await DiscoverClangAsync(userConfig);
	}

	private static async Task DiscoverGCCAsync(LocalUserConfig userConfig)
	{
		// Find the GCC SDKs
		var cCompilerPath = await WhereIsUtilities.FindExecutableAsync("gcc");
		var cppCompilerPath = await WhereIsUtilities.FindExecutableAsync("g++");

		var gccSDK = userConfig.EnsureSDK("GCC");
		gccSDK.SourceDirectories.Clear();
		gccSDK.SetProperties(
			new Dictionary<string, string>()
			{
					{ "CCompiler", cCompilerPath.ToString() },
					{ "CppCompiler", cppCompilerPath.ToString() },
			});
	}

	private static async Task DiscoverClangAsync(LocalUserConfig userConfig)
	{
		// Find the GCC SDKs
		var cCompilerPath = await WhereIsUtilities.FindExecutableAsync("clang");
		var cppCompilerPath = await WhereIsUtilities.FindExecutableAsync("clang++");
		var archiverPath = await WhereIsUtilities.FindExecutableAsync("ar");

		var clangSDK = userConfig.EnsureSDK("Clang");
		clangSDK.SourceDirectories.Clear();
		clangSDK.SetProperties(
			new Dictionary<string, string>()
			{
					{ "CCompiler", cCompilerPath.ToString() },
					{ "CppCompiler", cppCompilerPath.ToString() },
					{ "Archiver", archiverPath.ToString() },
			});
	}
}