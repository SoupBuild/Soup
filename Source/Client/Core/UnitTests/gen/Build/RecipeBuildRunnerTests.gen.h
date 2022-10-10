#pragma once
#include "Build/RecipeBuildRunnerTests.h"

TestState RunRecipeBuildRunnerTests() 
 {
	auto className = "RecipeBuilderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::RecipeBuildRunnerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });

	return state;
}