#pragma once
#include "/ArgumentBuilderTests.h"

TestState RunArgumentBuilderTests() 
 {
    auto className = "ArgumentBuilderTests";
    auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::ArgumentBuilderTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "NoParameters", [&testClass]() { testClass->NoParameters(); });
    state += SoupTest::RunTest(className, "SingleArgument_GenerateIncludeTree", [&testClass]() { testClass->SingleArgument_GenerateIncludeTree(); });
    state += SoupTest::RunTest(className, "SingleArgument_ExportModule_ThrowsZeroSource", [&testClass]() { testClass->SingleArgument_ExportModule_ThrowsZeroSource(); });
    state += SoupTest::RunTest(className, "SingleArgument_ExportModule_ThrowsMoreThanOneSource", [&testClass]() { testClass->SingleArgument_ExportModule_ThrowsMoreThanOneSource(); });
    state += SoupTest::RunTest(className, "SingleArgument_ExportModule_SingleSource", [&testClass]() { testClass->SingleArgument_ExportModule_SingleSource(); });

    return state;
}