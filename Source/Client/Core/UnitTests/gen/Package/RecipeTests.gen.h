#pragma once
#include "Package/RecipeTests.h"

TestState RunRecipeTests() 
{
	auto className = "RecipeTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::RecipeTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "InitializerDefault", [&testClass]() { testClass->InitializerDefault(); });
	state += Soup::Test::RunTest(className, "InitializerAll", [&testClass]() { testClass->InitializerAll(); });
	state += Soup::Test::RunTest(className, "OperatorEqualDefault", [&testClass]() { testClass->OperatorEqualDefault(); });
	state += Soup::Test::RunTest(className, "OperatorEqualAll", [&testClass]() { testClass->OperatorEqualAll(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualLanguage", [&testClass]() { testClass->OperatorNotEqualLanguage(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualNoVersion", [&testClass]() { testClass->OperatorNotEqualNoVersion(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualDependencies", [&testClass]() { testClass->OperatorNotEqualDependencies(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualNoDependencies", [&testClass]() { testClass->OperatorNotEqualNoDependencies(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualDevDependencies", [&testClass]() { testClass->OperatorNotEqualDevDependencies(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualNoDevDependencies", [&testClass]() { testClass->OperatorNotEqualNoDevDependencies(); });

	return state;
}