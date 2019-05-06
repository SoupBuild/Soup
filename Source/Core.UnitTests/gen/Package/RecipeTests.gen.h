#pragma once
#include "/Package/RecipeTests.h"

TestState RunRecipeTests() 
 {
    auto className = "RecipeTests";
    auto testClass = std::make_shared<Soup::UnitTests::RecipeTests>();
    TestState state = { 0, 0 };
    state += RunTest(className, "InitializerDefault", [&testClass]() { testClass->InitializerDefault(); });
    state += RunTest(className, "InitializerAll", [&testClass]() { testClass->InitializerAll(); });
    state += RunTest(className, "OperatorEqualDefault", [&testClass]() { testClass->OperatorEqualDefault(); });
    state += RunTest(className, "OperatorEqualAll", [&testClass]() { testClass->OperatorEqualAll(); });
    state += RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
    state += RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
    state += RunTest(className, "OperatorNotEqualType", [&testClass]() { testClass->OperatorNotEqualType(); });
    state += RunTest(className, "OperatorNotEqualNoType", [&testClass]() { testClass->OperatorNotEqualNoType(); });
    state += RunTest(className, "OperatorNotEqualDependencies", [&testClass]() { testClass->OperatorNotEqualDependencies(); });
    state += RunTest(className, "OperatorNotEqualNoDependencies", [&testClass]() { testClass->OperatorNotEqualNoDependencies(); });
    state += RunTest(className, "OperatorNotEqualPublic", [&testClass]() { testClass->OperatorNotEqualPublic(); });
    state += RunTest(className, "OperatorNotEqualNoPublic", [&testClass]() { testClass->OperatorNotEqualNoPublic(); });
    state += RunTest(className, "OperatorNotEqualSource", [&testClass]() { testClass->OperatorNotEqualSource(); });
    state += RunTest(className, "OperatorNotEqualNoSource", [&testClass]() { testClass->OperatorNotEqualNoSource(); });

    return state;
}