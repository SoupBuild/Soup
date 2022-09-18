// <copyright file="SwhereManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Build.Discover.UnitTests
{
	public class SwhereManagerUnitTests
	{
		[Fact]
		public async Task CreateOperation_NoInputOrOutput()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			var mockProcessManager = new MockProcessManager();
			using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\n");

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 2 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\n");

			bool includePrerelease = false;
			await SwhereManager.DiscoverAsync(includePrerelease);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist.",
					"INFO: No existing local user config.",
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Community",
					"HIGH: FindNewestDotNet6RuntimeVersion: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
					"INFO: CheckFile: 5.0.0",
					"INFO: CheckFile: 6.0.7",
					"INFO: CheckFile: 6.0.8",
					"INFO: CheckFile: 6.0.9",
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Community",
					"HIGH: Using VC Version: 14.33.31629",
					"HIGH: FindNewestWindows10KitVersion: C:/Program Files (x86)/Windows Kits/10/",
					"INFO: CheckFile: 10.0.19041.0",
					"INFO: Creating directory C:/Users/Me/.soup/",

				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/LocalUserConfig.sml",
					"Exists: C:/Users/Me/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/",
					"OpenWriteTruncate: C:/Users/Me/.soup/LocalUserConfig.sml",
				},
				mockFileSystem.GetRequests());

			// Verify expected process requests
			Assert.Equal(
				new List<string>()
				{
					"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateProcess: 2 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
					"ProcessStart: 2",
					"WaitForExit: 2",
					"GetStandardOutput: 2",
					"GetStandardError: 2",
					"GetExitCode: 2",
				},
				mockProcessManager.GetRequests());

			// Verify the contents of the local user config file
			var localUserConfig = mockFileSystem.GetMockFile(new Path("C:/Users/Me/.soup/LocalUserConfig.sml"));
			localUserConfig.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(localUserConfig.Content);
			var localUserConfigContent = await reader.ReadToEndAsync();
			var expected =
@"SDKsNameRoslynSourceDirectoriesC:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/PropertiesToolsRootC:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/NameDotNetSourceDirectoriesC:/Program Files/dotnet/PropertiesRuntimeVersion6.0.9RootPathC:/Program Files/dotnet/NameMSVCSourceDirectoriesC:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/PropertiesVersion14.33.31629VCToolsRootC:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/NameWindowsSourceDirectoriesC:/Program Files (x86)/Windows Kits/10/PropertiesVersion10.0.19041.0RootPathC:/Program Files (x86)/Windows Kits/10/NameNetFXToolsSourceDirectoriesC:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/PropertiesToolsRootC:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/";

			Assert.Equal(expected, localUserConfigContent);
		}
	}
}
