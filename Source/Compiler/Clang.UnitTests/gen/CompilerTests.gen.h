#pragma once
#include "/CompilerTests.h"

TestState RunCompilerTests() 
 {
    auto className = "CompilerTests";
    auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::CompilerTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
    state += SoupTest::RunTest(className, "Compile_Simple", [&testClass]() { testClass->Compile_Simple(); });
    state += SoupTest::RunTest(className, "Compile_Module", [&testClass]() { testClass->Compile_Module(); });
    state += SoupTest::RunTest(className, "LinkStaticLibrary_Simple", [&testClass]() { testClass->LinkStaticLibrary_Simple(); });
    state += SoupTest::RunTest(className, "LinkExecutable_Simple", [&testClass]() { testClass->LinkExecutable_Simple(); });

    return state;
}