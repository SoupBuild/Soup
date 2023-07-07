// <copyright file="SwhereManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Discover
{
	using System.Collections.Generic;
	using System.Runtime.InteropServices;
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
			var (loadConfigResult, userConfig) =
				await LocalUserConfigExtensions.TryLoadLocalUserConfigFromFileAsync(localUserConfigPath);
			if (!loadConfigResult)
			{
				Log.Info("No existing local user config.");
			}

			await DiscoverSharedPlatformAsync(includePrerelease, userConfig);
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

		private static async Task DiscoverSharedPlatformAsync(bool includePrerelease, LocalUserConfig userConfig)
		{
			var (dotNetExecutable, dotnetSDKs, dotnetRuntimes) = await DotNetSDKUtilities.FindDotNetAsync();
			var dotnetSDK = userConfig.EnsureSDK("DotNet");
			dotnetSDK.SourceDirectories = new List<Path>()
			{
				dotNetExecutable.GetParent(),
			};
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
		}

		private static async Task DiscoverWindowsPlatformAsync(bool includePrerelease, LocalUserConfig userConfig)
		{
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

			var (hasNuget, nugetPackagesPath, nugetPackages) = await NugetSDKUtilities.FindNugetPackagesAsync();
			if (hasNuget)
			{
				var nugetSDK = userConfig.EnsureSDK("Nuget");
				nugetSDK.SourceDirectories = new List<Path>()
				{
					nugetPackagesPath,
				};

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
			// Find the GCC SDKs
			var gccInstallPath = await WhereIsUtilities.FindExecutableAsync("gcc");

			var gccSDK = userConfig.EnsureSDK("GCC");
			gccSDK.SourceDirectories = new List<Path>()
			{
				gccInstallPath,
			};
			gccSDK.SetProperties(
				new Dictionary<string, string>()
				{
					{ "ToolsRoot", gccInstallPath.ToString() },
				});
		}
	}
}
