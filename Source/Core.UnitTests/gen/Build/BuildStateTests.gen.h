#pragma once
#include "/Build/BuildStateTests.h"

TestState RunBuildStateTests() 
 {
    auto className = "BuildStateTests";
    auto testClass = std::make_shared<Soup::UnitTests::BuildStateTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "TryBuildIncludeClosure_SourceFileMissingFails", [&testClass]() { testClass->TryBuildIncludeClosure_SourceFileMissingFails(); });
    state += SoupTest::RunTest(className, "TryBuildIncludeClosure_DependencyFileMissingFails", [&testClass]() { testClass->TryBuildIncludeClosure_DependencyFileMissingFails(); });
    state += SoupTest::RunTest(className, "TryBuildIncludeClosure_NoDependencies", [&testClass]() { testClass->TryBuildIncludeClosure_NoDependencies(); });
    state += SoupTest::RunTest(className, "TryBuildIncludeClosure_MultipleDependencies", [&testClass]() { testClass->TryBuildIncludeClosure_MultipleDependencies(); });

    return state;
}