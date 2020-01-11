#pragma once
#include "Tasks/ResolveToolsTaskTests.h"

TestState RunResolveToolsTaskTests() 
 {
	auto className = "ResolveToolsTaskTests";
	auto testClass = std::make_shared<RecipeBuild::UnitTests::ResolveToolsTaskTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialze_Success", [&testClass]() { testClass->Initialze_Success(); });
	state += SoupTest::RunTest(className, "Execute", [&testClass]() { testClass->Execute(); });

	return state;
}