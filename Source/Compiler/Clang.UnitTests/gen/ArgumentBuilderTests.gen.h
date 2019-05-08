#pragma once
#include "/ArgumentBuilderTests.h"

TestState RunArgumentBuilderTests() 
 {
    auto className = "ArgumentBuilderTests";
    auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::ArgumentBuilderTests>();
    TestState state = { 0, 0 };
    state += RunTest(className, "NoParameters", [&testClass]() { testClass->NoParameters(); });

    return state;
}