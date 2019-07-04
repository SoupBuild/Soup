#pragma once
#include "/Build/BuildEngineTests.h"

TestState RunBuildEngineTests() 
 {
    auto className = "BuildEngineTests";
    auto testClass = std::make_shared<Soup::UnitTests::BuildEngineTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "InitialzeEngine_Success", [&testClass]() { testClass->InitialzeEngine_Success(); });
    state += SoupTest::RunTest(className, "InitialzeEngine_NullCompilerThrows", [&testClass]() { testClass->InitialzeEngine_NullCompilerThrows(); });
    state += SoupTest::RunTest(className, "Build_Executable_SingleFile_NotIncremental", [&testClass]() { testClass->Build_Executable_SingleFile_NotIncremental(); });
    state += SoupTest::RunTest(className, "Build_Executable_SingleFile_Incremental_MissingBuildState", [&testClass]() { testClass->Build_Executable_SingleFile_Incremental_MissingBuildState(); });
    state += SoupTest::RunTest(className, "Build_Executable_SingleFile_Incremental_MissingFileInfo", [&testClass]() { testClass->Build_Executable_SingleFile_Incremental_MissingFileInfo(); });
    state += SoupTest::RunTest(className, "Build_Executable_SingleFile_Incremental_UpToDate", [&testClass]() { testClass->Build_Executable_SingleFile_Incremental_UpToDate(); });

    return state;
}