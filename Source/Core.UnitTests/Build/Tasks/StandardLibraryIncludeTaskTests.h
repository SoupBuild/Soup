// <copyright file="StandardLibraryIncludeTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class StandardLibraryIncludeTaskTests
	{
	public:
		[[Fact]]
		void Initialze_Success()
		{
			auto uut = StandardLibraryIncludeTask();
			
			Assert::AreEqual("StandardLibraryInclude", uut.GetName(), "Verify name matches expected.");
		}

		[[Fact]]
		void Execute()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Setup expected output from vswhere call
			processManager->RegisterExecuteResult(
				"Execute: [./] C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath",
				"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\r\n");

			// Setup the default version file
			fileSystem->CreateMockFile(
				Path("C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt"),
				MockFileState(std::stringstream("1.2.3.4\n")));

			auto uut = StandardLibraryIncludeTask();

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Using VS Installation: C:/Program Files (x86)/Microsoft Visual Studio/2019/Community",
					"INFO: Using VC Version: 1.2.3.4",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::None;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});

			// Verify build state
			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>();
			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());

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
