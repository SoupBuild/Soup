#pragma once
#include "Build/BuildEvaluateEngineTests.h"

TestState RunBuildEvaluateEngineTests() 
 {
	auto className = "BuildEvaluateEngineTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::BuildEvaluateEngineTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_FirstRun", [&testClass]() { testClass->Execute_OneOperation_FirstRun(); });
	state += Soup::Test::RunTest(className, "Execute_OneOperation_ObservedCircularReference_Fails", [&testClass]() { testClass->Execute_OneOperation_ObservedCircularReference_Fails(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_MissingFileInfo", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_MissingFileInfo(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_MissingTargetFile", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_MissingTargetFile(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_OutOfDate", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_OutOfDate(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_Executable_OutOfDate", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_Executable_OutOfDate(); });
	state += Soup::Test::RunTest(className, "Evaluate_OneOperation_Incremental_UpToDate", [&testClass]() { testClass->Evaluate_OneOperation_Incremental_UpToDate(); });
	state += Soup::Test::RunTest(className, "Execute_TwoOperations_DuplicateOutputFile_Fails", [&testClass]() { testClass->Execute_TwoOperations_DuplicateOutputFile_Fails(); });
	state += Soup::Test::RunTest(className, "Execute_TwoOperations_UndeclaredOutputWithDeclaredInput_Fails", [&testClass]() { testClass->Execute_TwoOperations_UndeclaredOutputWithDeclaredInput_Fails(); });

	return state;
}