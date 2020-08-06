#pragma once
#include "BuildHistoryJsonTests.h"

TestState RunBuildHistoryJsonTests() 
{
	auto className = "BuildHistoryJsonTests";
	auto testClass = std::make_shared<Soup::Build::Execute::UnitTests::BuildHistoryJsonTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Deserialize_GarbageThrows", [&testClass]() { testClass->Deserialize_GarbageThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingOperationsThrows", [&testClass]() { testClass->Deserialize_MissingOperations(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingCommandThrows", [&testClass]() { testClass->Deserialize_MissingCommandThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingInputThrows", [&testClass]() { testClass->Deserialize_MissingInputThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingOutputThrows", [&testClass]() { testClass->Deserialize_MissingOutputThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_Simple", [&testClass]() { testClass->Deserialize_Simple(); });
	state += SoupTest::RunTest(className, "Deserialize_Multiple", [&testClass]() { testClass->Deserialize_Multiple(); });
	state += SoupTest::RunTest(className, "Serialize_Simple", [&testClass]() { testClass->Serialize_Simple(); });
	state += SoupTest::RunTest(className, "Serialize_Multipl", [&testClass]() { testClass->Serialize_Multipl(); });

	return state;
}