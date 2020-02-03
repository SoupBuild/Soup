#pragma once
#include "Tasks/RecipeBuildTaskTests.h"

TestState RunRecipeBuildTaskTests() 
 {
	auto className = "RecipeBuildTaskTests";
	auto testClass = std::make_shared<RecipeBuild::UnitTests::RecipeBuildTaskTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += SoupTest::RunTest(className, "Build_Executable", [&testClass]() { testClass->Build_Executable(); });

	return state;
}