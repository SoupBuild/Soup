#pragma once
#include "Recipe/RecipeTests.h"

TestState RunRecipeTests() 
{
	auto className = "RecipeTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::RecipeTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "InitializerDefault", [&testClass]() { testClass->InitializerDefault(); });
	state += Soup::Test::RunTest(className, "InitializerAll", [&testClass]() { testClass->InitializerAll(); });
	state += Soup::Test::RunTest(className, "OperatorEqualDefault", [&testClass]() { testClass->OperatorEqualDefault(); });
	state += Soup::Test::RunTest(className, "OperatorEqualAll", [&testClass]() { testClass->OperatorEqualAll(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualLanguage", [&testClass]() { testClass->OperatorNotEqualLanguage(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualNoVersion", [&testClass]() { testClass->OperatorNotEqualNoVersion(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualRuntimeDependencies", [&testClass]() { testClass->OperatorNotEqualRuntimeDependencies(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualNoRuntimeDependencies", [&testClass]() { testClass->OperatorNotEqualNoRuntimeDependencies(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualBuildDependencies", [&testClass]() { testClass->OperatorNotEqualBuildDependencies(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualNoBuildDependencies", [&testClass]() { testClass->OperatorNotEqualNoBuildDependencies(); });

	return state;
}