#pragma once
#include "Package/RecipeBuilderTests.h"

TestState RunRecipeBuilderTests() 
 {
	auto className = "RecipeBuilderTests";
	auto testClass = std::make_shared<Soup::UnitTests::RecipeBuilderTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += SoupTest::RunTest(className, "Initialize_NullCompilerThrows", [&testClass]() { testClass->Initialize_NullCompilerThrows(); });

	return state;
}