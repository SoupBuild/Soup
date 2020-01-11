// <copyright file="BuildHistoryCheckerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class BuildHistoryCheckerTests
	{
	public:
		[[Fact]]
		void IsOutdated_ZeroInput_Throws()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Setup the input parameters
			auto targetFiles = std::vector<Path>({
				Path("Output.bin"),
			});
			auto inputFiles = std::vector<Path>({});
			auto rootPath = Path("C:/Root/");

			// Perform the check
			auto uut = BuildHistoryChecker();
			Assert::ThrowsRuntimeError([&uut, &targetFiles, &inputFiles, &rootPath]() {
				bool result = uut.IsOutdated(targetFiles, inputFiles, rootPath);
			});

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({}),
					fileSystem->GetRequests(),
					"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_MissingTarget()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Setup the input parameters
			auto targetFiles = std::vector<Path>({
				Path("Output.bin"),
			});
			auto inputFiles = std::vector<Path>({
				Path("Input.cpp")
			});
			auto rootPath = Path("C:/Root/");

			// Perform the check
			auto uut = BuildHistoryChecker();
			bool result = uut.IsOutdated(targetFiles, inputFiles, rootPath);

			// Verify the results
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/Output.bin",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Output target does not exist: C:/Root/Output.bin",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_TargetExists_MissingInputFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			fileSystem->CreateMockFile(Path("C:/Root/Output.bin"), MockFileState(outputTime));

			// Setup the input parameters
			auto targetFiles = std::vector<Path>({
				Path("Output.bin"),
			});
			auto inputFiles = std::vector<Path>({
				Path("Input.cpp")
			});
			auto rootPath = Path("C:/Root/");

			// Perform the check
			auto uut = BuildHistoryChecker();
			bool result = uut.IsOutdated(targetFiles, inputFiles, rootPath);
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/Output.bin",
					"GetLastWriteTime: C:/Root/Output.bin",
					"Exists: C:/Root/Input.cpp",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: IsOutdated: C:/Root/Output.bin [1434993120]",
					"ERRO:   C:/Root/Input.cpp [MISSING]",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_TargetExists_Outdated()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 13);
			fileSystem->CreateMockFile(Path("C:/Root/Output.bin"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("C:/Root/Input.cpp"), MockFileState(inputTime));

			// Setup the input parameters
			auto targetFiles = std::vector<Path>({
				Path("Output.bin"),
			});
			auto inputFiles = std::vector<Path>({
				Path("Input.cpp"),
			});
			auto rootPath = Path("C:/Root/");

			// Perform the check
			auto uut = BuildHistoryChecker();
			bool result = uut.IsOutdated(targetFiles, inputFiles, rootPath);

			// Verify the results
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/Output.bin",
					"GetLastWriteTime: C:/Root/Output.bin",
					"Exists: C:/Root/Input.cpp",
					"GetLastWriteTime: C:/Root/Input.cpp",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: IsOutdated: C:/Root/Output.bin [1434993120]",
					"DIAG:   C:/Root/Input.cpp [1434993180]",
					"INFO: Input altered after target [C:/Root/Input.cpp] -> [C:/Root/Output.bin]",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_TargetExists_UpToDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(Path("C:/Root/Output.bin"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("C:/Root/Input.cpp"), MockFileState(inputTime));

			// Setup the input parameters
			auto targetFiles = std::vector<Path>({
				Path("Output.bin"),
			});
			auto inputFiles = std::vector<Path>({
				Path("Input.cpp"),
			});
			auto rootPath = Path("C:/Root/");

			// Perform the check
			auto uut = BuildHistoryChecker();
			bool result = uut.IsOutdated(targetFiles, inputFiles, rootPath);

			// Verify the results
			Assert::IsFalse(result, "Verify the result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/Output.bin",
					"GetLastWriteTime: C:/Root/Output.bin",
					"Exists: C:/Root/Input.cpp",
					"GetLastWriteTime: C:/Root/Input.cpp",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: IsOutdated: C:/Root/Output.bin [1434993120]",
					"DIAG:   C:/Root/Input.cpp [1434993060]",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_MultipleInputs_RelativeAndAbsolute()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(Path("C:/Root/Output.bin"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("C:/Root/Input.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("C:/Input.h"), MockFileState(inputTime));

			// Setup the input parameters
			auto targetFiles = std::vector<Path>({
				Path("Output.bin"),
			});
			auto inputFiles = std::vector<Path>({
				Path("Input.cpp"),
				Path("C:/Input.h"),
			});
			auto rootPath = Path("C:/Root/");

			// Perform the check
			auto uut = BuildHistoryChecker();
			bool result = uut.IsOutdated(targetFiles, inputFiles, rootPath);

			// Verify the results
			Assert::IsFalse(result, "Verify the result is false.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/Output.bin",
					"GetLastWriteTime: C:/Root/Output.bin",
					"Exists: C:/Root/Input.cpp",
					"GetLastWriteTime: C:/Root/Input.cpp",
					"Exists: C:/Input.h",
					"GetLastWriteTime: C:/Input.h",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: IsOutdated: C:/Root/Output.bin [1434993120]",
					"DIAG:   C:/Root/Input.cpp [1434993060]",
					"DIAG:   C:/Input.h [1434993060]",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}
	};
}
