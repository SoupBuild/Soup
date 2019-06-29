#pragma once
#include "/ArgumentBuilderTests.h"

TestState RunArgumentBuilderTests() 
 {
    auto className = "ArgumentBuilderTests";
    auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::ArgumentBuilderTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "NoParameters", [&testClass]() { testClass->NoParameters(); });
    state += SoupTest::RunTest(className, "SingleArgument_GenerateIncludeTree", [&testClass]() { testClass->SingleArgument_GenerateIncludeTree(); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard_CPP11", [&testClass]() { testClass->SingleArgument_LanguageStandard_CPP11(); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard_CPP14", [&testClass]() { testClass->SingleArgument_LanguageStandard_CPP14(); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard_CPP17", [&testClass]() { testClass->SingleArgument_LanguageStandard_CPP17(); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard_CPP20", [&testClass]() { testClass->SingleArgument_LanguageStandard_CPP20(); });
    state += SoupTest::RunTest(className, "SingleArgument_IncludePaths", [&testClass]() { testClass->SingleArgument_IncludePaths(); });
    state += SoupTest::RunTest(className, "SingleArgument_PreprocessorDefinitions", [&testClass]() { testClass->SingleArgument_PreprocessorDefinitions(); });
    state += SoupTest::RunTest(className, "SingleArgument_Modules", [&testClass]() { testClass->SingleArgument_Modules(); });
    state += SoupTest::RunTest(className, "SingleArgument_ExportModule_ThrowsZeroSource", [&testClass]() { testClass->SingleArgument_ExportModule_ThrowsZeroSource(); });
    state += SoupTest::RunTest(className, "SingleArgument_ExportModule_SingleSource", [&testClass]() { testClass->SingleArgument_ExportModule_SingleSource(); });

    return state;
}