#pragma once
#include "/Package/RecipeManagerTests.h"

TestState RunRecipeManagerTests() 
 {
    auto className = "RecipeManagerTests";
    auto testClass = std::make_shared<Soup::UnitTests::RecipeManagerTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Parse_GarbageThrows", [&testClass]() { testClass->Parse_GarbageThrows(); });
    state += SoupTest::RunTest(className, "Parse_MissingNameThrows", [&testClass]() { testClass->Parse_MissingNameThrows(); });
    state += SoupTest::RunTest(className, "Parse_MissingVersionThrows", [&testClass]() { testClass->Parse_MissingVersionThrows(); });
    state += SoupTest::RunTest(className, "ParseSimple", [&testClass]() { testClass->ParseSimple(); });
    state += SoupTest::RunTest(className, "Parse_AllProperties", [&testClass]() { testClass->Parse_AllProperties(); });

    return state;
}