#pragma once
#include "/Package/RecipeTypeTests.h"

TestState RunRecipeTypeTests() 
 {
    auto className = "RecipeTypeTests";
    auto testClass = std::make_shared<Soup::UnitTests::RecipeTypeTests>();
    TestState state = { 0, 0 };
    state += RunTest(className, "ToStringValues(Soup::RecipeType::Executable, \"Executable\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeType::Executable, "Executable"); });
    state += RunTest(className, "ToStringValues(Soup::RecipeType::Library, \"Library\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeType::Library, "Library"); });

    return state;
}