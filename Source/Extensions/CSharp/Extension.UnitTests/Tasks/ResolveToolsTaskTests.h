// <copyright file="ResolveToolsTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::UnitTests
{
	class ResolveToolsTaskTests
	{
	public:
		// [[Fact]]
		void Initialize_Success()
		{
			auto uut = ResolveToolsTask();
			
			Assert::AreEqual("ResolveTools", uut.GetName(), "Verify name matches expected.");
		}

		// [[Fact]]
		void Execute()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = Opal::ScopedTraceListenerRegister(testListener);

			// Setup expected output from vswhere call
			processManager->RegisterExecuteResult(
				"Execute: [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
				"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\r\n");

			// Setup the default version file
			fileSystem->CreateMockFile(
				Path("C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				std::make_shared<MockFile>(std::stringstream("1.2.3.4\n")));

			auto uut = ResolveToolsTask();

			// Setup the input build state
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Using VS Installation: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community",
					"INFO: Using VC Version: 1.2.3.4",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>();

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
					"OpenRead: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Execute: [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}
	};
}
