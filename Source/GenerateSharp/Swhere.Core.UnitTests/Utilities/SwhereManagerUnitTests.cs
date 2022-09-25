// <copyright file="SwhereManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Build.Discover.UnitTests
{
	[Collection("Opal")]
	public class SwhereManagerUnitTests
	{
		[Fact]
		public async Task Discover()
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

			mockFileSystem.RegisterChildren(
				new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/"),
				new List<DirectoryEntry>()
				{
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/5.0.0"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.7"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.8"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.9"), IsDirectory = true, },
				});

			mockFileSystem.RegisterChildren(
				new Path("C:/Program Files (x86)/Windows Kits/10/include/"),
				new List<DirectoryEntry>()
				{
					new DirectoryEntry() { Path = new Path("C:/Program Files (x86)/Windows Kits/10/include/10.0.19041.0"), IsDirectory = true, },
				});

			mockFileSystem.CreateMockFile(
				new Path("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes("14.33.31629\r\n"))));

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
					"GetChildDirectories: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
					"Exists: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"OpenRead: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"GetChildDirectories: C:/Program Files (x86)/Windows Kits/10/include/",
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
@"SDKs: [
	{
		Name: ""Roslyn""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/""
		]
		Properties: { ToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/"" }
	}
	{
		Name: ""DotNet""
		SourceDirectories: [
			""C:/Program Files/dotnet/""
		]
		Properties: { RuntimeVersion: ""6.0.9"", RootPath: ""C:/Program Files/dotnet/"" }
	}
	{
		Name: ""MSVC""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/""
		]
		Properties: { Version: ""14.33.31629"", VCToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/"" }
	}
	{
		Name: ""Windows""
		SourceDirectories: [
			""C:/Program Files (x86)/Windows Kits/10/""
		]
		Properties: { Version: ""10.0.19041.0"", RootPath: ""C:/Program Files (x86)/Windows Kits/10/"" }
	}
	{
		Name: ""NetFXTools""
		SourceDirectories: [
			""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/""
		]
		Properties: { ToolsRoot: ""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/"" }
	}
]";

			Assert.Equal(expected, localUserConfigContent);
		}

		[Fact]
		public async Task Discover_Prerelease()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			var mockProcessManager = new MockProcessManager();
			using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath -prerelease",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Preview\n");

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 2 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Preview\n");

			mockFileSystem.RegisterChildren(
				new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/"),
				new List<DirectoryEntry>()
				{
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/5.0.0"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.7"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.8"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.9"), IsDirectory = true, },
				});

			mockFileSystem.RegisterChildren(
				new Path("C:/Program Files (x86)/Windows Kits/10/include/"),
				new List<DirectoryEntry>()
				{
					new DirectoryEntry() { Path = new Path("C:/Program Files (x86)/Windows Kits/10/include/10.0.19041.0"), IsDirectory = true, },
				});

			mockFileSystem.CreateMockFile(
				new Path("C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes("14.34.31823\r\n"))));

			bool includePrerelease = true;
			await SwhereManager.DiscoverAsync(includePrerelease);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist.",
					"INFO: No existing local user config.",
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath -prerelease",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Preview",
					"HIGH: FindNewestDotNet6RuntimeVersion: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
					"INFO: CheckFile: 5.0.0",
					"INFO: CheckFile: 6.0.7",
					"INFO: CheckFile: 6.0.8",
					"INFO: CheckFile: 6.0.9",
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Preview",
					"HIGH: Using VC Version: 14.34.31823",
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
					"GetChildDirectories: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
					"Exists: C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"OpenRead: C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"GetChildDirectories: C:/Program Files (x86)/Windows Kits/10/include/",
					"Exists: C:/Users/Me/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/",
					"OpenWriteTruncate: C:/Users/Me/.soup/LocalUserConfig.sml",
				},
				mockFileSystem.GetRequests());

			// Verify expected process requests
			Assert.Equal(
				new List<string>()
				{
					"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath -prerelease",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateProcess: 2 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
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
@"SDKs: [
	{
		Name: ""Roslyn""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Preview/MSBuild/Current/Bin/Roslyn/""
		]
		Properties: { ToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Preview/MSBuild/Current/Bin/Roslyn/"" }
	}
	{
		Name: ""DotNet""
		SourceDirectories: [
			""C:/Program Files/dotnet/""
		]
		Properties: { RuntimeVersion: ""6.0.9"", RootPath: ""C:/Program Files/dotnet/"" }
	}
	{
		Name: ""MSVC""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Tools/MSVC/14.34.31823/""
		]
		Properties: { Version: ""14.34.31823"", VCToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Tools/MSVC/14.34.31823/"" }
	}
	{
		Name: ""Windows""
		SourceDirectories: [
			""C:/Program Files (x86)/Windows Kits/10/""
		]
		Properties: { Version: ""10.0.19041.0"", RootPath: ""C:/Program Files (x86)/Windows Kits/10/"" }
	}
	{
		Name: ""NetFXTools""
		SourceDirectories: [
			""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/""
		]
		Properties: { ToolsRoot: ""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/"" }
	}
]";

			Assert.Equal(expected, localUserConfigContent);
		}

		[Fact]
		public async Task Discover_UpdateExisting()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			var mockProcessManager = new MockProcessManager();
			using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);

			// Create the original file
			var original =
