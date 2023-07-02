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
			var (loadConfigResult, userConfig) = 
				await LocalUserConfigExtensions.TryLoadLocalUserConfigFromFileAsync(localUserConfigPath);
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

			var (hasNuget, nugetPackagesPath, nugetPackages) = await NugetSDKUtilities.FindNugetPackagesAsync();
			if (hasNuget)
			{
				var nugetSDK = userConfig.EnsureSDK("Nuget");
				nugetSDK.SourceDirectories = new List<Path>()
				{
					nugetPackagesPath,
				};

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

			// Save the result
			await LocalUserConfigExtensions.SaveToFileAsync(localUserConfigPath, userConfig);
		}
	}
}
