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
		void Execute()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/Recipe.toml"),
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
				Path("C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
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

			auto workingDirectory = Path("C:/WorkingDirectory/");
			uut.Execute(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 0>Load PackageLock: C:/WorkingDirectory/PackageLock.toml",
					"INFO: 0>PackageLock file does not exist.",
					"DIAG: 0>Load Recipe: C:/WorkingDirectory/Recipe.toml",
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Check outdated parameters file: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Check outdated read access file: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Check outdated write access file: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Check for previous operation invocation",
					"INFO: 1>Operation has no successful previous invocation",
					"HIGH: 1>Generate Phase: C++|MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/WorkingDirectory/",
					"DIAG: 1>C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>Allowed Write Access:",
					"DIAG: 1>C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Create Directory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Loading generate evaluate operation graph",
					"DIAG: 1>Loading previous operation graph",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No valid previous build graph found",
					"INFO: 1>Create Directory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Create Directory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/PackageLock.toml",
					"Exists: C:/WorkingDirectory/Recipe.toml",
					"OpenReadBinary: C:/WorkingDirectory/Recipe.toml",
					"Exists: C:/RootRecipe.toml",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"CreateDirectory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"Exists: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
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
					"CreateDetourProcess: 1 [C:/WorkingDirectory/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [5] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}
	};
}
