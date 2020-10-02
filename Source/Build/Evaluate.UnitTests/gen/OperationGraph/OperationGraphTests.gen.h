#pragma once
#include "OperationGraph/OperationGraphTests.h"

TestState RunOperationGraphTests() 
 {
	auto className = "OperationGraphTests";
	auto testClass = std::make_shared<Soup::Build::Evaluate::UnitTests::OperationGraphTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Default", [&testClass]() { testClass->Initialize_Default(); });
	state += Soup::Test::RunTest(className, "Initialize_ListOperations_Single", [&testClass]() { testClass->Initialize_ListOperations_Single(); });
	state += Soup::Test::RunTest(className, "SetRootOperationIds", [&testClass]() { testClass->SetRootOperationIds(); });
	state += Soup::Test::RunTest(className, "TryFindOperationInfo_Missing", [&testClass]() { testClass->TryFindOperationInfo_Missing(); });
	state += Soup::Test::RunTest(className, "TryFindOperationInfo_Found", [&testClass]() { testClass->TryFindOperationInfo_Found(); });
	state += Soup::Test::RunTest(className, "GetOperationInfo_MissingThrows", [&testClass]() { testClass->GetOperationInfo_MissingThrows(); });
	state += Soup::Test::RunTest(className, "GetOperationInfo_Found", [&testClass]() { testClass->GetOperationInfo_Found(); });
	state += Soup::Test::RunTest(className, "AddOperationInfo", [&testClass]() { testClass->AddOperationInfo(); });

	return state;
}