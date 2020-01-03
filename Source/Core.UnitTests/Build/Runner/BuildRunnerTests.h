// <copyright file="BuildRunnerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
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
		void Execute_NoNodes_ForceBuild()
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
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>();
			auto forceBuild = true;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
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
		void Execute_NoNodes_Incremental()
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
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>();
			auto forceBuild = false;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"INFO: BuildHistory file does not exist",
					"INFO: No previous state found, full rebuild required",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup/BuildHistory.json",
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
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
		void Execute_OneNode_ForceBuild()
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
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
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
			});
			bool forceBuild = true;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"HIGH: TestCommand: 1",
					"DIAG: Execute: Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Execute: [C:/TestWorkingDirectory/] Command.exe Arguments",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneNode_Incremental_NoBuildHistory()
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
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
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
			});
			auto forceBuild = false;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"INFO: BuildHistory file does not exist",
					"INFO: No previous state found, full rebuild required",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup/BuildHistory.json",
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Execute: [C:/TestWorkingDirectory/] Command.exe Arguments",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}

		[[Fact]]
		void Execute_OneNode_Incremental_MissingFileInfo()
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
			auto initialBuildHistory = BuildHistory();
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/.soup/BuildHistory.json"),
				MockFileState(std::move(initialBuildHistoryJson)));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
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
			});
			auto forceBuild = false;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"INFO: Missing file info: InputFile.cpp",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/.soup/BuildHistory.json",
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Execute_OneNode_Incremental_MissingTargetFile()
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
				FileInfo(Path("InputFile.in"), { }),
			});
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/.soup/BuildHistory.json"),
				MockFileState(std::move(initialBuildHistoryJson)));

			// Setup the input file only
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(Path("C:/TestWorkingDirectory/InputFile.in"), MockFileState(inputTime));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
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
			});
			auto forceBuild = false;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"INFO: Output target does not exist: C:/TestWorkingDirectory/OutputFile.out",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/.soup/BuildHistory.json",
					"Exists: C:/TestWorkingDirectory/OutputFile.out",
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Execute_OneNode_Incremental_OutOfDate()
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
				FileInfo(Path("InputFile.in"), { }),
			});
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/.soup/BuildHistory.json"),
				MockFileState(std::move(initialBuildHistoryJson)));

			// Setup the input/output files to be out of date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 10);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(Path("Command.exe"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("C:/TestWorkingDirectory/OutputFile.out"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("C:/TestWorkingDirectory/InputFile.in"), MockFileState(inputTime));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
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
			});
			auto forceBuild = false;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"DIAG: IsOutdated: C:/TestWorkingDirectory/OutputFile.out [1434993000]",
					"DIAG:   C:/TestWorkingDirectory/InputFile.in [1434993060]",
					"INFO: Input altered after target [C:/TestWorkingDirectory/InputFile.in] -> [C:/TestWorkingDirectory/OutputFile.out]",
					"HIGH: TestCommand: 1",
					"DIAG: Execute: Command.exe Arguments",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/.soup/BuildHistory.json",
					"Exists: C:/TestWorkingDirectory/OutputFile.out",
					"GetLastWriteTime: C:/TestWorkingDirectory/OutputFile.out",
					"Exists: C:/TestWorkingDirectory/InputFile.in",
					"GetLastWriteTime: C:/TestWorkingDirectory/InputFile.in",
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Execute_OneNode_Incremental_UpToDate()
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
				FileInfo(Path("InputFile.in"), { }),
			});
			std::stringstream initialBuildHistoryJson;
			BuildHistoryJson::Serialize(initialBuildHistory, initialBuildHistoryJson);
			fileSystem->CreateMockFile(
				Path("C:/BuildDirectory/.soup/BuildHistory.json"),
				MockFileState(std::move(initialBuildHistoryJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(Path("Command.exe"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("C:/TestWorkingDirectory/OutputFile.out"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("C:/TestWorkingDirectory/InputFile.in"), MockFileState(inputTime));

			auto uut = BuildRunner(Path("C:/BuildDirectory/"));

			// Setup the input build state
			auto nodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
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
			});
			auto forceBuild = false;
			uut.Execute(nodes, forceBuild);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Loading previous build state",
					"DIAG: Check for updated source",
					"DIAG: IsOutdated: C:/TestWorkingDirectory/OutputFile.out [1434993120]",
					"DIAG:   C:/TestWorkingDirectory/InputFile.in [1434993060]",
					"DIAG:   C:/TestWorkingDirectory/InputFile.in [1434993060]",
					"INFO: Up to date",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"HIGH: Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/BuildDirectory/.soup/BuildHistory.json",
					"OpenRead: C:/BuildDirectory/.soup/BuildHistory.json",
					"Exists: C:/TestWorkingDirectory/OutputFile.out",
					"GetLastWriteTime: C:/TestWorkingDirectory/OutputFile.out",
					"Exists: C:/TestWorkingDirectory/InputFile.in",
					"GetLastWriteTime: C:/TestWorkingDirectory/InputFile.in",
					"Exists: C:/BuildDirectory/.soup",
					"CreateDirectory: C:/BuildDirectory/.soup",
					"OpenWrite: C:/BuildDirectory/.soup/BuildHistory.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
