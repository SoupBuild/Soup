// <copyright file="RecipeBuildTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace RecipeBuild::UnitTests
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
			auto testListener = std::make_shared<Soup::TestTraceListener>();
			auto scopedTraceListener = Soup::ScopedTraceListenerRegister(testListener);

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
			auto buildState = Soup::Build::BuildState();
			auto state = Soup::Build::PropertyBagWrapper(buildState.GetActiveState());
			state.SetPropertyStringValue("PackageRoot", "C:/PackageRoot/");
			state.SetPropertyBooleanValue("ForceRebuild", false);
			state.SetPropertyStringValue("BuildFlavor", "debug");
			state.SetPropertyStringList("PlatformLibraries", std::vector<std::string>());
			state.SetPropertyStringList("PlatformIncludePaths", std::vector<std::string>());
			state.SetPropertyStringList("PlatformLibraryPaths", std::vector<std::string>());
			state.SetPropertyStringList("PlatformPreprocessorDefinitions", std::vector<std::string>());

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify build state
			auto expectedBuildNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>();

			Soup::AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());

			// TODO: Verify build state
		}
	};
}
