#pragma once
#include "/Package/RecipeBuildGeneratorTests.h"

TestState RunRecipeBuildGeneratorTests() 
 {
    auto className = "RecipeBuildGeneratorTests";
    auto testClass = std::make_shared<Soup::UnitTests::RecipeBuildGeneratorTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
    state += SoupTest::RunTest(className, "Initialize_NullCompilerThrows", [&testClass]() { testClass->Initialize_NullCompilerThrows(); });
    state += SoupTest::RunTest(className, "Execute", [&testClass]() { testClass->Execute(); });

    return state;
}