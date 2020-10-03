// <copyright file="BuildEvaluateEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class BuildEvaluateEngineTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto fileSystemState = FileSystemState(1234);
			auto operationGraph = OperationGraph(1234);
			auto uut = BuildEvaluateEngine(
				workingDirectory,
				fileSystemState,
				operationGraph);
		}

		[[Fact]]
		void Evaluate_NoOperations()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(1234);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Setup the input build state
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto operationGraph = OperationGraph(1234);
			auto uut = BuildEvaluateEngine(workingDirectory, fileSystemState, operationGraph);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneOperation_FirstRun()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(1234);

			auto inputFileId = fileSystemState.ToFileId(Path("/InputFile.in"), Path("C:/TestWorkingDirectory/"));
			auto outputFileId = fileSystemState.ToFileId(Path("/OutputFile.out"), Path("C:/TestWorkingDirectory/"));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto operationGraph = OperationGraph(
				1234,
				{
					1,
				},
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ inputFileId, },
						{ outputFileId, },
						{ },
						1,
						false,
						{ },
						{ }),
				});
			auto uut = BuildEvaluateEngine(workingDirectory, fileSystemState, operationGraph);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Operation has no successful previous invocation",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"DIAG: Build evaluation end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({}),
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
		void Evaluate_OneOperation_Incremental_MissingFileInfo()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(1234);

			auto inputFileId = fileSystemState.ToFileId(Path("/InputFile.in"), Path("C:/TestWorkingDirectory/"));
			auto outputFileId = fileSystemState.ToFileId(Path("/OutputFile.out"), Path("C:/TestWorkingDirectory/"));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto operationGraph = OperationGraph(
				1234,
				{
					1,
				},
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ inputFileId, },
						{ outputFileId, },
						{ },
						1,
						true,
						{ inputFileId, },
						{ outputFileId, }),
				});
			auto uut = BuildEvaluateEngine(workingDirectory, fileSystemState, operationGraph);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"WARN: Output file missing from file system state.",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"DIAG: Build evaluation end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({}),
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
		void Evaluate_OneOperation_Incremental_MissingTargetFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(1234);

			auto inputFileId = fileSystemState.ToFileId(Path("/InputFile.in"), Path("C:/TestWorkingDirectory/"));
			auto outputFileId = fileSystemState.ToFileId(Path("/OutputFile.out"), Path("C:/TestWorkingDirectory/"));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input file only
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystemState.SetLastWriteTime(inputFileId, inputTime);
			fileSystemState.SetLastWriteTime(outputFileId, std::nullopt);

			// Create the build state
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto operationGraph = OperationGraph(
				1234,
				{
					1,
				},
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ inputFileId, },
						{ outputFileId, },
						{ },
						1,
						true,
						{ inputFileId, },
						{ outputFileId, }),
				});

			auto uut = BuildEvaluateEngine(workingDirectory, fileSystemState, operationGraph);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Output target does not exist: C:/TestWorkingDirectory/OutputFile.out",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"DIAG: Build evaluation end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({}),
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
		void Evaluate_OneOperation_Incremental_OutOfDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(1234);

			auto inputFileId = fileSystemState.ToFileId(Path("/InputFile.in"), Path("C:/TestWorkingDirectory/"));
			auto outputFileId = fileSystemState.ToFileId(Path("/OutputFile.out"), Path("C:/TestWorkingDirectory/"));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input/output files to be out of date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 10);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystemState.SetLastWriteTime(inputFileId, inputTime);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);

			// Setup the input build state
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto operationGraph = OperationGraph(
				1234,
				{
					1,
				},
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ inputFileId, },
						{ outputFileId, },
						{ },
						1,
						true,
						{ inputFileId, },
						{ outputFileId, }),
				});
			auto uut = BuildEvaluateEngine(workingDirectory, fileSystemState, operationGraph);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Input altered after target [C:/TestWorkingDirectory/InputFile.in] -> [C:/TestWorkingDirectory/OutputFile.out]",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: ./Command.exe Arguments",
					"DIAG: Build evaluation end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({}),
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
		void Evaluate_OneOperation_Incremental_UpToDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(1234);

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
			auto workingDirectory = Path("C:/BuildDirectory/");
			auto operationGraph = OperationGraph(
				1234,
				{
					1,
				},
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ inputFileId, },
						{ outputFileId, },
						{ },
						1,
						true,
						{ inputFileId, },
						{ outputFileId, }),
				});
			auto uut = BuildEvaluateEngine(workingDirectory, fileSystemState, operationGraph);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Up to date",
					"INFO: TestCommand: 1",
					"DIAG: Build evaluation end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
