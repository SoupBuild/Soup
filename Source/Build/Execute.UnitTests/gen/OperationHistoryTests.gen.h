#pragma once
#include "OperationHistoryTests.h"

TestState RunOperationHistoryTests() 
 {
	auto className = "OperationHistoryTests";
	auto testClass = std::make_shared<Soup::Build::Execute::UnitTests::OperationHistoryTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Default", [&testClass]() { testClass->Initialize_Default(); });
	state += Soup::Test::RunTest(className, "Initialize_ListOperations_Single", [&testClass]() { testClass->Initialize_ListOperations_Single(); });
	state += Soup::Test::RunTest(className, "TryFindOperationInfo_Missing", [&testClass]() { testClass->TryFindOperationInfo_Missing(); });
	state += Soup::Test::RunTest(className, "TryFindOperationInfo_Found", [&testClass]() { testClass->TryFindOperationInfo_Found(); });
	state += Soup::Test::RunTest(className, "AddOperationInfo", [&testClass]() { testClass->AddOperationInfo(); });

	return state;
}