#pragma once
#include "/Utils/PathTests.h"

TestState RunPathTests() 
 {
    auto className = "PathTests";
    auto testClass = std::make_shared<Soup::UnitTests::PathTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "DefaultInitializer", [&testClass]() { testClass->DefaultInitializer(); });
    state += SoupTest::RunTest(className, "SimpleRelativePath", [&testClass]() { testClass->SimpleRelativePath(); });
    state += SoupTest::RunTest(className, "SimpleAbsolutePath", [&testClass]() { testClass->SimpleAbsolutePath(); });
    state += SoupTest::RunTest(className, "RemoveParentDirectoryInside", [&testClass]() { testClass->RemoveParentDirectoryInside(); });
    state += SoupTest::RunTest(className, "RemoveTwoParentDirectoryInside", [&testClass]() { testClass->RemoveTwoParentDirectoryInside(); });
    state += SoupTest::RunTest(className, "LeaveParentDirectoryAtStart", [&testClass]() { testClass->LeaveParentDirectoryAtStart(); });
    state += SoupTest::RunTest(className, "Concatenate_Simple", [&testClass]() { testClass->Concatenate_Simple(); });
    state += SoupTest::RunTest(className, "Concatenate_UpDirectory", [&testClass]() { testClass->Concatenate_UpDirectory(); });
    state += SoupTest::RunTest(className, "Concatenate_UpDirectoryBeginning", [&testClass]() { testClass->Concatenate_UpDirectoryBeginning(); });

    return state;
}