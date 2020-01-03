#pragma once
#include "Build/Tasks/StandardLibraryIncludeTaskTests.h"

TestState RunStandardLibraryIncludeTaskTests() 
 {
	auto className = "StandardLibraryIncludeTaskTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::StandardLibraryIncludeTaskTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialze_Success", [&testClass]() { testClass->Initialze_Success(); });
	state += SoupTest::RunTest(className, "Execute", [&testClass]() { testClass->Execute(); });

	return state;
}