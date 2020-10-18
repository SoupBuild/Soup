// <copyright file="RecipeBuildTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::UnitTests
{
	class RecipeBuildTaskTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto uut = RecipeBuildTask();
			
			Assert::AreEqual("RecipeBuild", uut.GetName(), "Verify name matches expected.");
		}

		[[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = Opal::ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("C:/PackageRoot/Recipe.json"),
				std::make_shared<MockFile>(std::stringstream(R"({
					"name": "MyPackage",
					"version": "1.2.3"
				})")));

			auto uut = RecipeBuildTask();

			// Setup the input build state
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("PackageRoot").SetValueString("C:/PackageRoot/");
			state.CreateValue("BuildFlavor").SetValueString("debug");
			state.CreateValue("PlatformLibraries").SetValueStringList(std::vector<std::string>());
			state.CreateValue("PlatformLibraryPaths").SetValueStringList(std::vector<std::string>());
			state.CreateValue("PlatformPreprocessorDefinitions").SetValueStringList(std::vector<std::string>());

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>();

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));

			// TODO: Verify build state
		}
	};
}
