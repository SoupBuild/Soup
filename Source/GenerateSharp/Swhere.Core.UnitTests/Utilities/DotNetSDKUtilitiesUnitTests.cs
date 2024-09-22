// <copyright file="DotNetSDKUtilitiesUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Xunit;
using Path = Opal.Path;

namespace Soup.Build.Discover.UnitTests;

[Collection("Opal")]
public class DotNetSDKUtilitiesUnitTests
{
	[Fact]
	public async Task FindDotNet_Windows()
	{
		// Register the test listener
		var testListener = new TestTraceListener();
		using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

		var mockFileSystem = new MockFileSystem();
		using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

		var mockProcessManager = new MockProcessManager();
		using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);
		mockProcessManager.RegisterExecuteResult(
			"CreateProcess: 1 [./] C:/Windows/System32/where.exe dotnet",
			"C:\\Program Files\\dotnet\\dotnet.exe\r\n");
		mockProcessManager.RegisterExecuteResult(
			"CreateProcess: 2 [./] C:/Program Files/dotnet/dotnet.exe --list-sdks",
			"5.0.0 [C:\\Program Files\\dotnet\\sdk]\r\n6.0.8 [C:\\Program Files\\dotnet\\sdk]\r\n7.0.201 [C:\\Program Files\\dotnet\\sdk]\r\n7.0.300-preview.23179.2 [C:\\Program Files\\dotnet\\sdk]\r\n7.0.304 [C:\\Program Files\\dotnet\\sdk]\r\n7.0.400-preview.23274.1 [C:\\Program Files\\dotnet\\sdk]\r\n");
		mockProcessManager.RegisterExecuteResult(
			"CreateProcess: 3 [./] C:/Program Files/dotnet/dotnet.exe --list-runtimes",
			"Microsoft.AspNetCore.App 3.1.32 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.AspNetCore.App 5.0.17 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.AspNetCore.App 6.0.14 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.AspNetCore.App 6.0.16 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.AspNetCore.App 6.0.18 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.AspNetCore.App 7.0.3 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.AspNetCore.App 7.0.5 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.AspNetCore.App 7.0.7 [C:\\Program Files\\dotnet\\shared\\Microsoft.AspNetCore.App]\r\nMicrosoft.NETCore.App 3.1.32 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 5.0.17 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 6.0.12 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 6.0.14 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 6.0.15 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 6.0.16 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 6.0.18 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 6.0.20 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 7.0.3 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 7.0.5 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.NETCore.App 7.0.7 [C:\\Program Files\\dotnet\\shared\\Microsoft.NETCore.App]\r\nMicrosoft.WindowsDesktop.App 3.1.32 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\nMicrosoft.WindowsDesktop.App 5.0.17 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\nMicrosoft.WindowsDesktop.App 6.0.14 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\nMicrosoft.WindowsDesktop.App 6.0.16 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\nMicrosoft.WindowsDesktop.App 6.0.18 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\nMicrosoft.WindowsDesktop.App 7.0.3 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\nMicrosoft.WindowsDesktop.App 7.0.5 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\nMicrosoft.WindowsDesktop.App 7.0.7 [C:\\Program Files\\dotnet\\shared\\Microsoft.WindowsDesktop.App]\r\n");

		mockFileSystem.RegisterChildren(
			new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/"),
			[
				new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/5.0.0/"), IsDirectory = true, },
				new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.7/"), IsDirectory = true, },
				new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.8/"), IsDirectory = true, },
				new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.9/"), IsDirectory = true, },
				new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/7.0.7/"), IsDirectory = true, },
			]);

		var platform = OSPlatform.Windows;
		var result = await DotNetSDKUtilities.FindDotNetAsync(platform);

