// <copyright file="RecipeBuildTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class RecipeBuildTaskTests
	{
	public:
		[[Fact]]
		void Initialze_Success()
		{
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = RecipeBuildTask(compiler, compiler);
			
			Assert::AreEqual("RecipeBuild", uut.GetName(), "Verify name matches expected.");
		}

		[[Fact]]
		void Initialze_NullCompilerThrows()
		{
			auto compiler = nullptr;
			Assert::ThrowsRuntimeError([&compiler]() {
				auto uut = RecipeBuildTask(compiler, compiler);
			});
		}

		[[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			fileSystem->CreateMockFile(
				Path("C:/PackageRoot/Recipe.json"),
				MockFileState(std::stringstream(R"({
					"name": "MyPackage",
					"version": "1.2.3"
				})")));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = RecipeBuildTask(compiler, compiler);

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);
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

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>(),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>(),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>();

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());

			// TODO: Verify build state
		}
	};
}
