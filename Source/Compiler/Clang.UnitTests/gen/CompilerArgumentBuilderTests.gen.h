#pragma once
#include "/CompilerArgumentBuilderTests.h"

TestState RunCompilerArgumentBuilderTests() 
 {
    auto className = "CompilerArgumentBuilderTests";
    auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::CompilerArgumentBuilderTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "EmptySourceFile_Throws", [&testClass]() { testClass->EmptySourceFile_Throws(); });
    state += SoupTest::RunTest(className, "EmptyTargetFile_Throws", [&testClass]() { testClass->EmptyTargetFile_Throws(); });
    state += SoupTest::RunTest(className, "DefaultParameters", [&testClass]() { testClass->DefaultParameters(); });
    state += SoupTest::RunTest(className, "SingleArgument_GenerateIncludeTree", [&testClass]() { testClass->SingleArgument_GenerateIncludeTree(); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP11, \"-std=c++11\")", [&testClass]() { testClass->SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP11, "-std=c++11"); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP14, \"-std=c++14\")", [&testClass]() { testClass->SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP14, "-std=c++14"); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP17, \"-std=c++17\")", [&testClass]() { testClass->SingleArgument_LanguageStandard(Soup::LanguageStandard::CPP17, "-std=c++17"); });
    state += SoupTest::RunTest(className, "SingleArgument_LanguageStandard_CPP20", [&testClass]() { testClass->SingleArgument_LanguageStandard_CPP20(); });
    state += SoupTest::RunTest(className, "SingleArgument_OptimizationLevel_Disabled", [&testClass]() { testClass->SingleArgument_OptimizationLevel_Disabled(); });
    state += SoupTest::RunTest(className, "SingleArgument_OptimizationLevel(Soup::OptimizationLevel::Size, \"-Oz\")", [&testClass]() { testClass->SingleArgument_OptimizationLevel(Soup::OptimizationLevel::Size, "-Oz"); });
    state += SoupTest::RunTest(className, "SingleArgument_OptimizationLevel(Soup::OptimizationLevel::Speed, \"-O3\")", [&testClass]() { testClass->SingleArgument_OptimizationLevel(Soup::OptimizationLevel::Speed, "-O3"); });
    state += SoupTest::RunTest(className, "SingleArgument_IncludePaths", [&testClass]() { testClass->SingleArgument_IncludePaths(); });
    state += SoupTest::RunTest(className, "SingleArgument_PreprocessorDefinitions", [&testClass]() { testClass->SingleArgument_PreprocessorDefinitions(); });
    state += SoupTest::RunTest(className, "SingleArgument_Modules", [&testClass]() { testClass->SingleArgument_Modules(); });
    state += SoupTest::RunTest(className, "SingleArgument_ExportModule_SingleSource", [&testClass]() { testClass->SingleArgument_ExportModule_SingleSource(); });

    return state;
}