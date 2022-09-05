// <copyright file="RecipeBuildRunnerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class RecipeBuildRunnerTests
	{
	public:
		// [[Fact]]
		void Initialize_Success()
		{
			auto arguments = RecipeBuildArguments();
			auto localUserConfig = LocalUserConfig();
			auto uut = RecipeBuildRunner(arguments, localUserConfig);
		}

		// [[Fact]]
		void Execute_NoDependencies()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++|1"
				)")));

			auto operationGraph = OperationGraph(
				{},
				std::vector<OperationId>({
				}),
				std::vector<OperationInfo>({
				}));
			auto operationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(operationGraph, operationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(operationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto arguments = RecipeBuildArguments();
			auto localUserConfig = LocalUserConfig();
			auto uut = RecipeBuildRunner(arguments, localUserConfig);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			uut.Execute(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 0>Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.toml",
					"INFO: 0>PackageLock file does not exist.",
					"DIAG: 0>Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Check outdated parameters file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Check outdated read access file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Check outdated write access file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Check for previous operation invocation",
					"INFO: 1>Operation has no successful previous invocation",
					"HIGH: 1>Generate Phase: C++|MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>Allowed Write Access:",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Loading generate evaluate operation graph",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.toml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"Exists: C:/WorkingDirectory/RootRecipe.toml",
					"Exists: C:/RootRecipe.toml",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [5] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}

		// [[Fact]]
		void Execute_BuildDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++|1"
					[Dependencies]
					Build = [
						"TestBuild@1.2.3",
					]
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "TestBuild"
					Language = "C#|1"
				)")));

			auto myProjectOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>({
				}),
				std::vector<OperationInfo>({
				}));
			auto myProjectOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myProjectOperationGraph, myProjectOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(myProjectOperationGraphContent)));

			auto testBuildOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>({
				}),
				std::vector<OperationInfo>({
				}));
			auto testBuildOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(testBuildOperationGraph, testBuildOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(testBuildOperationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto arguments = RecipeBuildArguments();
			auto localUserConfig = LocalUserConfig();
			auto uut = RecipeBuildRunner(arguments, localUserConfig);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			uut.Execute(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 0>Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.toml",
					"INFO: 0>PackageLock file does not exist.",
					"DIAG: 0>Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"DIAG: 0>Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.toml",
					"DIAG: 1>Running Build: C#|TestBuild",
					"INFO: 1>Host Build 'TestBuild'",
					"INFO: 1>Check outdated parameters file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Check outdated read access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Check outdated write access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Check for previous operation invocation",
					"INFO: 1>Operation has no successful previous invocation",
					"HIGH: 1>Generate Phase: C#|TestBuild",
					"DIAG: 1>Execute: [C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 1>Allowed Write Access:",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Loading generate evaluate operation graph",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Done",
					"DIAG: 2>Running Build: C++|MyPackage",
					"INFO: 2>Build 'MyPackage'",
					"INFO: 2>Check outdated parameters file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Check outdated read access file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Check outdated write access file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Check for previous operation invocation",
					"INFO: 2>Operation has no successful previous invocation",
					"HIGH: 2>Generate Phase: C++|MyPackage",
					"DIAG: 2>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 2>Allowed Read Access:",
					"DIAG: 2>C:/testlocation/Generate/",
					"DIAG: 2>C:/Windows/",
					"DIAG: 2>C:/Program Files/dotnet/",
					"DIAG: 2>C:/WorkingDirectory/MyPackage/",
					"DIAG: 2>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 2>Allowed Write Access:",
					"DIAG: 2>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Loading generate evaluate operation graph",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.toml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.toml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/packages/C#/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/RootRecipe.toml",
					"Exists: C:/Users/Me/RootRecipe.toml",
					"Exists: C:/Users/RootRecipe.toml",
					"Exists: C:/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/EvaluateResultGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/EvaluateResultGraph.bog",
					"Exists: C:/WorkingDirectory/RootRecipe.toml",
					"Exists: C:/RootRecipe.toml",
					"GetCurrentDirectory",
					"GetCurrentDirectory",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/ Environment [2] 1 AllowedRead [5] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateDetourProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [6] AllowedWrite [1]",
					"ProcessStart: 2",
					"WaitForExit: 2",
					"GetStandardOutput: 2",
					"GetStandardError: 2",
					"GetExitCode: 2",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}
		
		// [[Fact]]
		void Execute_PackageLock_OverrideBuildDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "MyPackage"
					Language = "C++|1"
					[Dependencies]
					Build = [
						"TestBuild@1.2.3",
					]
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name = "TestBuild"
					Language = "C#|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version = 2
					[Closures]
					[Closures.Root]
					"C#" = [
						{ Name = "TestBuild", Version = "1.3.0", Build = "Build1" },
					]
					"C++" = [
						{ Name = "MyPackage", Version = "../MyPackage/", Build = "Build0" },
					]
					[Closures.Build0]
					"C#" = [
						{ Name = "C++", Version = "1.0.2" },
						{ Name = "TestBuild", Version = "1.3.0" },
					]
					[Closures.Build1]
					"C#" = [
						{ Name = "C#", Version = "1.0.1" },
					]
				)")));

			auto myProjectOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>({
				}),
				std::vector<OperationInfo>({
				}));
			auto myProjectOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myProjectOperationGraph, myProjectOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(myProjectOperationGraphContent)));

			auto testBuildOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>({
				}),
				std::vector<OperationInfo>({
				}));
			auto testBuildOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(testBuildOperationGraph, testBuildOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(testBuildOperationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto arguments = RecipeBuildArguments();
			auto localUserConfig = LocalUserConfig();
			auto uut = RecipeBuildRunner(arguments, localUserConfig);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			uut.Execute(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 0>Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.toml",
					"INFO: 0>Package lock loaded",
					"DIAG: 0>Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"DIAG: 0>Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.toml",
					"DIAG: 1>Running Build: C#|TestBuild",
					"INFO: 1>Host Build 'TestBuild'",
					"INFO: 1>Check outdated parameters file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Check outdated read access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Check outdated write access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Check for previous operation invocation",
					"INFO: 1>Operation has no successful previous invocation",
					"HIGH: 1>Generate Phase: C#|TestBuild",
					"DIAG: 1>Execute: [C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 1>Allowed Write Access:",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Loading generate evaluate operation graph",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 1>Done",
					"DIAG: 2>Running Build: C++|MyPackage",
					"INFO: 2>Build 'MyPackage'",
					"INFO: 2>Check outdated parameters file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Check outdated read access file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Check outdated write access file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Check for previous operation invocation",
					"INFO: 2>Operation has no successful previous invocation",
					"HIGH: 2>Generate Phase: C++|MyPackage",
					"DIAG: 2>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 2>Allowed Read Access:",
					"DIAG: 2>C:/testlocation/Generate/",
					"DIAG: 2>C:/Windows/",
					"DIAG: 2>C:/Program Files/dotnet/",
					"DIAG: 2>C:/WorkingDirectory/MyPackage/",
					"DIAG: 2>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 2>Allowed Write Access:",
					"DIAG: 2>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Loading generate evaluate operation graph",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.toml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.toml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.toml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.toml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/packages/C#/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/RootRecipe.toml",
					"Exists: C:/Users/Me/RootRecipe.toml",
					"Exists: C:/Users/RootRecipe.toml",
					"Exists: C:/RootRecipe.toml",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/EvaluateResultGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/EvaluateResultGraph.bog",
					"Exists: C:/WorkingDirectory/RootRecipe.toml",
					"Exists: C:/RootRecipe.toml",
					"GetCurrentDirectory",
					"GetCurrentDirectory",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/ Environment [2] 1 AllowedRead [5] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateDetourProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [6] AllowedWrite [1]",
					"ProcessStart: 2",
					"WaitForExit: 2",
					"GetStandardOutput: 2",
					"GetStandardError: 2",
					"GetExitCode: 2",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}
	};
}
