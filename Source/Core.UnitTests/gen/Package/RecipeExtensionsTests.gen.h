#pragma once
#include "/Package/RecipeExtensionsTests.h"

TestState RunRecipeExtensionsTests() 
 {
    auto className = "RecipeExtensionsTests";
    auto testClass = std::make_shared<Soup::UnitTests::RecipeExtensionsTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "TryLoadFromDirectory_MissingFile", [&testClass]() { testClass->TryLoadFromDirectory_MissingFile(); });
    state += SoupTest::RunTest(className, "TryLoadFromDirectory_GarbageFile", [&testClass]() { testClass->TryLoadFromDirectory_GarbageFile(); });
    state += SoupTest::RunTest(className, "TryLoadFromDirectory_SimpleFile", [&testClass]() { testClass->TryLoadFromDirectory_SimpleFile(); });

    return state;
}