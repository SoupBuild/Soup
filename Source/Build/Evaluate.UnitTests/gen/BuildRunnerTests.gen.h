#pragma once
#include "BuildRunnerTests.h"

TestState RunBuildRunnerTests() 
 {
	auto className = "BuildRunnerTests";
	auto testClass = std::make_shared<Soup::Build::Evaluate::UnitTests::BuildRunnerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_FirstRun", [&testClass]() { testClass->Execute_OneOperation_FirstRun(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_MissingFileInfo", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_MissingFileInfo(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_MissingTargetFile", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_MissingTargetFile(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_OutOfDate", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_OutOfDate(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_UpToDate", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_UpToDate(); });

	return state;
}