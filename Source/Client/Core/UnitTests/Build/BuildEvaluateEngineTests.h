// <copyright file="BuildEvaluateEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
using namespace std::chrono;

namespace Soup::Core::UnitTests
{
	class BuildEvaluateEngineTests
	{
		static const long GENERIC_WRITE = 0x40000000L;

	public:
		// [[Fact]]
		void Initialize()
		{
			auto fileSystemState = FileSystemState();
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);
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
			auto fileSystemState = FileSystemState();

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Setup the input build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph();
			auto operationResults = OperationResults();
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto ranOperations = uut.Evaluate(
				operationGraph,
				operationResults,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			Assert::IsFalse(ranOperations, "Verify no operations ran");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(),
				operationResults.GetResults(),
				"Verify operation results match expected.");

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

			// Register the test system
			auto system = std::make_shared<MockSystem>();
			auto scopedSystem = ScopedSystemRegister(system);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			detourProcessManager->RegisterExecuteCallback(
				"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
				[](Monitor::IDetourCallback& callback)
				{
					callback.OnCreateFile2(L"InputFile2.in", 0, 0, 0, 0, false);
					callback.OnCreateFile2(L"OutputFile2.out", GENERIC_WRITE, 0, 0, 0, false);
				});

			// Setup the input build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
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
						1),
				});
			auto operationResults = OperationResults();
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto ranOperations = uut.Evaluate(
				operationGraph,
				operationResults,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			Assert::IsTrue(ranOperations, "Verify ran operations");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(
				{
					{
						1,
						OperationResult(
							true,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::time_point<std::chrono::system_clock>()),
							{ 4, },
							{ 5, })
					},
				}),
				operationResults.GetResults(),
				"Verify operation results match expected.");

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
					"TryGetLastWriteTime: C:/TestWorkingDirectory/OutputFile2.out",
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
		void Execute_OneOperation_ObservedCircularReference_Fails()
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
			auto fileSystemState = FileSystemState(
				1,
				std::unordered_map<FileId, Path>({
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			detourProcessManager->RegisterExecuteCallback(
				"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
				[](Monitor::IDetourCallback& callback)
				{
					// Read and write the same file
					callback.OnCreateFile2(L"File.txt", 0, 0, 0, 0, false);
					callback.OnCreateFile2(L"File.txt", GENERIC_WRITE, 0, 0, 0, false);
				});

			// Setup the input build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
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
						1),
				});
			auto operationResults = OperationResults();
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();

			auto exception = Assert::Throws<std::runtime_error>([&]()
			{
				auto ranOperations = uut.Evaluate(
					operationGraph,
					operationResults,
					temporaryDirectory,
					globalAllowedReadAccess,
					globalAllowedWriteAccess);
			});

			Assert::AreEqual("", exception.what(), "Verify Exception message");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(
				{
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
				operationResults.GetResults(),
				"Verify operation results match expected.");

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

			// Verify expected system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentTime",
				}),
				system->GetRequests(),
				"Verify system requests match expected.");

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

			// Register the test system
			auto system = std::make_shared<MockSystem>();
			auto scopedSystem = ScopedSystemRegister(system);

			auto executableInputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 9min);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
					{ 3, Path("C:/TestWorkingDirectory/Command.exe") },
				}),
				std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::file_clock>>>({
					{ 3, executableInputTime },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
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
						1),
				});
			auto operationResults = OperationResults({
				{
					1,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::sys_days(May/22/2015) + 9h + 10min),
						{ 1, },
						{ 2, })
				},
			});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto ranOperations = uut.Evaluate(
				operationGraph,
				operationResults,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			Assert::IsTrue(ranOperations, "Verify ran operations");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(
				{
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
				operationResults.GetResults(),
				"Verify operation results match expected.");

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
					"TryGetLastWriteTime: C:/TestWorkingDirectory/OutputFile.out",
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

			// Register the test system
			auto system = std::make_shared<MockSystem>();
			auto scopedSystem = ScopedSystemRegister(system);

			// Setup the input file only
			auto inputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 11min);
			auto executableInputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 9min);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
					{ 3, Path("C:/TestWorkingDirectory/Command.exe") },
				}),
				std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::file_clock>>>({
					{ 1, inputTime },
					{ 2, std::nullopt },
					{ 3, executableInputTime },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Create the build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
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
						1),
				});
			auto operationResults = OperationResults({
				{
					1,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::sys_days(May/22/2015) + 9h + 10min),
						{ 1, },
						{ 2, })
				},
			});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto ranOperations = uut.Evaluate(
				operationGraph,
				operationResults,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			Assert::IsTrue(ranOperations, "Verify ran operations");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(
				{
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
				operationResults.GetResults(),
				"Verify operation results match expected.");

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

			// Verify expected system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentTime",
				}),
				system->GetRequests(),
				"Verify system requests match expected.");

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

			// Register the test system
			auto system = std::make_shared<MockSystem>();
			auto scopedSystem = ScopedSystemRegister(system);

			// Setup the input/output files to be out of date
			auto outputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 10min);
			auto inputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 11min);
			auto executableInputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 9min);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
					{ 3, Path("C:/TestWorkingDirectory/Command.exe") },
				}),
				std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::file_clock>>>({
					{ 1, inputTime },
					{ 2, outputTime },
					{ 3, executableInputTime },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
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
						1),
				});
			auto operationResults = OperationResults({
				{
					1,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::sys_days(May/22/2015) + 9h + 10min),
						{ 1, },
						{ 2, })
				},
			});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto ranOperations = uut.Evaluate(
				operationGraph,
				operationResults,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			Assert::IsTrue(ranOperations, "Verify ran operations");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(
				{
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
				operationResults.GetResults(),
				"Verify operation results match expected.");

			// Verify expected system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentTime",
				}),
				system->GetRequests(),
				"Verify system requests match expected.");

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
		void Evaluate_OneOperation_Incremental_Executable_OutOfDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test system
			auto system = std::make_shared<MockSystem>();
			auto scopedSystem = ScopedSystemRegister(system);

			// Setup the input/output files to be out of date
			auto outputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 10min);
			auto inputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 9min);
			auto executableInputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 11min);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
					{ 3, Path("C:/TestWorkingDirectory/Command.exe") },
				}),
				std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::file_clock>>>({
					{ 1, inputTime },
					{ 2, outputTime },
					{ 3, executableInputTime },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			// Setup the input build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
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
						1),
				});
			auto operationResults = OperationResults({
				{
					1,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::sys_days(May/22/2015) + 9h + 0min),
						{ 1, },
						{ 2, })
				},
			});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto ranOperations = uut.Evaluate(
				operationGraph,
				operationResults,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			Assert::IsTrue(ranOperations, "Verify ran operations");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(
				{
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
				operationResults.GetResults(),
				"Verify operation results match expected.");

			// Verify expected system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentTime",
				}),
				system->GetRequests(),
				"Verify system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Input altered after last evaluate [C:/TestWorkingDirectory/Command.exe]",
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
			auto outputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 12min);
			auto inputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 11min);
			auto executableInputTime = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::sys_days(May/22/2015) + 9h + 10min);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/InputFile.in") },
					{ 2, Path("C:/TestWorkingDirectory/OutputFile.out") },
					{ 3, Path("C:/TestWorkingDirectory/Command.exe") },
				}),
				std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::file_clock>>>({
					{ 1, inputTime },
					{ 2, outputTime },
					{ 3, executableInputTime },
				}));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			// Create the initial build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
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
						1),
				});
			auto operationResults = OperationResults({
				{
					1,
					OperationResult(
						true,
						std::chrono::clock_cast<std::chrono::file_clock>(std::chrono::sys_days(May/22/2015) + 9h + 15min),
						{ 1, },
						{ 2, })
				},
			});
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();
			auto ranOperations = uut.Evaluate(
				operationGraph,
				operationResults,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess);

			Assert::IsFalse(ranOperations, "Verify did not run operations");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(
				{
					{
						1,
						OperationResult(
							true,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::sys_days(May / 22 / 2015) + 9h + 15min),
							{ 1, },
							{ 2, })
					},
				}),
				operationResults.GetResults(),
				"Verify operation results match expected.");

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

		// [[Fact]]
		void Execute_TwoOperations_DuplicateOutputFile_Fails()
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
			auto fileSystemState = FileSystemState(
				3,
				std::unordered_map<FileId, Path>({
					{ 1, Path("C:/TestWorkingDirectory/OutputFile.out") },
				}));

			// Register the test process manager
			auto detourProcessManager = std::make_shared<Monitor::MockDetourProcessManager>();
			auto scopedDetourProcessManager = Monitor::ScopedDetourProcessManagerRegister(detourProcessManager);

			detourProcessManager->RegisterExecuteCallback(
				"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command1.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
				[](Monitor::IDetourCallback& callback)
				{
					callback.OnCreateFile2(L"OutputFile.out", GENERIC_WRITE, 0, 0, 0, false);
				});

			// Setup the input build state
			auto uut = BuildEvaluateEngine(
				false,
				fileSystemState);

			// Evaluate the build
			auto operationGraph = OperationGraph(
				{ 1, },
				{
					OperationInfo(
						1,
						"TestCommand: 1",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command1.exe"),
							"Arguments"),
						{ },
						{ },
						{ },
						{ },
						{ 2 },
						1),
					OperationInfo(
						2,
						"TestCommand: 2",
						CommandInfo(
							Path("C:/TestWorkingDirectory/"),
							Path("./Command2.exe"),
							"Arguments"),
						{ },
						{ 1, },
						{ },
						{ },
						{ },
						1),
				});
			auto operationResults = OperationResults();
			auto temporaryDirectory = Path();
			auto globalAllowedReadAccess = std::vector<Path>();
			auto globalAllowedWriteAccess = std::vector<Path>();

			auto exception = Assert::Throws<std::runtime_error>([&]()
			{
				auto ranOperations = uut.Evaluate(
					operationGraph,
					operationResults,
					temporaryDirectory,
					globalAllowedReadAccess,
					globalAllowedWriteAccess);
			});

			Assert::AreEqual("File \"C:/TestWorkingDirectory/OutputFile.out\" already written to by operation \"TestCommand: 2\"", exception.what(), "Verify Exception message");

			// Verify operation results
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(),
				operationResults.GetResults(),
				"Verify operation results match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build evaluation start",
					"DIAG: Check for previous operation invocation",
					"INFO: Operation has no successful previous invocation",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: [C:/TestWorkingDirectory/] ./Command1.exe Arguments",
					"DIAG: Allowed Read Access:",
					"DIAG: Allowed Write Access:",
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
					"TryGetLastWriteTime: C:/TestWorkingDirectory/OutputFile.out",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"CreateDetourProcess: 1 [C:/TestWorkingDirectory/] ./Command1.exe Arguments Environment [2] 1 AllowedRead [0] AllowedWrite [0]",
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
