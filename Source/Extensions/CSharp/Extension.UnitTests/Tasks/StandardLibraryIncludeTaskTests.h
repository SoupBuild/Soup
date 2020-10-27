// <copyright file="StandardLibraryIncludeTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::UnitTests
{
	class StandardLibraryIncludeTaskTests
	{
	public:
		// [[Fact]]
		void Initialize_Success()
		{
			auto uut = StandardLibraryIncludeTask();
			
			Assert::AreEqual("StandardLibraryInclude", uut.GetName(), "Verify name matches expected.");
		}

		// [[Fact]]
		void Execute()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = Opal::ScopedTraceListenerRegister(testListener);

			auto uut = StandardLibraryIncludeTask();

			// Setup the input build state
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>();

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));

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
	};
}
