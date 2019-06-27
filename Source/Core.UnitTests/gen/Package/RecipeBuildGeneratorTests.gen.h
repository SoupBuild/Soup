#pragma once
#include "/Package/RecipeBuildGeneratorTests.h"

TestState RunRecipeBuildGeneratorTests() 
 {
    auto className = "RecipeBuildGeneratorTests";
    auto testClass = std::make_shared<Soup::UnitTests::RecipeBuildGeneratorTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Initialze", [&testClass]() { testClass->Initialze(); });

    return state;
}