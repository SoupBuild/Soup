#pragma once
#include "Build/BuildEngineTests.h"

TestState RunBuildEngineTests() 
 {
	auto className = "RecipeBuilderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::BuildEngineTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Execute_NoDependencies", [&testClass]() { testClass->Execute_NoDependencies(); });

	return state;
}