// <copyright file="BuildEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationGraph/OperationGraphWriter.h"

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

			// Register the test system
			auto system = std::make_shared<MockSystem>();
			auto scopedSystem = ScopedSystemRegister(system);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|0.7.0"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/testlocation/BuiltIn/Soup.Cpp/0.7.0/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			auto operationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto files = std::set<FileId>();
			auto fileSystemState = FileSystemState();
			auto operationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(operationGraph, files, fileSystemState, operationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(operationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Register the test process manager
			auto monitorProcessManager = std::make_shared<Monitor::MockMonitorProcessManager>();
			auto scopedMonitorProcessManager = Monitor::ScopedMonitorProcessManagerRegister(monitorProcessManager);

			auto arguments = RecipeBuildArguments();
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			BuildEngine::Execute(std::move(arguments));

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load Local User Config: C:/Users/Me/.soup/LocalUserConfig.sml",
					"WARN: Local User Config file does not exist",
					"WARN: Local User Config invalid",
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/testlocation/BuiltIn/Soup.Cpp/0.7.0/Recipe.sml",
					"DIAG: 0>Package was prebuilt: Soup.Cpp",
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"INFO: 1>Previous graph found",
					"INFO: 1>Checking for existing Evaluate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Check outdated generate input file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Generate Input file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Checking for existing Generate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Check for previous operation invocation",
					"INFO: 1>Operation has no successful previous invocation",
					"HIGH: 1>Generate: C++|MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Soup.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>C:/testlocation/BuiltIn/Soup.Cpp/0.7.0/out/",
					"DIAG: 1>Allowed Write Access:",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Loading new Evaluate Operation Graph",
					"DIAG: 1>Map previous operation graph observed results",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentTime",
				}),
				system->GetRequests(),
				"Verify system requests match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/LocalUserConfig.sml",
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/testlocation/BuiltIn/Soup.Cpp/0.7.0/Recipe.sml",
					"OpenReadBinary: C:/testlocation/BuiltIn/Soup.Cpp/0.7.0/Recipe.sml",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
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
					"CreateMonitorProcess: 1 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Soup.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [6] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				monitorProcessManager->GetRequests(),
				"Verify monitor process manager requests match expected.");

			// Verify files
			auto myPackageGenerateInputMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt"));
			Assert::AreEqual(
				ValueTable(
				{
					{
						"Dependencies",
						ValueTable(
						{
							{
								"Build",
								ValueTable(
								{
									{
										"Soup.Cpp",
										ValueTable(
										{
											{ "SoupTargetDirectory", std::string("C:/testlocation/BuiltIn/Soup.Cpp/0.7.0/out/.soup/") },
										})
									},
								})
							},
						})
					},
					{
						"EvaluateMacros",
						ValueTable(
						{
							{ "/(PACKAGE_MyPackage)/", std::string("C:/WorkingDirectory/MyPackage/") },
							{ "/(TARGET_MyPackage)/", std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/") },
						})
					},
					{
						"EvaluateReadAccess",
						ValueList(
						{
							std::string("/(PACKAGE_MyPackage)/"),
							std::string("/(TARGET_MyPackage)/"),
						})
					},
					{
						"EvaluateWriteAccess",
						ValueList(
						{
							std::string("/(TARGET_MyPackage)/"),
						})
					},
					{
						"GenerateMacros",
						ValueTable(
						{
							{ "/(TARGET_Soup.Cpp)/", std::string("C:/testlocation/BuiltIn/Soup.Cpp/0.7.0/out/") },
						})
					},
					{
						"GlobalState",
						ValueTable(
						{
							{
								"Context",
								ValueTable(
								{
									{ "PackageDirectory", std::string("/(PACKAGE_MyPackage)/") },
									{ "TargetDirectory", std::string("/(TARGET_MyPackage)/") },
								})
							},
							{
								"Dependencies",
								ValueTable(
								{
									{
										"Build",
										ValueTable(
										{
											{
												"Soup.Cpp",
												ValueTable(
												{
													{
														"Context",
														ValueTable(
														{
															{ "Reference", std::string("Wren|Soup.Cpp@0.7.0") },
															{ "TargetDirectory", std::string("/(TARGET_Soup.Cpp)/") },
														})
													},
												})
											},
										})
									},
								})
							},
							{ "Parameters", ValueTable() },
							{ "SDKs", ValueList() },
						})
					},
					{
						"PackageRoot",
						std::string("C:/WorkingDirectory/MyPackage/")
					},
				}),
				ValueTableReader::Deserialize(myPackageGenerateInputMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor"));
			auto myPackageGenerateResults = OperationResultsReader::Deserialize(myPackageGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::time_point<std::chrono::system_clock>()),
							{},
							{})
					},
				}),
				myPackageGenerateResults,
				"Verify my package generate results content match expected.");
		}
	};
}
