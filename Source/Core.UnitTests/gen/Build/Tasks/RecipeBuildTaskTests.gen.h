#pragma once
#include "Build/Tasks/RecipeBuildTaskTests.h"

TestState RunRecipeBuildTaskTests() 
 {
	auto className = "RecipeBuildTaskTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::RecipeBuildTaskTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialze_Success", [&testClass]() { testClass->Initialze_Success(); });
	state += SoupTest::RunTest(className, "Initialze_NullCompilerThrows", [&testClass]() { testClass->Initialze_NullCompilerThrows(); });
	state += SoupTest::RunTest(className, "Build_Executable", [&testClass]() { testClass->Build_Executable(); });

	return state;
}