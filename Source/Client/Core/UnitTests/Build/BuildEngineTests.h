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

			// Register the test system
			auto system = std::make_shared<MockSystem>();
			auto scopedSystem = ScopedSystemRegister(system);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			fileSystem->CreateMockDirectory(
				Path("C:/WorkingDirectory/MyPackage/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("./Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("./Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("./Recipe.sml"),
				})));

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'MyPackage'
					Language: (C++@0.8)
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'Soup.Cpp'
					Language: (Wren@1)
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'Soup.Wren'
					Language: (Wren@1)
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
								'mwasplund|Soup.Cpp': { Version: 0.8.2 }
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
								'mwasplund|Soup.Wren': { Version: 0.4.1 }
							}
						}
						Tool0: {}
					}
				)")));

			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Register the test process manager
			auto monitorProcessManager = std::make_shared<Monitor::MockMonitorProcessManager>();
			auto scopedMonitorProcessManager = Monitor::ScopedMonitorProcessManagerRegister(monitorProcessManager);

			// Emulate generate phase
			monitorProcessManager->RegisterExecuteCallback(
				std::format("CreateMonitorProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/{0} C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]", GetGenerateExeName()),
				[&](Monitor::ISystemAccessMonitor& /*monitor*/)
				{
					auto myPackageOperationGraph = OperationGraph(
						std::vector<OperationId>(),
						std::vector<OperationInfo>());
					auto myPackageFiles = std::set<FileId>();
					auto myPackageOperationGraphContent = std::stringstream();
					OperationGraphWriter::Serialize(myPackageOperationGraph, myPackageFiles, fileSystemState, myPackageOperationGraphContent);
					fileSystem->CreateMockFile(
						Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog"),
						std::make_shared<MockFile>(std::move(myPackageOperationGraphContent)));
				});

			monitorProcessManager->RegisterExecuteCallback(
				std::format("CreateMonitorProcess: 1 [C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/] C:/testlocation/{0} C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]", GetGenerateExeName()),
				[&](Monitor::ISystemAccessMonitor& /*monitor*/)
				{
					auto soupCppOperationGraph = OperationGraph(
						std::vector<OperationId>(),
						std::vector<OperationInfo>());
					auto soupCppFiles = std::set<FileId>();
					auto soupCppOperationGraphContent = std::stringstream();
					OperationGraphWriter::Serialize(soupCppOperationGraph, soupCppFiles, fileSystemState, soupCppOperationGraphContent);
					fileSystem->CreateMockFile(
						Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog"),
						std::make_shared<MockFile>(std::move(soupCppOperationGraphContent)));
				});

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
					"INFO: 2>Preload Directory Missing: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/",
					"INFO: 2>Checking for existing Evaluate Operation Graph",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog",
					"INFO: 2>Operation graph file does not exist",
					"INFO: 2>No previous graph found",
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
					std::format("DIAG: 2>Execute: [C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/] C:/testlocation/{0} C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/", GetGenerateExeName()),
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
					"INFO: 1>Preload Directory Missing: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"INFO: 1>Operation graph file does not exist",
					"INFO: 1>No previous graph found",
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
					std::format("DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/{0} C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/", GetGenerateExeName()),
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
					"TryGetDirectoryFilesLastWriteTime: C:/WorkingDirectory/MyPackage/",
					"TryGetDirectoryFilesLastWriteTime: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/",
					"TryGetDirectoryFilesLastWriteTime: C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"TryGetDirectoryFilesLastWriteTime: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog",
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
					"TryGetDirectoryFilesLastWriteTime: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
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
					std::format("CreateMonitorProcess: 1 [C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/] C:/testlocation/{0} C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]", GetGenerateExeName()),
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
					std::format("CreateMonitorProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/{0} C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]", GetGenerateExeName()),
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

		// [[Fact]]
		void Execute_NoDependencies_UpToDate()
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

			fileSystem->CreateMockFile(
				Path(std::format("C:/testlocation/{0}", GetGenerateExeName())),
				std::make_shared<MockFile>());

			fileSystem->CreateMockDirectory(
				Path("C:/WorkingDirectory/MyPackage/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("./Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			fileSystem->CreateMockDirectory(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			fileSystem->CreateMockDirectory(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			fileSystem->CreateMockDirectory(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("./Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/"),
				std::make_shared<MockDirectory>(std::vector<Path>({
					Path("./Recipe.sml"),
				})));

			fileSystem->CreateMockDirectory(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			fileSystem->CreateMockDirectory(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			fileSystem->CreateMockDirectory(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/temp/"),
				std::make_shared<MockDirectory>(std::vector<Path>({})));

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'MyPackage'
					Language: (C++@0.8)
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'Soup.Cpp'
					Language: (Wren@1)
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: 'Soup.Wren'
					Language: (Wren@1)
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
								'mwasplund|Soup.Cpp': { Version: 0.8.2 }
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
								'mwasplund|Soup.Wren': { Version: 0.4.1 }
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

			auto soupCppGenerateInput = ValueTable({
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
			});
			auto soupCppGenerateInputContent = std::stringstream();
			ValueTableWriter::Serialize(soupCppGenerateInput, soupCppGenerateInputContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt"),
				std::make_shared<MockFile>(std::move(soupCppGenerateInputContent)));

			auto myPackageGenerateInput = ValueTable({
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
			});
			auto myPackageGenerateInputContent = std::stringstream();
			ValueTableWriter::Serialize(myPackageGenerateInput, myPackageGenerateInputContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt"),
				std::make_shared<MockFile>(std::move(myPackageGenerateInputContent)));

			auto myPackageGenerateResults = OperationResults({
				{
					1,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::time_point<std::chrono::system_clock>()),
						{},
						{})
				},
			});
			auto myPackageGenerateResultsContent = std::stringstream();
			auto myPackageGenerateResultsFiles = std::set<FileId>();
			OperationResultsWriter::Serialize(myPackageGenerateResults, myPackageGenerateResultsFiles, fileSystemState,  myPackageGenerateResultsContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor"),
				std::make_shared<MockFile>(std::move(myPackageGenerateResultsContent)));

			auto soupCppGenerateResults = OperationResults({
				{
					1,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::time_point<std::chrono::system_clock>()),
						{},
						{})
				},
			});
			auto soupCppGenerateResultsContent = std::stringstream();
			auto soupCppGenerateResultsFiles = std::set<FileId>();
			OperationResultsWriter::Serialize(soupCppGenerateResults, soupCppGenerateResultsFiles, fileSystemState, soupCppGenerateResultsContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor"),
				std::make_shared<MockFile>(std::move(soupCppGenerateResultsContent)));

			auto soupCppEvaluateResults = OperationResults();
			auto soupCppEvaluateResultsContent = std::stringstream();
			auto soupCppEvaluateResultsFiles = std::set<FileId>();
			OperationResultsWriter::Serialize(soupCppEvaluateResults, soupCppEvaluateResultsFiles, fileSystemState, soupCppEvaluateResultsContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bor"),
				std::make_shared<MockFile>(std::move(soupCppEvaluateResultsContent)));

			auto myPackageEvaluateResults = OperationResults();
			auto myPackageEvaluateResultsContent = std::stringstream();
			auto myPackageEvaluateResultsFiles = std::set<FileId>();
			OperationResultsWriter::Serialize(myPackageEvaluateResults, myPackageEvaluateResultsFiles, fileSystemState, myPackageEvaluateResultsContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bor"),
				std::make_shared<MockFile>(std::move(myPackageEvaluateResultsContent)));

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
					"INFO: 2>Previous results found",
					"INFO: 2>Check outdated generate input file: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt",
					"INFO: 2>Checking for existing Generate Operation Results",
					"DIAG: 2>C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor",
					"INFO: 2>Previous results found",
					"DIAG: 2>Build evaluation start",
					"DIAG: 2>Check for previous operation invocation",
					#ifdef _WIN32
						"INFO: 2>Up to date",
						"INFO: 2>Generate: [Wren]mwasplund|Soup.Cpp",
						"DIAG: 2>Build evaluation end",
					#else
						"INFO: 2>Input altered after last evaluate [C:/testlocation/generate]",
						"DIAG: 2>Executable out of date",
						"HIGH: 2>Generate: [Wren]mwasplund|Soup.Cpp",
						"DIAG: 2>Execute: [C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/] C:/testlocation/generate C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/",
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
					#endif
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
					"INFO: 1>Previous results found",
					"INFO: 1>Check outdated generate input file: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"INFO: 1>Checking for existing Generate Operation Results",
					"DIAG: 1>C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
					"INFO: 1>Previous results found",
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Check for previous operation invocation",
					#ifdef _WIN32
						"INFO: 1>Up to date",
						"INFO: 1>Generate: [C++]MyPackage",
						"DIAG: 1>Build evaluation end",
					#else
						"INFO: 1>Input altered after last evaluate [C:/testlocation/generate]",
						"DIAG: 1>Executable out of date",
						"HIGH: 1>Generate: [C++]MyPackage",
						"DIAG: 1>Execute: [C:/WorkingDirectory/MyPackage/] C:/testlocation/generate C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
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
					#endif
					"DIAG: 1>Build evaluation start",
					"DIAG: 1>Build evaluation end",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected system requests
			Assert::AreEqual(
				std::vector<std::string>({
					#ifndef _WIN32
						"GetCurrentTime",
						"GetCurrentTime",
					#endif
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
					"TryGetDirectoryFilesLastWriteTime: C:/WorkingDirectory/MyPackage/",
					"TryGetDirectoryFilesLastWriteTime: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/",
					"TryGetDirectoryFilesLastWriteTime: C:/BuiltIn/Packages/mwasplund/Soup.Wren/0.4.1/",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"TryGetDirectoryFilesLastWriteTime: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bor",
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt",
					"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor",
					std::format("TryGetLastWriteTime: C:/testlocation/{0}", GetGenerateExeName()),
					#ifndef _WIN32
						"OpenWriteBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor",
						"TryOpenReadBinary: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog",
					#endif
					"Exists: C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/temp/",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"TryGetDirectoryFilesLastWriteTime: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt",
					"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
					#ifndef _WIN32
						"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor",
						"TryOpenReadBinary: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog",
					#endif
					"Exists: C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/",
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
					#ifndef _WIN32
						"CreateMonitorProcess: 1 [C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/] C:/testlocation/generate C:/Users/Me/.soup/packages/Wren/mwasplund/Soup.Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]",
						"ProcessStart: 1",
						"WaitForExit: 1",
						"GetStandardOutput: 1",
						"GetStandardError: 1",
						"GetExitCode: 1",
						std::format("CreateMonitorProcess: 2 [C:/WorkingDirectory/MyPackage/] C:/testlocation/{0} C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/ Environment [2] 1 AllowedRead [7] AllowedWrite [1]", GetGenerateExeName()),
						"ProcessStart: 2",
						"WaitForExit: 2",
						"GetStandardOutput: 2",
						"GetStandardError: 2",
						"GetExitCode: 2",
					#endif
				}),
				monitorProcessManager->GetRequests(),
				"Verify monitor process manager requests match expected.");
		}

	private:
		std::string_view GetGenerateExeName()
		{
			#ifdef _WIN32
				return "Soup.Generate.exe";
			#else
				return "generate";
			#endif
		}
	};
}
