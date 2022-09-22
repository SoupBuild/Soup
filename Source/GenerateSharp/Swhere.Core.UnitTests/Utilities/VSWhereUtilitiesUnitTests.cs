// <copyright file="VSWhereUtilitiesUnitTests.cs" company="Soup">
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
	public class VSWhereUtilitiesUnitTests
	{
		[Fact]
		public async Task FindRoslynInstallAsync()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockProcessManager = new MockProcessManager();
			using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\n");

			bool includePrerelease = false;
			var result = await VSWhereUtilities.FindRoslynInstallAsync(includePrerelease);

			Assert.Equal(new Path("C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/Roslyn/"), result);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Community",
				},
				testListener.GetMessages());

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
				},
				mockProcessManager.GetRequests());
		}

		[Fact]
		public async Task FindRoslynInstallAsync_Prerelease()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockProcessManager = new MockProcessManager();
			using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath -prerelease",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Preview\n");

			bool includePrerelease = true;
			var result = await VSWhereUtilities.FindRoslynInstallAsync(includePrerelease);

			Assert.Equal(new Path("C:/Program Files/Microsoft Visual Studio/2022/Preview/MSBuild/Current/Bin/Roslyn/"), result);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.Roslyn.Compiler -property installationPath -prerelease",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Preview",
				},
				testListener.GetMessages());

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
				},
				mockProcessManager.GetRequests());
		}

		[Fact]
		public async Task FindMSVCInstallAsync()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockProcessManager = new MockProcessManager();
			using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);

			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\n");

			mockFileSystem.CreateMockFile(
				new Path("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes("14.33.31629\r\n"))));

			bool includePrerelease = false;
			var result = await VSWhereUtilities.FindMSVCInstallAsync(includePrerelease);

			Assert.Equal("14.33.31629", result.Version);
			Assert.Equal(new Path("C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/"), result.Path);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Community",
					"HIGH: Using VC Version: 14.33.31629",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"OpenRead: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
				},
				mockFileSystem.GetRequests());

			// Verify expected process requests
			Assert.Equal(
				new List<string>()
				{
					"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				},
				mockProcessManager.GetRequests());
		}

		[Fact]
		public async Task FindMSVCInstallAsync_Prerelease()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			var mockProcessManager = new MockProcessManager();
			using var scopedProcessManager = new ScopedSingleton<IProcessManager>(mockProcessManager);

			mockProcessManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
				"C:\\Program Files\\Microsoft Visual Studio\\2022\\Preview\n");

			mockFileSystem.CreateMockFile(
				new Path("C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes("14.34.31823\r\n"))));

			bool includePrerelease = true;
			var result = await VSWhereUtilities.FindMSVCInstallAsync(includePrerelease);

			Assert.Equal("14.34.31823", result.Version);
			Assert.Equal(new Path("C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Tools/MSVC/14.34.31823/"), result.Path);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
					"HIGH: Using VS Installation: C:/Program Files/Microsoft Visual Studio/2022/Preview",
					"HIGH: Using VC Version: 14.34.31823",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"OpenRead: C:/Program Files/Microsoft Visual Studio/2022/Preview/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
				},
				mockFileSystem.GetRequests());

			// Verify expected process requests
			Assert.Equal(
				new List<string>()
				{
					"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				},
				mockProcessManager.GetRequests());
		}
	}
}
