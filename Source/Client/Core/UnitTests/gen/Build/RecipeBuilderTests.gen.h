#pragma once
#include "Build/RecipeBuilderTests.h"

TestState RunRecipeBuilderTests() 
 {
	auto className = "RecipeBuilderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::RecipeBuilderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += Soup::Test::RunTest(className, "Initialize_NullRuntimeCompilerThrows", [&testClass]() { testClass->Initialize_NullRuntimeCompilerThrows(); });
	state += Soup::Test::RunTest(className, "Initialize_NullSystemCompilerThrows", [&testClass]() { testClass->Initialize_NullSystemCompilerThrows(); });

	return state;
}