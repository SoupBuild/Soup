#pragma once
#include "/CompilerTests.h"

TestState RunCompilerTests() 
 {
    auto className = "CompilerTests";
    auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::CompilerTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

    return state;
}