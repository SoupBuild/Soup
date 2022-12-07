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
					Language: "C++|1"
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
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

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
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"INFO: 1>Previous graph found",
					"INFO: 1>Checking for existing Evaluate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
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
					"INFO: 1>Checking for existing Generate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
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
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bor",
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
				ValueTable(
				{
					{ "Dependencies", Value(ValueTable()) },
					{ "LanguageExtensionPath", Value(std::string("C:/testlocation/Extensions/Soup.Cpp/0.4.0/Soup.Cpp.dll")) },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")) },
					{ "SDKs", Value(ValueList()) },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/")) },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/")) },
				}),
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
