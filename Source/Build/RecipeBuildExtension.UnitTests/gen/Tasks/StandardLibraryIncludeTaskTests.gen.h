#pragma once
#include "Tasks/StandardLibraryIncludeTaskTests.h"

TestState RunStandardLibraryIncludeTaskTests() 
 {
	auto className = "StandardLibraryIncludeTaskTests";
	auto testClass = std::make_shared<RecipeBuild::UnitTests::StandardLibraryIncludeTaskTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += SoupTest::RunTest(className, "Execute", [&testClass]() { testClass->Execute(); });

	return state;
}