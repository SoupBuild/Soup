#pragma once
#include "/Utils/PathTests.h"

TestState RunPathTests() 
 {
    auto className = "PathTests";
    auto testClass = std::make_shared<Soup::UnitTests::PathTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "DefaultInitializer", [&testClass]() { testClass->DefaultInitializer(); });
    state += SoupTest::RunTest(className, "SimpleRelativePath", [&testClass]() { testClass->SimpleRelativePath(); });
    state += SoupTest::RunTest(className, "SimpleAbsolutePath", [&testClass]() { testClass->SimpleAbsolutePath(); });

    return state;
}