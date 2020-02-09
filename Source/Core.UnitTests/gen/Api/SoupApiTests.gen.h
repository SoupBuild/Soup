#pragma once
#include "Api/SoupApiTests.h"

TestState RunSoupApiTests() 
 {
	auto className = "SoupApiTests";
	auto testClass = std::make_shared<Soup::Api::UnitTests::SoupApiTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });

	return state;
}