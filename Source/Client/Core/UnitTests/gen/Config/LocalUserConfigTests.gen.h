#pragma once
#include "Config/LocalUserConfigTests.h"

TestState RunLocalUserConfigTests() 
 {
	auto className = "LocalUserConfigTests";
	auto testClass = std::make_shared<Soup::UnitTests::LocalUserConfigTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "InitializerDefault", [&testClass]() { testClass->InitializerDefault(); });
	state += Soup::Test::RunTest(className, "InitializerAll", [&testClass]() { testClass->InitializerAll(); });

	return state;
}