// <copyright file="BuildEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class BuildEngineTests
	{
	public:
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
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
				)")));

			auto operationGraph = OperationGraph(
				{},
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
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
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			BuildEngine::Execute(std::move(arguments));

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist.",
					"WARN: Local User Config invalid.",
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist.",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
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
					"HIGH: 1>Generate: C++|MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/testlocation/Extensions/Soup.Cpp/0.4.0/",
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
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/LocalUserConfig.sml",
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
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
					"CreateDetourProcess: 1 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [6] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");

			// Verify files
			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable()), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.Cpp/0.4.0/Soup.Cpp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog"));
			auto myPackageGenerateGraph = OperationGraphReader::Deserialize(myPackageGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : myPackageGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{
					},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C++|MyPackage",
							CommandInfo(
								Path("C:/WorkingDirectory/MyPackage/"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				myPackageGenerateGraph,
				"Verify file content match expected.");
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
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Version: "1.0.0"
					Dependencies: {
						Build: [
							"TestBuild@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "C#|1"
					Version: "1.2.3"
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
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			BuildEngine::Execute(std::move(arguments));

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist.",
					"WARN: Local User Config invalid.",
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist.",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"DIAG: 2>Running Build: C#|TestBuild",
					"INFO: 2>Build 'TestBuild'",
					"INFO: 2>Check outdated parameters file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Check outdated read access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Check outdated write access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Check for previous operation invocation",
					"INFO: 2>Operation has no successful previous invocation",
					"HIGH: 2>Generate: C#|TestBuild",
					"DIAG: 2>Execute: [C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 2>Allowed Read Access:",
					"DIAG: 2>C:/testlocation/Generate/",
					"DIAG: 2>C:/testlocation/Extensions/Soup.CSharp/0.7.0/",
					"DIAG: 2>C:/Windows/",
					"DIAG: 2>C:/Program Files/dotnet/",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 2>Allowed Write Access:",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Loading generate evaluate operation graph",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Done",
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
					"HIGH: 1>Generate: C++|MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/testlocation/Extensions/Soup.Cpp/0.4.0/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
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
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/LocalUserConfig.sml",
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
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
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
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
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/ Environment [2] 1 AllowedRead [6] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateDetourProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]",
					"ProcessStart: 2",
					"WaitForExit: 2",
					"GetStandardOutput: 2",
					"GetStandardError: 2",
					"GetExitCode: 2",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");

			// Verify files
			auto testBuildGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Architecture", Value(std::string("x64")), },
					{ "Compiler", Value(std::string("MSVC")), },
					{ "Dependencies", Value(ValueTable()), },
					{ "Flavor", Value(std::string("release")), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.CSharp/0.7.0/Soup.CSharp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/")), },
					{ "System", Value(std::string("win32")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/")), },
				})),
				ValueTableReader::Deserialize(testBuildGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto testBuildGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3\nC:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/\n",
				testBuildGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/\n",
				testBuildGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateGraph.bog"));
			auto testBuildGenerateGraph = OperationGraphReader::Deserialize(testBuildGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : testBuildGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{
					},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C#|TestBuild",
							CommandInfo(
								Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				testBuildGenerateGraph,
				"Verify file content match expected.");

			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Build", Value(ValueTable(std::map<std::string, Value>({
							{ "TestBuild", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("TestBuild@1.2.3")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.Cpp/0.4.0/Soup.Cpp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/\nC:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog"));
			auto myPackageGenerateGraph = OperationGraphReader::Deserialize(myPackageGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : myPackageGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{
					},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C++|MyPackage",
							CommandInfo(
								Path("C:/WorkingDirectory/MyPackage/"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				myPackageGenerateGraph,
				"Verify file content match expected.");
		}

		// [[Fact]]
		void Execute_TriangleDependency_NoRebuild()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Runtime: [
							"PackageA@1.2.3"
							"PackageB@1.1.1"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageA"
					Language: "C++|1"
					Version: "1.2.3"
					Dependencies: {
						Runtime: [
							"PackageB@1.1.1"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageB"
					Language: "C++|1"
					Version: "1.1.1"
				)")));

			auto myProjectOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({}));
			auto myProjectOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myProjectOperationGraph, myProjectOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(myProjectOperationGraphContent)));
			auto packageAOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({}));
			auto packageAOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(packageAOperationGraph, packageAOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(packageAOperationGraphContent)));
			auto packageBOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({}));
			auto packageBOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(packageBOperationGraph, packageBOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(packageBOperationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto arguments = RecipeBuildArguments();
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			BuildEngine::Execute(std::move(arguments));

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist.",
					"WARN: Local User Config invalid.",
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist.",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"DIAG: Recipe closure already loaded: C++|PackageB",
					"DIAG: 3>Running Build: C++|PackageB",
					"INFO: 3>Build 'PackageB'",
					"INFO: 3>Check outdated parameters file: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",     
					"INFO: 3>Value Table file does not exist",
					"INFO: 3>Save Parameters file",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 3>Check outdated read access file: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",    
					"INFO: 3>Path list file does not exist",
					"INFO: 3>Save Read Access file",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 3>Check outdated write access file: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",  
					"INFO: 3>Path list file does not exist",
					"INFO: 3>Save Write Access file",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 3>Loading previous operation graph",
					"INFO: 3>Operation graph file does not exist",
					"INFO: 3>No valid previous build graph found",
					"DIAG: 3>Build evaluation start",
					"DIAG: 3>Check for previous operation invocation",
					"INFO: 3>Operation has no successful previous invocation",
					"HIGH: 3>Generate: C++|PackageB",
					"DIAG: 3>Execute: [C:/Users/Me/.soup/packages/C++/PackageB/1.1.1] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 3>Allowed Read Access:",
					"DIAG: 3>C:/testlocation/Generate/",
					"DIAG: 3>C:/testlocation/Extensions/Soup.Cpp/0.4.0/",
					"DIAG: 3>C:/Windows/",
					"DIAG: 3>C:/Program Files/dotnet/",
					"DIAG: 3>C:/Users/Me/.soup/packages/C++/PackageB/1.1.1",
					"DIAG: 3>C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 3>Allowed Write Access:",
					"DIAG: 3>C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 3>Build evaluation end",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 3>Loading generate evaluate operation graph",
					"DIAG: 3>Loading previous operation graph",
					"INFO: 3>Operation graph file does not exist",
					"INFO: 3>No valid previous build graph found",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"DIAG: 3>Build evaluation start",
					"DIAG: 3>Build evaluation end",
					"INFO: 3>Saving updated build state",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 3>Done",
					"DIAG: 2>Running Build: C++|PackageA",
					"INFO: 2>Build 'PackageA'",
					"INFO: 2>Check outdated parameters file: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",     
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Check outdated read access file: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",    
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Check outdated write access file: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",  
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Check for previous operation invocation",
					"INFO: 2>Operation has no successful previous invocation",
					"HIGH: 2>Generate: C++|PackageA",
					"DIAG: 2>Execute: [C:/Users/Me/.soup/packages/C++/PackageA/1.2.3] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 2>Allowed Read Access:",
					"DIAG: 2>C:/testlocation/Generate/",
					"DIAG: 2>C:/testlocation/Extensions/Soup.Cpp/0.4.0/",
					"DIAG: 2>C:/Windows/",
					"DIAG: 2>C:/Program Files/dotnet/",
					"DIAG: 2>C:/Users/Me/.soup/packages/C++/PackageA/1.2.3",
					"DIAG: 2>C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 2>C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 2>Allowed Write Access:",
					"DIAG: 2>C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Loading generate evaluate operation graph",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 2>Done",
					"DIAG: 3>Running Build: C++|PackageB",
					"DIAG: 3>Recipe already built: C++|PackageB",
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
					"HIGH: 1>Generate: C++|MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/testlocation/Extensions/Soup.Cpp/0.4.0/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
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
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/LocalUserConfig.sml",
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/EvaluateResultGraph.bog",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
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
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/Users/Me/.soup/packages/C++/PackageB/1.1.1] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [6] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateDetourProcess: 2 [C:/Users/Me/.soup/packages/C++/PackageA/1.2.3] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]",
					"ProcessStart: 2",
					"WaitForExit: 2",
					"GetStandardOutput: 2",
					"GetStandardError: 2",
					"GetExitCode: 2",
					"CreateDetourProcess: 3 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [8] AllowedWrite [1]",
					"ProcessStart: 3",
					"WaitForExit: 3",
					"GetStandardOutput: 3",
					"GetStandardError: 3",
					"GetExitCode: 3",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");

			// Verify files
			auto packageAGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Runtime", Value(ValueTable(std::map<std::string, Value>({
							{ "PackageB", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("PackageB@1.1.1")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.Cpp/0.4.0/Soup.Cpp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
				})),
				ValueTableReader::Deserialize(packageAGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto packageAGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\nC:/Users/Me/.soup/packages/C++/PackageA/1.2.3\nC:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				packageAGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageAGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				packageAGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageAGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog"));
			auto packageAGenerateGraph = OperationGraphReader::Deserialize(packageAGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : packageAGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{
					},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C++|PackageA",
							CommandInfo(
								Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				packageAGenerateGraph,
				"Verify file content match expected.");

			auto packageBGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable()), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.Cpp/0.4.0/Soup.Cpp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
				})),
				ValueTableReader::Deserialize(packageBGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto packageBGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C++/PackageB/1.1.1\nC:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				packageBGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageBGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				packageBGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageBGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog"));
			auto packageBGenerateGraph = OperationGraphReader::Deserialize(packageBGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : packageBGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{
					},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C++|PackageB",
							CommandInfo(
								Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				packageBGenerateGraph,
				"Verify file content match expected.");

			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Runtime", Value(ValueTable(std::map<std::string, Value>({
							{ "PackageA", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("PackageA@1.2.3")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
							}))), },
							{ "PackageB", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("PackageB@1.1.1")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.Cpp/0.4.0/Soup.Cpp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\nC:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\nC:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog"));
			auto myPackageGenerateGraph = OperationGraphReader::Deserialize(myPackageGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : myPackageGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{
					},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C++|MyPackage",
							CommandInfo(
								Path("C:/WorkingDirectory/MyPackage/"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				myPackageGenerateGraph,
				"Verify file content match expected.");
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
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Version: "1.0.0"
					Dependencies: {
						Build: [
							"TestBuild@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "C#|1"
					Version: "1.3.0"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 2
					Closures: {
						Root: {
							C#: [
								{ Name: "TestBuild", Version: "1.3.0", Build: "Build1" }
							]
							C++: [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
						}
						Build0: {
							C#: [
								{ Name: "C++", Version: "1.0.2" }
								{ Name: "TestBuild", Version: "1.3.0" }
							]
						}
						Build1: {
							C#: [
								{ Name: "C#", Version: "1.0.1" }
							]
						}
					}
				)")));

			auto myProjectOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto myProjectOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myProjectOperationGraph, myProjectOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateEvaluateGraph.bog"),
				std::make_shared<MockFile>(std::move(myProjectOperationGraphContent)));

			auto testBuildOperationGraph = OperationGraph(
				{},
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
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
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			BuildEngine::Execute(std::move(arguments));

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist.",
					"WARN: Local User Config invalid.",
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					"DIAG: 2>Running Build: C#|TestBuild",
					"INFO: 2>Build 'TestBuild'",
					"INFO: 2>Check outdated parameters file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Check outdated read access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Check outdated write access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Check for previous operation invocation",
					"INFO: 2>Operation has no successful previous invocation",
					"HIGH: 2>Generate: C#|TestBuild",
					"DIAG: 2>Execute: [C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"DIAG: 2>Allowed Read Access:",
					"DIAG: 2>C:/testlocation/Generate/",
					"DIAG: 2>C:/testlocation/Extensions/Soup.CSharp/0.7.0/",
					"DIAG: 2>C:/Windows/",
					"DIAG: 2>C:/Program Files/dotnet/",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 2>Allowed Write Access:",
					"DIAG: 2>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Loading generate evaluate operation graph",
					"DIAG: 2>Loading previous operation graph",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No valid previous build graph found",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/temp/",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/",
					"INFO: 2>Done",
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
					"HIGH: 1>Generate: C++|MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/Generate/",
					"DIAG: 1>C:/testlocation/Extensions/Soup.Cpp/0.4.0/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/",
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
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/LocalUserConfig.sml",
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
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
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
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
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0] C:/testlocation/Generate/Soup.Build.Generate.exe C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/ Environment [2] 1 AllowedRead [6] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateDetourProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Generate/Soup.Build.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]",
					"ProcessStart: 2",
					"WaitForExit: 2",
					"GetStandardOutput: 2",
					"GetStandardError: 2",
					"GetExitCode: 2",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");

			// Verify files
			auto testBuildGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Architecture", Value(std::string("x64")), },
					{ "Compiler", Value(std::string("MSVC")), },
					{ "Dependencies", Value(ValueTable()), },
					{ "Flavor", Value(std::string("release")), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.CSharp/0.7.0/Soup.CSharp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/")), },
					{ "System", Value(std::string("win32")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/")), },
				})),
				ValueTableReader::Deserialize(testBuildGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto testBuildGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0\nC:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/\n",
				testBuildGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/\n",
				testBuildGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/GenerateGraph.bog"));
			auto testBuildGenerateGraph = OperationGraphReader::Deserialize(testBuildGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : testBuildGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{
					},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C#|TestBuild",
							CommandInfo(
								Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				testBuildGenerateGraph,
				"Verify file content match expected.");

			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Build", Value(ValueTable(std::map<std::string, Value>({
							{ "TestBuild", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("TestBuild@1.2.3")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.Cpp/0.4.0/Soup.Cpp.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/txTMowfPh1V3rPmbvNBmBW9Z8Jg/\nC:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateGraphMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateGraph.bog"));
			auto myPackageGenerateGraph = OperationGraphReader::Deserialize(myPackageGenerateGraphMockFile->Content);

			// TODO: Clear time for now until mocked
			for (auto& operation : myPackageGenerateGraph.GetOperations())
				operation.second.EvaluateTime = std::chrono::time_point<std::chrono::system_clock>::min();

			Assert::AreEqual(
				OperationGraph(
					{},
					std::vector<OperationId>({
						1,
					}),
					std::vector<OperationInfo>({
						OperationInfo(
							1,
							"Generate: C++|MyPackage",
							CommandInfo(
								Path("C:/WorkingDirectory/MyPackage/"),
								Path("C:/testlocation/Generate/Soup.Build.Generate.exe"),
								"C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
							{},
							{},
							{},
							{},
							{},
							1,
							true,
							std::chrono::time_point<std::chrono::system_clock>::min(),
							{},
							{}),
					})),
				myPackageGenerateGraph,
				"Verify file content match expected.");
		}
	};
}
