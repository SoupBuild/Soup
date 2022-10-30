#pragma once
#include "OperationGraph/OperationResultsTests.h"

TestState RunOperationResultsTests() 
 {
	auto className = "OperationResultsTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::OperationResultsTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Default", [&testClass]() { testClass->Initialize_Default(); });
	state += Soup::Test::RunTest(className, "Initialize_ListOperations_Single", [&testClass]() { testClass->Initialize_ListOperations_Single(); });
	state += Soup::Test::RunTest(className, "TryFindResult_Missing", [&testClass]() { testClass->TryFindResult_Missing(); });
	state += Soup::Test::RunTest(className, "TryFindResult_Found", [&testClass]() { testClass->TryFindResult_Found(); });
	state += Soup::Test::RunTest(className, "AddOrUpdateOperationResult", [&testClass]() { testClass->AddOrUpdateOperationResult(); });

	return state;
}