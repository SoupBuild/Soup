// <copyright file="BuildEvaluateEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class BuildEvaluateEngineTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto fileSystemState = std::make_shared<FileSystemState>();
			auto operationGraph = OperationGraph();
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto uut = BuildEvaluateEngine(
				fileSystemState,
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);
		}

		// [[Fact]]
		void Evaluate_NoOperations()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = std::make_shared<FileSystemState>();

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Setup the input build state
			auto operationGraph = OperationGraph();
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto uut = BuildEvaluateEngine(
				fileSystemState,
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

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

		// [[Fact]]
		void Execute_OneOperation_FirstRun()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = std::make_shared<FileSystemState>(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto operationGraph = OperationGraph(
				{
					{ 1, Path("C:/Folder/File.txt") },
				},
				{ 1, },
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1,
						false,
						{ },
						{ }),
				});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto uut = BuildEvaluateEngine(
				fileSystemState,
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Operation has no successful previous invocation",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"DIAG: Allowed Read Access:",
					"DIAG: Allowed Write Access:",
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
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
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
		void Evaluate_OneOperation_Incremental_MissingFileInfo()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = std::make_shared<FileSystemState>(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto operationGraph = OperationGraph(
				{
					{ 1, Path("C:/Folder/File.txt") },
				},
				{ 1, },
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1,
						true,
						{ 1, },
						{ 2, }),
				});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto uut = BuildEvaluateEngine(
				fileSystemState,
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Output target does not exist: C:/TestWorkingDirectory/OutputFile.out",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"DIAG: Allowed Read Access:",
					"DIAG: Allowed Write Access:",
					"DIAG: Build evaluation end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestWorkingDirectory/OutputFile.out",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
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
		void Evaluate_OneOperation_Incremental_MissingTargetFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Setup the input file only
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = std::make_shared<FileSystemState>(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
				}),
				std::unordered_map<FileId, std::optional<time_t>>({
					{ 1, inputTime },
					{ 2, std::nullopt },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Create the build state
			auto operationGraph = OperationGraph(
				{
					{ 1, Path("C:/Folder/File.txt") },
				},
				{ 1, },
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1,
						true,
						{ 1, },
						{ 2, }),
				});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto uut = BuildEvaluateEngine(
				fileSystemState,
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Output target does not exist: C:/TestWorkingDirectory/OutputFile.out",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"DIAG: Allowed Read Access:",
					"DIAG: Allowed Write Access:",
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
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
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
		void Evaluate_OneOperation_Incremental_OutOfDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Setup the input/output files to be out of date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 10);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = std::make_shared<FileSystemState>(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
				}),
				std::unordered_map<FileId, std::optional<time_t>>({
					{ 1, inputTime },
					{ 2, outputTime },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcesManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto operationGraph = OperationGraph(
				{
					{ 1, Path("C:/Folder/File.txt") },
				},
				{ 1, },
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1,
						true,
						{ 1, },
						{ 2, }),
				});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto uut = BuildEvaluateEngine(
				fileSystemState,
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			// Evaluate the build
			uut.Evaluate();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Input altered after target [C:/TestWorkingDirectory/InputFile.in] -> [C:/TestWorkingDirectory/OutputFile.out]",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: [C:/TestWorkingDirectory/] ./Command.exe Arguments",
					"DIAG: Allowed Read Access:",
					"DIAG: Allowed Write Access:",
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
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
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
		void Evaluate_OneOperation_Incremental_UpToDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = std::make_shared<FileSystemState>(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
				}),
				std::unordered_map<FileId, std::optional<time_t>>({
					{ 1, inputTime },
					{ 2, outputTime },
				}));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Create the initial build state
			auto operationGraph = OperationGraph(
				{
					{ 1, Path("C:/Folder/File.txt") },
				},
				{ 1, },
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command.exe"),
							"Arguments"),
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1,
						true,
						{ 1, },
						{ 2, }),
				});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto uut = BuildEvaluateEngine(
				fileSystemState,
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

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
