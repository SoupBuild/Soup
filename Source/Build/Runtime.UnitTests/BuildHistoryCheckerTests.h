// <copyright file="BuildHistoryCheckerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Helpers.h"

namespace Soup::Build::Runtime::UnitTests
{
	class BuildHistoryCheckerTests
	{
	public:
		[[Fact]]
		void IsOutdated_ZeroInput()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");
			
			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			fileSystemState.SetLastWriteTime(outputFileId, std::nullopt);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);

			// Verify the results
			Assert::IsFalse(result, "Verify the result is false.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_UnknownTarget()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");

			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			auto inputFileId = fileSystemState.ToFileId(Path("Input.cpp"), rootPath);
			fileSystemState.SetLastWriteTime(inputFileId, std::nullopt);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({
				inputFileId,
			});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);

			// Verify the results
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_DeletedTarget()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");

			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			fileSystemState.SetLastWriteTime(outputFileId, std::nullopt);
			auto inputFileId = fileSystemState.ToFileId(Path("Input.cpp"), rootPath);
			fileSystemState.SetLastWriteTime(inputFileId, std::nullopt);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({
				inputFileId,
			});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);

			// Verify the results
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Output target does not exist: C:/Root/Output.bin",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_TargetExists_UnknownInputFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");

			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);
			auto inputFileId = fileSystemState.ToFileId(Path("Input.cpp"), rootPath);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({
				inputFileId,
			});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_SingleInput_TargetExists_DeletedInputFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");

			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);
			auto inputFileId = fileSystemState.ToFileId(Path("Input.cpp"), rootPath);
			fileSystemState.SetLastWriteTime(inputFileId, std::nullopt);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({
				inputFileId,
			});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Input Missing [C:/Root/Input.cpp]",
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

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 13);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");

			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);
			auto inputFileId = fileSystemState.ToFileId(Path("Input.cpp"), rootPath);
			fileSystemState.SetLastWriteTime(inputFileId, inputTime);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({
				inputFileId,
			});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);

			// Verify the results
			Assert::IsTrue(result, "Verify the result is true.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
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

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");

			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);
			auto inputFileId = fileSystemState.ToFileId(Path("Input.cpp"), rootPath);
			fileSystemState.SetLastWriteTime(inputFileId, inputTime);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({
				inputFileId,
			});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);

			// Verify the results
			Assert::IsFalse(result, "Verify the result is false.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void IsOutdated_MultipleInputs_RelativeAndAbsolute()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Create the file state
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);

			// Initialize the file system state
			auto fileSystemState = FileSystemState(1234);

			// Setup the input parameters
			auto rootPath = Path("C:/Root/");

			auto outputFileId = fileSystemState.ToFileId(Path("Output.bin"), rootPath);
			fileSystemState.SetLastWriteTime(outputFileId, outputTime);
			auto inputFile1Id = fileSystemState.ToFileId(Path("Input.cpp"), rootPath);
			fileSystemState.SetLastWriteTime(inputFile1Id, inputTime);
			auto inputFile2Id = fileSystemState.ToFileId(Path("C:/Input.h"), rootPath);
			fileSystemState.SetLastWriteTime(inputFile2Id, inputTime);

			auto targetFiles = std::vector<FileId>({
				outputFileId,
			});
			auto inputFiles = std::vector<FileId>({
				inputFile1Id,
				inputFile2Id,
			});

			// Perform the check
			auto uut = BuildHistoryChecker(fileSystemState);
			bool result = uut.IsOutdated(targetFiles, inputFiles);

			// Verify the results
			Assert::IsFalse(result, "Verify the result is false.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}
	};
}
