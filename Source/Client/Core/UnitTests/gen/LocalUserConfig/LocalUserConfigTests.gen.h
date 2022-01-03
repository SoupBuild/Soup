#pragma once
#include "LocalUserConfig/LocalUserConfigTests.h"

TestState RunLocalUserConfigTests() 
{
	auto className = "LocalUserConfigTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::LocalUserConfigTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "InitializerDefault", [&testClass]() { testClass->InitializerDefault(); });
	state += Soup::Test::RunTest(className, "OperatorEqualDefault", [&testClass]() { testClass->OperatorEqualDefault(); });

	return state;
}