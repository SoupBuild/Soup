#pragma once
#include "/Package/RecipeTypeTests.h"

TestState RunRecipeTypeTests() 
 {
    auto className = "RecipeTypeTests";
    auto testClass = std::make_shared<Soup::UnitTests::RecipeTypeTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeType::Executable, \"Executable\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeType::Executable, "Executable"); });
    state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeType::Library, \"Library\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeType::Library, "Library"); });
    state += SoupTest::RunTest(className, "ParseValues(\"Executable\", Soup::RecipeType::Executable)", [&testClass]() { testClass->ParseValues("Executable", Soup::RecipeType::Executable); });
    state += SoupTest::RunTest(className, "ParseValues(\"Library\", Soup::RecipeType::Library)", [&testClass]() { testClass->ParseValues("Library", Soup::RecipeType::Library); });
    state += SoupTest::RunTest(className, "ParseGarbageThrows", [&testClass]() { testClass->ParseGarbageThrows(); });

    return state;
}