		Assert.Equal(new Path("C:/Program Files/dotnet/dotnet.exe"), result.DotNetExecutable);
		Assert.Equal(
			[
				("5.0.0", new Path("C:/Program Files/dotnet/sdk/")),
				("6.0.8", new Path("C:/Program Files/dotnet/sdk/")),
				("7.0.201", new Path("C:/Program Files/dotnet/sdk/")),
				("7.0.300-preview.23179.2", new Path("C:/Program Files/dotnet/sdk/")),
				("7.0.304", new Path("C:/Program Files/dotnet/sdk/")),
				("7.0.400-preview.23274.1", new Path("C:/Program Files/dotnet/sdk/")),
			],
			result.SDKVersions);
		Assert.Equal(
			new Dictionary<string, IList<(string Version, Path InstallDirectory)>>()
			{
				{
					"Microsoft.AspNetCore.App",
					new List<(string Version, Path InstallDirectory)>()
					{
						("3.1.32", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
						("5.0.17", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
						("6.0.14", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
						("6.0.16", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
						("6.0.18", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
						("7.0.3", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
						("7.0.5", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
						("7.0.7", new Path("C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/")),
					}
				},
				{
					"Microsoft.NETCore.App",
					new List<(string Version, Path InstallDirectory)>()
					{
						("3.1.32", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("5.0.17", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("6.0.12", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("6.0.14", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("6.0.15", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("6.0.16", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("6.0.18", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("6.0.20", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("7.0.3", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("7.0.5", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
						("7.0.7", new Path("C:/Program Files/dotnet/shared/Microsoft.NETCore.App/")),
					}
				},
				{
					"Microsoft.WindowsDesktop.App",
					new List<(string Version, Path InstallDirectory)>()
					{
						("3.1.32", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
						("5.0.17", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
						("6.0.14", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
						("6.0.16", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
						("6.0.18", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
						("7.0.3", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
						("7.0.5", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
						("7.0.7", new Path("C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/")),
					}
				},
			},
			result.Runtimes,
			new DictionaryOfListsComparer<(string Version, Path InstallDirectory)>());

		Assert.Equal(
			new Dictionary<string, IList<(string Version, Path InstallDirectory)>>()
			{
				{
					"Microsoft.NETCore.App.Ref",
					new List<(string Version, Path InstallDirectory)>()
					{
						("5.0.0", new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/")),
						("6.0.7", new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/")),
						("6.0.8", new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/")),
						("6.0.9", new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/")),
						("7.0.7", new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/")),

					}
				},
			},
			result.TargetingPacks,
			new DictionaryOfListsComparer<(string Version, Path InstallDirectory)>());

		// Verify expected logs
		Assert.Equal(
			[
				"INFO: C:/Windows/System32/where.exe dotnet",
				"HIGH: Using DotNet: C:/Program Files/dotnet/dotnet.exe",
				"HIGH: Find DotNet SDK Versions",
				"INFO: C:/Program Files/dotnet/dotnet.exe --list-sdks",
				"INFO: Found SDK: 5.0.0 C:/Program Files/dotnet/sdk/",
				"INFO: Found SDK: 6.0.8 C:/Program Files/dotnet/sdk/",
				"INFO: Found SDK: 7.0.201 C:/Program Files/dotnet/sdk/",
				"INFO: Found SDK: 7.0.300-preview.23179.2 C:/Program Files/dotnet/sdk/",
				"INFO: Found SDK: 7.0.304 C:/Program Files/dotnet/sdk/",
				"INFO: Found SDK: 7.0.400-preview.23274.1 C:/Program Files/dotnet/sdk/",
				"HIGH: Find DotNet Runtime Versions",
				"INFO: C:/Program Files/dotnet/dotnet.exe --list-runtimes",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 3.1.32 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 5.0.17 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 6.0.14 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 6.0.16 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 6.0.18 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 7.0.3 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 7.0.5 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.AspNetCore.App 7.0.7 C:/Program Files/dotnet/shared/Microsoft.AspNetCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 3.1.32 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 5.0.17 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 6.0.12 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 6.0.14 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 6.0.15 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 6.0.16 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 6.0.18 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 6.0.20 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 7.0.3 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 7.0.5 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.NETCore.App 7.0.7 C:/Program Files/dotnet/shared/Microsoft.NETCore.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 3.1.32 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 5.0.17 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 6.0.14 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 6.0.16 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 6.0.18 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 7.0.3 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 7.0.5 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"INFO: Found Runtime: Microsoft.WindowsDesktop.App 7.0.7 C:/Program Files/dotnet/shared/Microsoft.WindowsDesktop.App/",
				"HIGH: FindDotNetPackVersions: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"Exists: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
				"GetChildDirectories: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
			],
			mockFileSystem.Requests);

		// Verify expected process requests
		Assert.Equal(
			[
				"CreateProcess: 1 [./] C:/Windows/System32/where.exe dotnet",
				"ProcessStart: 1",
				"WaitForExit: 1",
				"GetStandardOutput: 1",
				"GetStandardError: 1",
				"GetExitCode: 1",
				"CreateProcess: 2 [./] C:/Program Files/dotnet/dotnet.exe --list-sdks",
				"ProcessStart: 2",
				"WaitForExit: 2",
				"GetStandardOutput: 2",
				"GetStandardError: 2",
				"GetExitCode: 2",
				"CreateProcess: 3 [./] C:/Program Files/dotnet/dotnet.exe --list-runtimes",
				"ProcessStart: 3",
				"WaitForExit: 3",
				"GetStandardOutput: 3",
				"GetStandardError: 3",
				"GetExitCode: 3",
			],
			mockProcessManager.Requests);
	}
}