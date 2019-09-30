#pragma once
#include "/Package/RecipeTests.h"

TestState RunRecipeTests() 
 {
	auto className = "RecipeTests";
	auto testClass = std::make_shared<Soup::UnitTests::RecipeTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "InitializerDefault", [&testClass]() { testClass->InitializerDefault(); });
	state += SoupTest::RunTest(className, "InitializerAll", [&testClass]() { testClass->InitializerAll(); });
	state += SoupTest::RunTest(className, "OperatorEqualDefault", [&testClass]() { testClass->OperatorEqualDefault(); });
	state += SoupTest::RunTest(className, "OperatorEqualAll", [&testClass]() { testClass->OperatorEqualAll(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualType", [&testClass]() { testClass->OperatorNotEqualType(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualNoType", [&testClass]() { testClass->OperatorNotEqualNoType(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualDependencies", [&testClass]() { testClass->OperatorNotEqualDependencies(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualNoDependencies", [&testClass]() { testClass->OperatorNotEqualNoDependencies(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualPublic", [&testClass]() { testClass->OperatorNotEqualPublic(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualNoPublic", [&testClass]() { testClass->OperatorNotEqualNoPublic(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualSource", [&testClass]() { testClass->OperatorNotEqualSource(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualNoSource", [&testClass]() { testClass->OperatorNotEqualNoSource(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualIncludePaths", [&testClass]() { testClass->OperatorNotEqualIncludePaths(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualNoIncludePaths", [&testClass]() { testClass->OperatorNotEqualNoIncludePaths(); });

	return state;
}