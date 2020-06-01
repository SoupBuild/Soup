#pragma once
#include "Build/Runner/BuildRunnerTests.h"

TestState RunBuildRunnerTests() 
 {
	auto className = "BuildRunnerTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::BuildRunnerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += SoupTest::RunTest(className, "Execute_NoOperations_ForceBuild", [&testClass]() { testClass->Execute_NoOperations_ForceBuild(); });
	state += SoupTest::RunTest(className, "Execute_NoOperations_Incremental", [&testClass]() { testClass->Execute_NoOperations_Incremental(); });
	state += SoupTest::RunTest(className, "Execute_OneOperation_ForceBuild", [&testClass]() { testClass->Execute_OneOperation_ForceBuild(); });
	state += SoupTest::RunTest(className, "Execute_OneOperation_Incremental_NoBuildHistory", [&testClass]() { testClass->Execute_OneOperation_Incremental_NoBuildHistory(); });
	state += SoupTest::RunTest(className, "Execute_OneOperation_Incremental_MissingFileInfo", [&testClass]() { testClass->Execute_OneOperation_Incremental_MissingFileInfo(); });
	state += SoupTest::RunTest(className, "Execute_OneOperation_Incremental_MissingTargetFile", [&testClass]() { testClass->Execute_OneOperation_Incremental_MissingTargetFile(); });
	state += SoupTest::RunTest(className, "Execute_OneOperation_Incremental_OutOfDate", [&testClass]() { testClass->Execute_OneOperation_Incremental_OutOfDate(); });
	state += SoupTest::RunTest(className, "Execute_OneOperation_Incremental_UpToDate", [&testClass]() { testClass->Execute_OneOperation_Incremental_UpToDate(); });

	return state;
}