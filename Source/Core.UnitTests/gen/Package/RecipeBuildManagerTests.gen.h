#pragma once
#include "/Package/RecipeBuildManagerTests.h"

TestState RunRecipeBuildManagerTests() 
 {
	auto className = "RecipeBuildManagerTests";
	auto testClass = std::make_shared<Soup::UnitTests::RecipeBuildManagerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += SoupTest::RunTest(className, "Initialize_NullSystemCompilerThrows", [&testClass]() { testClass->Initialize_NullSystemCompilerThrows(); });
	state += SoupTest::RunTest(className, "Initialize_NullRuntimeCompilerThrows", [&testClass]() { testClass->Initialize_NullRuntimeCompilerThrows(); });

	return state;
}