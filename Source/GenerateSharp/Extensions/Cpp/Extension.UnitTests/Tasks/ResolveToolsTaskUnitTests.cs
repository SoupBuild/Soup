// <copyright file="ResolveToolsTaskUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Text;
using Xunit;

namespace Soup.Build.Cpp.UnitTests
{
	public class ResolveToolsTaskUnitTests
	{
		[Fact]
		public void Initialize_Success()
		{
			var buildState = new MockBuildState();
			var uut = new ResolveToolsTask(buildState);
		}

		[Fact]
		public void Execute()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test file system
			var fileSystem = new MockFileSystem();

			// Setup expected output from vswhere call
			processManager.RegisterExecuteResult(
				"CreateProcess: 1 [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
				"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\r\n");

			// Setup the default version file
			fileSystem.CreateMockFile(
				new Path("C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				new MockFile(new System.IO.MemoryStream(Encoding.ASCII.GetBytes("1.2.3.4\n"))));

			// Register the test systems
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			using (var scopedFileSystem = new ScopedSingleton<IFileSystem>(fileSystem))
			{
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("System", new Value("win32"));
				parametersTable.Add("Architecture", new Value("x64"));

				var uut = new ResolveToolsTask(buildState);

				uut.Execute();

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"DIAG: C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath -prerelease",
						"INFO: Using VS Installation: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community",
						"INFO: Using VC Version: 14.28.29910",
						"DIAG: FindNewestWindows10KitVersion: C:/Program Files (x86)/Windows Kits/10/include/",
						"DIAG: CheckFile: 10.0.19041.0",
						"INFO: Using Windows Kit Version: 10.0.19041.0",
					},
					testListener.GetMessages());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>();

				Assert.Equal(
					expectedBuildOperations,
					buildState.GetBuildOperations());

				// Verify expected file system requests
				Assert.Equal(
					new List<string>()
					{},
					fileSystem.GetRequests());

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
					processManager.GetRequests());
			}
		}
	}
}
