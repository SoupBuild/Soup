#pragma once
#include "/Package/RecipeBuildManagerTests.h"

TestState RunRecipeBuildManagerTests() 
 {
	auto className = "RecipeBuildManagerTests";
	auto testClass = std::make_shared<Soup::UnitTests::RecipeBuildManagerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += SoupTest::RunTest(className, "Initialize_NullCompilerThrows", [&testClass]() { testClass->Initialize_NullCompilerThrows(); });

	return state;
}