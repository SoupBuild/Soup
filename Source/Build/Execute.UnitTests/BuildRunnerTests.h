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
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto fileSystemState = FileSystemState();
			auto uut = BuildRunner(workingDirectory, fileSystemState);
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

			// Setup the input build state
			auto operations = Utilities::BuildOperationList();
			auto outputDirectory = Path("out/release/");
			auto forceBuild = true;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/release/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/release/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/release/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/release/.soup/OperationHistory.bin",
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

			// Setup the input build state
			auto operations = Utilities::BuildOperationList();
			auto outputDirectory = Path("out/release/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"HIGH: Operation History file does not exist",
					"INFO: No previous operation state found, full rebuild required",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/release/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/release/.soup/OperationHistory.bin",
					"Exists: C:/BuildDirectory/out/release/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/release/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/release/.soup/OperationHistory.bin",
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

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
			auto outputDirectory = Path("out/release/");
			bool forceBuild = true;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/release/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/release/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/release/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/release/.soup/OperationHistory.bin",
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
		void Execute_OneOperation_Incremental_NoOperationHistory()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

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
			auto outputDirectory = Path("out/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"HIGH: Operation History file does not exist",
					"INFO: No previous operation state found, full rebuild required",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/debug/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"Exists: C:/BuildDirectory/out/debug/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/debug/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Create the initial operation history state
			auto initialOperationHistory = OperationHistory();
			std::stringstream initialBinaryOperationHistory;
			OperationHistoryWriter::Serialize(initialOperationHistory, initialBinaryOperationHistory);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/debug/.soup/OperationHistory.bin"),
				std::make_shared<MockFile>(std::move(initialBinaryOperationHistory)));

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

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
			auto outputDirectory = Path("out/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/debug/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"OpenReadBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"Exists: C:/BuildDirectory/out/debug/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/debug/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input file only
			auto inputFileId = fileSystemState.ToFileId(Path("/InputFile.in"), Path("C:/TestWorkingDirectory/"));
			auto outputFileId = fileSystemState.ToFileId(Path("/OutputFile.out"), Path("C:/TestWorkingDirectory/"));

			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystemState.SetLastWriteTime(inputFileId, inputTime);
			fileSystemState.SetLastWriteTime(outputFileId, std::nullopt);

			// Create the initial build state
			auto initialOperationHistory = OperationHistory({
				OperationInfo(
					CommandInfo(
						Path("C:/TestWorkingDirectory/"),
						Path("./Command.exe"),
						"Arguments"),
					{ inputFileId, },
					{ outputFileId, }),
			});
			std::stringstream initialBinaryOperationHistory;
			OperationHistoryWriter::Serialize(initialOperationHistory, initialBinaryOperationHistory);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/debug/.soup/OperationHistory.bin"),
				std::make_shared<MockFile>(std::move(initialBinaryOperationHistory)));

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

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
			auto outputDirectory = Path("out/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
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
					"INFO: Create Directory: C:/BuildDirectory/out/debug/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"OpenReadBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"Exists: C:/BuildDirectory/out/debug/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/debug/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input/output files to be out of date
			auto inputFileId = fileSystemState.ToFileId(Path("/InputFile.in"), Path("C:/TestWorkingDirectory/"));
			auto outputFileId = fileSystemState.ToFileId(Path("/OutputFile.out"), Path("C:/TestWorkingDirectory/"));

			auto outputTime = CreateDateTime(2015, 5, 22, 9, 10);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystemState.SetLastWriteTime(inputFileId, inputTime);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);

			// Create the initial build state
			auto initialOperationHistory = OperationHistory({
					OperationInfo(
					CommandInfo(
						Path("C:/TestWorkingDirectory/"),
						Path("./Command.exe"),
						"Arguments"),
						{ inputFileId, },
						{ outputFileId, }),
			});
			std::stringstream initialBinaryOperationHistory;
			OperationHistoryWriter::Serialize(initialOperationHistory, initialBinaryOperationHistory);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/debug/.soup/OperationHistory.bin"),
				std::make_shared<MockFile>(std::move(initialBinaryOperationHistory)));

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

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
			auto outputDirectory = Path("out/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"INFO: Input altered after target [C:/TestWorkingDirectory/InputFile.in] -> [C:/TestWorkingDirectory/OutputFile.out]",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/debug/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"OpenReadBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"Exists: C:/BuildDirectory/out/debug/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/debug/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Setup the input/output files to be up to date
			auto inputFileId = fileSystemState.ToFileId(Path("/InputFile.in"), Path("C:/TestWorkingDirectory/"));
			auto outputFileId = fileSystemState.ToFileId(Path("/OutputFile.out"), Path("C:/TestWorkingDirectory/"));

			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystemState.SetLastWriteTime(inputFileId, inputTime);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);

			// Create the initial build state
			auto initialOperationHistory = OperationHistory({
				OperationInfo(
					CommandInfo(
						Path("C:/TestWorkingDirectory/"),
						Path("./Command.exe"),
						"Arguments"),
					{ inputFileId, },
					{ outputFileId, }),
			});
			std::stringstream initialBinaryOperationHistory;
			OperationHistoryWriter::Serialize(initialOperationHistory, initialBinaryOperationHistory);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/out/debug/.soup/OperationHistory.bin"),
				std::make_shared<MockFile>(std::move(initialBinaryOperationHistory)));

			auto workingDirectory = Path("C:/BuildDirectory/");
			auto uut = BuildRunner(workingDirectory, fileSystemState);

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
			auto outputDirectory = Path("out/debug/");
			auto forceBuild = false;
			uut.Execute(
				Utilities::BuildOperationListWrapper(operations),
				outputDirectory,
				forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"INFO: Up to date",
					"INFO: TestCommand: 1",
					"INFO: Saving updated build state",
					"INFO: Create Directory: C:/BuildDirectory/out/debug/.soup/",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"OpenReadBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
					"Exists: C:/BuildDirectory/out/debug/.soup/",
					"CreateDirectory: C:/BuildDirectory/out/debug/.soup/",
					"OpenWriteBinary: C:/BuildDirectory/out/debug/.soup/OperationHistory.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
