#pragma once
#include "/Commands/PackCommandTests.h"

TestState RunPackCommandTests() 
 {
	auto className = "PackCommandTests";
	auto testClass = std::make_shared<Soup::Client::UnitTests::PackCommandTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

	return state;
}