#pragma once
#include "/Build/BuildEngineTests.h"

TestState RunBuildEngineTests() 
 {
    auto className = "BuildEngineTests";
    auto testClass = std::make_shared<Soup::UnitTests::BuildEngineTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "InitialzeEngine_Success", [&testClass]() { testClass->InitialzeEngine_Success(); });
    state += SoupTest::RunTest(className, "InitialzeEngine_NullCompilerThrows", [&testClass]() { testClass->InitialzeEngine_NullCompilerThrows(); });
    state += SoupTest::RunTest(className, "Build_Executable_Simple_NotIncremental", [&testClass]() { testClass->Build_Executable_Simple_NotIncremental(); });
    state += SoupTest::RunTest(className, "Build_Executable_Simple_Incremental_MissingBuildState", [&testClass]() { testClass->Build_Executable_Simple_Incremental_MissingBuildState(); });
    state += SoupTest::RunTest(className, "Build_Executable_Simple_Incremental_MissingFileInfo", [&testClass]() { testClass->Build_Executable_Simple_Incremental_MissingFileInfo(); });
    state += SoupTest::RunTest(className, "Build_Executable_Simple_Incremental_MissingTargetFile", [&testClass]() { testClass->Build_Executable_Simple_Incremental_MissingTargetFile(); });
    state += SoupTest::RunTest(className, "Build_Executable_Simple_Incremental_UpToDate", [&testClass]() { testClass->Build_Executable_Simple_Incremental_UpToDate(); });
    state += SoupTest::RunTest(className, "Build_Library_MultipleFiles_IncludeModuleUpdated", [&testClass]() { testClass->Build_Library_MultipleFiles_IncludeModuleUpdated(); });
    state += SoupTest::RunTest(className, "Build_Library_Complex_OutOfDate", [&testClass]() { testClass->Build_Library_Complex_OutOfDate(); });
    state += SoupTest::RunTest(className, "Build_Library_ModuleInterface_UpToDate", [&testClass]() { testClass->Build_Library_ModuleInterface_UpToDate(); });
    state += SoupTest::RunTest(className, "Build_Library_ModuleInterfaceNoSource_OutOfDate", [&testClass]() { testClass->Build_Library_ModuleInterfaceNoSource_OutOfDate(); });

    return state;
}