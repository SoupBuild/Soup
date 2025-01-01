#pragma once
#include "operation-graph/OperationGraphTests.h"

TestState RunOperationGraphTests() 
 {
	auto className = "OperationGraphTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::OperationGraphTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Default", [&testClass]() { testClass->Initialize_Default(); });
	state += Soup::Test::RunTest(className, "Initialize_ListOperations_Single", [&testClass]() { testClass->Initialize_ListOperations_Single(); });
	state += Soup::Test::RunTest(className, "SetRootOperationIds", [&testClass]() { testClass->SetRootOperationIds(); });
	state += Soup::Test::RunTest(className, "GetOperationInfo_MissingThrows", [&testClass]() { testClass->GetOperationInfo_MissingThrows(); });
	state += Soup::Test::RunTest(className, "GetOperationInfo_Found", [&testClass]() { testClass->GetOperationInfo_Found(); });
	state += Soup::Test::RunTest(className, "AddOperation", [&testClass]() { testClass->AddOperation(); });

	return state;
}