@"SDKs: [
	{
		Name: ""CustomSDK""
		SourceDirectories: [
			""C:/SomeWhere/""
		]
		Properties: { ToolsRoot: ""C:/SomeWhere/"" }
	}
	{
		Name: ""Roslyn""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Preview/MSBuild/Current/Bin/Roslyn/""
		]
		Properties: { ToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Preview/MSBuild/Current/Bin/Roslyn/"" }
	}
	{
		Name: ""DotNet""
		SourceDirectories: [
			""C:/Program Files/dotnet/""
		]
		Properties: { RuntimeVersion: ""6.0.9"", RootPath: ""C:/Program Files/dotnet/"" }
	}
	{
		Name: ""MSVC""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Tools/MSVC/14.34.31823/""
		]
		Properties: { Version: ""14.34.31823"", VCToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Tools/MSVC/14.34.31823/"" }
	}
	{
		Name: ""Windows""
		SourceDirectories: [
			""C:/Program Files (x86)/Windows Kits/10/""
		]
		Properties: { Version: ""10.0.19041.0"", RootPath: ""C:/Program Files (x86)/Windows Kits/10/"" }
	}
	{
		Name: ""NetFXTools""
		SourceDirectories: [
			""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/""
		]
		Properties: { ToolsRoot: ""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/"" }
	}
]
";
			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(original));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Users/Me/.soup/LocalUserConfig.sml"),
				new MockFile(originalContent));

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\n");

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 2 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\n");

			mockFileSystem.RegisterChildren(
				new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/"),
				new List<DirectoryEntry>()
				{
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/5.0.0"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.7"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.8"), IsDirectory = true, },
					new DirectoryEntry() { Path = new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.9"), IsDirectory = true, },
				});

			mockFileSystem.RegisterChildren(
				new Path("C:/Program Files (x86)/Windows Kits/10/include/"),
				new List<DirectoryEntry>()
				{
					new DirectoryEntry() { Path = new Path("C:/Program Files (x86)/Windows Kits/10/include/10.0.19041.0"), IsDirectory = true, },
				});

			mockFileSystem.CreateMockFile(
				new Path("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes("14.33.31629\r\n"))));

			bool includePrerelease = false;
			await SwhereManager.DiscoverAsync(includePrerelease);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
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
					"OpenRead: C:/Users/Me/.soup/LocalUserConfig.sml",
					"GetChildDirectories: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
					"Exists: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"OpenRead: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"GetChildDirectories: C:/Program Files (x86)/Windows Kits/10/include/",
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
@"SDKs: [
	{
		Name: ""CustomSDK""
		SourceDirectories: [
			""C:/SomeWhere/""
		]
		Properties: { ToolsRoot: ""C:/SomeWhere/"" }
	}
	{
		Name: ""Roslyn""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/""
		]
		Properties: { ToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/"" }
	}
	{
		Name: ""DotNet""
		SourceDirectories: [
			""C:/Program Files/dotnet/""
		]
		Properties: { RuntimeVersion: ""6.0.9"", RootPath: ""C:/Program Files/dotnet/"" }
	}
	{
		Name: ""MSVC""
		SourceDirectories: [
			""C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/""
		]
		Properties: { Version: ""14.33.31629"", VCToolsRoot: ""C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/"" }
	}
	{
		Name: ""Windows""
		SourceDirectories: [
			""C:/Program Files (x86)/Windows Kits/10/""
		]
		Properties: { Version: ""10.0.19041.0"", RootPath: ""C:/Program Files (x86)/Windows Kits/10/"" }
	}
	{
		Name: ""NetFXTools""
		SourceDirectories: [
			""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/""
		]
		Properties: { ToolsRoot: ""C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.8 Tools/"" }
	}
]
";

			Assert.Equal(expected, localUserConfigContent);
		}
	}
}
