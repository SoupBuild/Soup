#pragma once
#include "Package/RecipeBuilderTests.h"

TestState RunRecipeBuilderTests() 
 {
	auto className = "RecipeBuilderTests";
	auto testClass = std::make_shared<Soup::UnitTests::RecipeBuilderTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += SoupTest::RunTest(className, "Initialize_NullRuntimeCompilerThrows", [&testClass]() { testClass->Initialize_NullRuntimeCompilerThrows(); });
	state += SoupTest::RunTest(className, "Initialize_NullSystemCompilerThrows", [&testClass]() { testClass->Initialize_NullSystemCompilerThrows(); });

	return state;
}