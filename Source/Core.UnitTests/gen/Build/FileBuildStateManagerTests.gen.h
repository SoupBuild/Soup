#pragma once
#include "/Build/FileBuildStateManagerTests.h"

TestState RunFileBuildStateManagerTests() 
 {
    auto className = "FileBuildStateManagerTests";
    auto testClass = std::make_shared<Soup::UnitTests::FileBuildStateManagerTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "TryLoadFromFile_MissingFile", [&testClass]() { testClass->TryLoadFromFile_MissingFile(); });
    state += SoupTest::RunTest(className, "TryLoadFromFile_GarbageFile", [&testClass]() { testClass->TryLoadFromFile_GarbageFile(); });
    state += SoupTest::RunTest(className, "TryLoadFromFile_SimpleFile", [&testClass]() { testClass->TryLoadFromFile_SimpleFile(); });

    return state;
}