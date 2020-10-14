#pragma once
#include "Config/LocalUserConfigJsonTests.h"

TestState RunLocalUserConfigJsonTests() 
 {
	auto className = "LocalUserConfigJsonTests";
	auto testClass = std::make_shared<Soup::UnitTests::LocalUserConfigJsonTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Deserialize_GarbageThrows", [&testClass]() { testClass->Deserialize_GarbageThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_Simple", [&testClass]() { testClass->Deserialize_Simple(); });
	state += Soup::Test::RunTest(className, "Deserialize_AllProperties", [&testClass]() { testClass->Deserialize_AllProperties(); });

	return state;
}