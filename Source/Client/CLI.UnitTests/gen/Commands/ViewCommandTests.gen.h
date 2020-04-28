#pragma once
#include "Commands/ViewCommandTests.h"

TestState RunViewCommandTests() 
 {
	auto className = "ViewCommandTests";
	auto testClass = std::make_shared<Soup::Client::UnitTests::ViewCommandTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

	return state;
}