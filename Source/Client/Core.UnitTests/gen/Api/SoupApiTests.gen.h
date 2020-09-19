#pragma once
#include "Api/SoupApiTests.h"

TestState RunSoupApiTests() 
 {
	auto className = "SoupApiTests";
	auto testClass = std::make_shared<Soup::Api::UnitTests::SoupApiTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "GetPackage_NotFound", [&testClass]() { testClass->GetPackage_NotFound(); });
	state += Soup::Test::RunTest(className, "GetPackage_Success", [&testClass]() { testClass->GetPackage_Success(); });

	return state;
}