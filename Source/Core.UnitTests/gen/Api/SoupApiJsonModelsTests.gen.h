#pragma once
#include "Api/SoupApiJsonModelsTests.h"

TestState RunSoupApiJsonModelsTests() 
{
	auto className = "SoupApiJsonModelsTests";
	auto testClass = std::make_shared<Soup::Api::UnitTests::SoupApiJsonModelsTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "ParsePackageResult_GarbageThrows", [&testClass]() { testClass->ParsePackageResult_GarbageThrows(); });
	state += SoupTest::RunTest(className, "ParsePackageResult_MissingNameThrows", [&testClass]() { testClass->ParsePackageResult_MissingNameThrows(); });
	state += SoupTest::RunTest(className, "ParsePackageResult_Simple", [&testClass]() { testClass->ParsePackageResult_Simple(); });
	state += SoupTest::RunTest(className, "ParsePackageResult_AllProperties", [&testClass]() { testClass->ParsePackageResult_AllProperties(); });

	return state;
}