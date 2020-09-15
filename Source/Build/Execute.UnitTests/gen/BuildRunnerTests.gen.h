#pragma once
#include "BuildRunnerTests.h"

TestState RunBuildRunnerTests() 
 {
	auto className = "BuildRunnerTests";
	auto testClass = std::make_shared<Soup::Build::Execute::UnitTests::BuildRunnerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Execute_NoOperations_ForceBuild", [&testClass]() { testClass->Execute_NoOperations_ForceBuild(); });
	state += Soup::Test::RunTest(className, "Execute_NoOperations_Incremental", [&testClass]() { testClass->Execute_NoOperations_Incremental(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_ForceBuild", [&testClass]() { testClass->Execute_OneOperation_ForceBuild(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_Incremental_NoOperationHistory", [&testClass]() { testClass->Execute_OneOperation_Incremental_NoOperationHistory(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_Incremental_MissingFileInfo", [&testClass]() { testClass->Execute_OneOperation_Incremental_MissingFileInfo(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_Incremental_MissingTargetFile", [&testClass]() { testClass->Execute_OneOperation_Incremental_MissingTargetFile(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_Incremental_OutOfDate", [&testClass]() { testClass->Execute_OneOperation_Incremental_OutOfDate(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_Incremental_UpToDate", [&testClass]() { testClass->Execute_OneOperation_Incremental_UpToDate(); });

	return state;
}