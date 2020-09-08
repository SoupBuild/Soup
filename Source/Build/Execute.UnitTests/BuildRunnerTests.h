// <copyright file="BuildRunnerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class BuildRunnerTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto uut = BuildRunner(Path("C:/BuildDirectory/"));
		}

		[[Fact]]
		void Execute_NoOperations_ForceBuild()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList();
			auto objectDirectory = Path("out/obj/release/");
			auto forceBuild = true;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/release/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/release/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/release/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/release/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}

		[[Fact]]
		void Execute_NoOperations_Incremental()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList();
			auto objectDirectory = Path("out/obj/release/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"INFO: BuildHistory file does not exist",
					"INFO: No previous state found, full rebuild required",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/release/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/release/.soup/BuildHistory.json",
					"Exists: C:/BuildDirectory/out/obj/release/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/release/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/release/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}
		
		[[Fact]]
		void Execute_OneOperation_ForceBuild()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList(
				std::vector<Memory::Reference<Utilities::BuildOperation>>({
					new Utilities::BuildOperation(
						"TestCommand: 1",
						Path("Command.exe"),
						"Arguments",
						Path("C:/TestWorkingDirectory/"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})),
				}));
			auto objectDirectory = Path("out/obj/release/");
			bool forceBuild = true;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/release/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/release/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/release/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/release/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneOperation_Incremental_NoBuildHistory()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList(
				std::vector<Memory::Reference<Utilities::BuildOperation>>({
					new Utilities::BuildOperation(
						"TestCommand: 1",
						Path("Command.exe"),
						"Arguments",
						Path("C:/TestWorkingDirectory/"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})),
				}));
			auto objectDirectory = Path("out/obj/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"INFO: BuildHistory file does not exist",
					"INFO: No previous state found, full rebuild required",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/debug/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"Exists: C:/BuildDirectory/out/obj/debug/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/debug/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneOperation_Incremental_MissingFileInfo()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Create the initial build state
			auto initialBuildHistory = BuildHistory();
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json"),
				std::make_shared<MockFile>(std::move(initialBuildHistoryJson)));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList(
				std::vector<Memory::Reference<Utilities::BuildOperation>>({
					new Utilities::BuildOperation(
						"TestCommand: 1",
						Path("Command.exe"),
						"Arguments",
						Path("C:/TestWorkingDirectory/"),
						std::vector<Path>({
							Path("InputFile.cpp"),
						}),
						std::vector<Path>({
							Path("OutputFile.obj"),
						})),
				}));
			auto objectDirectory = Path("out/obj/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/debug/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"Exists: C:/BuildDirectory/out/obj/debug/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/debug/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneOperation_Incremental_MissingTargetFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Create the initial build state
			auto initialBuildHistory = BuildHistory({
					OperationInfo(
						"C:/TestWorkingDirectory/ : ./Command.exe Arguments",
						{ Path("InputFile.in") },
						{ Path("OutputFile.out") }),
			});
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json"),
				std::make_shared<MockFile>(std::move(initialBuildHistoryJson)));

			// Setup the input file only
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(
				Path("C:/TestWorkingDirectory/InputFile.in"),
				std::make_shared<MockFile>(inputTime));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList(
				std::vector<Memory::Reference<Utilities::BuildOperation>>({
					new Utilities::BuildOperation(
						"TestCommand: 1",
						Path("Command.exe"),
						"Arguments",
						Path("C:/TestWorkingDirectory/"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})),
				}));
			auto objectDirectory = Path("out/obj/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"INFO: Output target does not exist: C:/TestWorkingDirectory/OutputFile.out",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/debug/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"Exists: C:/TestWorkingDirectory/OutputFile.out",
					"Exists: C:/BuildDirectory/out/obj/debug/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/debug/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneOperation_Incremental_OutOfDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Create the initial build state
			auto initialBuildHistory = BuildHistory({
					OperationInfo(
						"C:/TestWorkingDirectory/ : ./Command.exe Arguments",
						{ Path("InputFile.in") },
						{ Path("OutputFile.out") }),
			});
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json"),
				std::make_shared<MockFile>(std::move(initialBuildHistoryJson)));

			// Setup the input/output files to be out of date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 10);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(
				Path("Command.exe"),
				std::make_shared<MockFile>(outputTime));
			fileSystem->CreateMockFile(
				Path("C:/TestWorkingDirectory/OutputFile.out"),
				std::make_shared<MockFile>(outputTime));
			fileSystem->CreateMockFile(
				Path("C:/TestWorkingDirectory/InputFile.in"),
				std::make_shared<MockFile>(inputTime));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList(
				std::vector<Memory::Reference<Utilities::BuildOperation>>({
					new Utilities::BuildOperation(
						"TestCommand: 1",
						Path("Command.exe"),
						"Arguments",
						Path("C:/TestWorkingDirectory/"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})),
				}));
			auto objectDirectory = Path("out/obj/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"DIAG: IsOutdated: C:/TestWorkingDirectory/OutputFile.out [1434964200]",
					"DIAG:   C:/TestWorkingDirectory/InputFile.in [1434964260]",
					"INFO: Input altered after target [C:/TestWorkingDirectory/InputFile.in] -> [C:/TestWorkingDirectory/OutputFile.out]",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/debug/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"Exists: C:/TestWorkingDirectory/OutputFile.out",
					"GetLastWriteTime: C:/TestWorkingDirectory/OutputFile.out",
					"Exists: C:/TestWorkingDirectory/InputFile.in",
					"GetLastWriteTime: C:/TestWorkingDirectory/InputFile.in",
					"Exists: C:/BuildDirectory/out/obj/debug/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/debug/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				detourProcessManager->GetRequests(),
				"Verify detour process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneOperation_Incremental_UpToDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Create the initial build state
			auto initialBuildHistory = BuildHistory({
					OperationInfo(
						"C:/TestWorkingDirectory/ : ./Command.exe Arguments",
						{ Path("InputFile.in") },
						{ Path("OutputFile.out") }),
			});
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json"),
				std::make_shared<MockFile>(std::move(initialBuildHistoryJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(
				Path("Command.exe"),
				std::make_shared<MockFile>(outputTime));
			fileSystem->CreateMockFile(
				Path("C:/TestWorkingDirectory/OutputFile.out"),
				std::make_shared<MockFile>(outputTime));
			fileSystem->CreateMockFile(
				Path("C:/TestWorkingDirectory/InputFile.in"),
				std::make_shared<MockFile>(inputTime));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto operations = Utilities::BuildOperationList(
				std::vector<Memory::Reference<Utilities::BuildOperation>>({
					new Utilities::BuildOperation(
						"TestCommand: 1",
						Path("Command.exe"),
						"Arguments",
						Path("C:/TestWorkingDirectory/"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})),
				}));
			auto objectDirectory = Path("out/obj/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				objectDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"DIAG: IsOutdated: C:/TestWorkingDirectory/OutputFile.out [1434964320]",
					"DIAG:   C:/TestWorkingDirectory/InputFile.in [1434964260]",
					"INFO: Up to date",
					"INFO: TestCommand: 1",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/obj/debug/.soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
					"Exists: C:/TestWorkingDirectory/OutputFile.out",
					"GetLastWriteTime: C:/TestWorkingDirectory/OutputFile.out",
					"Exists: C:/TestWorkingDirectory/InputFile.in",
					"GetLastWriteTime: C:/TestWorkingDirectory/InputFile.in",
					"Exists: C:/BuildDirectory/out/obj/debug/.soup",
					"CreateDirectory: C:/BuildDirectory/out/obj/debug/.soup",
					"OpenWrite: C:/BuildDirectory/out/obj/debug/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
