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

			fileSystem->CreateMockDirectory(
				Path("C:/WorkingDirectory/MyPackage/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			fileSystem->CreateMockDirectory(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'MyPackage'
					Language: 'C++|0.8.2'
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'Soup.Cpp'
					Language: 'Wren|1'
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'Soup.Wren'
					Language: 'Wren|1'
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							'C++': {
								MyPackage: { Version: '../MyPackage/', Build: 'Build0', Tool: 'Tool0' }
							}
						}
						Build0: {
							Wren: {
								'mwasplund|Soup.Cpp': { Version: '0.8.2' }
							}
						}
						Tool0: {}
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/locks/Wren/mwasplund/Soup.Cpp/0.8.2/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							Wren: {
								'mwasplund|Soup.Cpp': { Version: './', Build: 'Build0', Tool: 'Tool0' }
							}
						}
						Build0: {
							Wren: {
								'mwasplund|Soup.Wren': { Version: '0.4.1' }
							}
						}
						Tool0: {}
					}
				)")));

			auto fileSystemState = FileSystemState();

			auto myPackageOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto myPackageFiles = std::set<FileId>();
			auto myPackageOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myPackageOperationGraph, myPackageFiles, fileSystemState, myPackageOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(myPackageOperationGraphContent)));

			auto soupCppOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto soupCppFiles = std::set<FileId>();
			auto soupCppOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(soupCppOperationGraph, soupCppFiles, fileSystemState, soupCppOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(soupCppOperationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Register the test process manager
			auto monitorProcessManager = std::make_shared<Monitor::MockMonitorProcessManager>();
			auto scopedMonitorProcessManager = Monitor::ScopedMonitorProcessManagerRegister(monitorProcessManager);

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto arguments = RecipeBuildArguments();
			arguments.HostPlatform = "TestPlatform";
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");

			// Load user config state
			auto userDataPath = BuildEngine::GetSoupUserDataPath();
			auto recipeCache = RecipeCache();

			auto packageProvider = BuildEngine::LoadBuildGraph(
				builtInPackageDirectory,
				arguments.WorkingDirectory,
				arguments.GlobalParameters,
				userDataPath,
				recipeCache);

			BuildEngine::Execute(
				packageProvider,
				std::move(arguments),
				userDataPath,
				recipeCache);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/mwasplund/Soup.Cpp/0.8.2/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/Recipe.sml",
					"DIAG: 0>Package was prebuilt: mwasplund|Soup.Wren",
					"DIAG: 2>Running Build: [Wren]mwasplund|Soup.Cpp",
					"INFO: 2>Build 'mwasplund|Soup.Cpp'",
					"INFO: 2>Checking for existing Evaluate Operation Graph",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog",
					"INFO: 2>Previous graph found",
					"INFO: 2>Checking for existing Evaluate Operation Results",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bor",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/",
					"INFO: 2>Check outdated generate input file: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Generate Input file",
					"INFO: 2>Checking for existing Generate Operation Results",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Check for previous operation invocation",
					"INFO: 2>Operation has no successful previous invocation",
					"HIGH: 2>Generate: [Wren]mwasplund|Soup.Cpp",
					"DIAG: 2>Execute: [C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/] C:/testlocation/Soup.Generate.exe C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/",
					"DIAG: 2>Allowed Read Access:",
					"DIAG: 2>C:/testlocation/",
					"DIAG: 2>C:/Users/Me/.soup/LocalUserConfig.sml",
					"DIAG: 2>C:/Windows/",
					"DIAG: 2>C:/Program Files/dotnet/",
					"DIAG: 2>C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/out/",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/",
					"DIAG: 2>Allowed Write Access:",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Loading new Evaluate Operation Graph",
					"DIAG: 2>Map previous operation graph observed results",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/temp/",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Build evaluation end",
					"INFO: 2>Done",
					"DIAG: 1>Running Build: [C++]MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"INFO: 1>Previous graph found",
					"INFO: 1>Checking for existing Evaluate Operation Results",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bor",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"INFO: 1>Check outdated generate input file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Generate Input file",
					"INFO: 1>Checking for existing Generate Operation Results",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Check for previous operation invocation",
					"INFO: 1>Operation has no successful previous invocation",
					"HIGH: 1>Generate: [C++]MyPackage",
					"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/Soup.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"DIAG: 1>Allowed Read Access:",
					"DIAG: 1>C:/testlocation/",
					"DIAG: 1>C:/Users/Me/.soup/LocalUserConfig.sml",
					"DIAG: 1>C:/Windows/",
					"DIAG: 1>C:/Program Files/dotnet/",
					"DIAG: 1>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/",
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
					"GetCurrentTime",
				}),
				system->GetRequests(),
				"Verify system requests match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentDirectory",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/Recipe.sml",
					"TryOpenReadBinary: C:/Users/Me/.soup/locks/Wren/mwasplund/Soup.Cpp/0.8.2/PackageLock.sml",
					"TryOpenReadBinary: C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/Recipe.sml",
					"GetDirectoryFilesLastWriteTime: C:/WorkingDirectory/MyPackage/",
					"GetDirectoryFilesLastWriteTime: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/",
					"GetDirectoryFilesLastWriteTime: C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"GetDirectoryFilesLastWriteTime: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bor",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/temp/",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"GetDirectoryFilesLastWriteTime: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
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
					"CreateMonitorProcess: 1 [C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/] C:/testlocation/Soup.Generate.exe C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					"CreateMonitorProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/Soup.Generate.exe C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]",
					"ProcessStart: 2",
					"WaitForExit: 2",
					"GetStandardOutput: 2",
					"GetStandardError: 2",
					"GetExitCode: 2",
				}),
				monitorProcessManager->GetRequests(),
				"Verify monitor process manager requests match expected.");

			// Verify files
			auto myPackageGenerateInputMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt"));
			Assert::AreEqual(
				ValueTable({
					{
						"Dependencies",
						ValueTable(
						{
							{
								"Build",
								ValueTable(
								{
									{
										"mwasplund|Soup.Cpp",
										ValueTable(
										{
											{ "SoupTargetDirectory", std::string("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/") },
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
							{ "/(BUILD_TARGET_mwasplund|Soup.Cpp)/", std::string("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/") },
						})
					},
					{
						"GenerateSubGraphMacros",
						ValueTable(
						{
							{ "/(TARGET_mwasplund|Soup.Cpp)/", std::string("/(BUILD_TARGET_mwasplund|Soup.Cpp)/") },
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
									{ "HostPlatform", std::string("TestPlatform") },
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
												"mwasplund|Soup.Cpp",
												ValueTable(
												{
													{
														"Context",
														ValueTable(
														{
															{ "Reference", std::string("[Wren]mwasplund|Soup.Cpp@0.8.2") },
															{ "TargetDirectory", std::string("/(TARGET_mwasplund|Soup.Cpp)/") },
														})
													},
												})
											},
										})
									},
								})
							},
							{
								"FileSystem",
								ValueList({
									std::string("Recipe.sml"),
								})
							},
							{ "Parameters", ValueTable() },
						})
					},
					{
						"PackageRoot",
						std::string("C:/WorkingDirectory/MyPackage/")
					},
					{
						"UserDataPath",
						std::string("C:/Users/Me/.soup/")
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

			auto soupCppGenerateInputMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt"));
			Assert::AreEqual(
				ValueTable({
					{
						"Dependencies",
						ValueTable(
						{
							{
								"Build",
								ValueTable(
								{
									{
										"mwasplund|Soup.Wren",
										ValueTable(
										{
											{ "SoupTargetDirectory", std::string("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/out/.soup/") },
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
							{ "/(PACKAGE_mwasplund|Soup.Cpp)/", std::string("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/") },
							{ "/(TARGET_mwasplund|Soup.Cpp)/", std::string("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/") },
						})
					},
					{
						"EvaluateReadAccess",
						ValueList(
						{
							std::string("/(PACKAGE_mwasplund|Soup.Cpp)/"),
							std::string("/(TARGET_mwasplund|Soup.Cpp)/"),
						})
					},
					{
						"EvaluateWriteAccess",
						ValueList(
						{
							std::string("/(TARGET_mwasplund|Soup.Cpp)/"),
						})
					},
					{
						"GenerateMacros",
						ValueTable(
						{
							{ "/(BUILD_TARGET_mwasplund|Soup.Wren)/", std::string("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/out/") },
						})
					},
					{
						"GenerateSubGraphMacros",
						ValueTable(
						{
							{ "/(TARGET_mwasplund|Soup.Wren)/", std::string("/(BUILD_TARGET_mwasplund|Soup.Wren)/") },
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
									{ "HostPlatform", std::string("TestPlatform") },
									{ "PackageDirectory", std::string("/(PACKAGE_mwasplund|Soup.Cpp)/") },
									{ "TargetDirectory", std::string("/(TARGET_mwasplund|Soup.Cpp)/") },
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
												"mwasplund|Soup.Wren",
												ValueTable(
												{
													{
														"Context",
														ValueTable(
														{
															{ "Reference", std::string("[Wren]mwasplund|Soup.Wren@0.4.1") },
															{ "TargetDirectory", std::string("/(TARGET_mwasplund|Soup.Wren)/") },
														})
													},
												})
											},
										})
									},
								})
							},
							{
								"FileSystem",
								ValueList({
									std::string("Recipe.sml"),
								})
							},
							{
								"Parameters",
								ValueTable(
								{
									{ "System", std::string("Windows") },
								})
							},
						})
					},
					{
						"PackageRoot",
						std::string("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/")
					},
					{
						"UserDataPath",
						std::string("C:/Users/Me/.soup/")
					},
				}),
				ValueTableReader::Deserialize(soupCppGenerateInputMockFile->Content),
				"Verify file content match expected.");

			auto soupCppGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor"));
			auto soupCppGenerateResults = OperationResultsReader::Deserialize(soupCppGenerateResultsMockFile->Content, fileSystemState);

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
				soupCppGenerateResults,
				"Verify my package generate results content match expected.");
		}
	};
}
