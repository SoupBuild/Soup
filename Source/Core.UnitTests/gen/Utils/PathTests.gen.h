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
    state += SoupTest::RunTest(className, "AlternativeDirectoriesPath", [&testClass]() { testClass->AlternativeDirectoriesPath(); });
    state += SoupTest::RunTest(className, "RemoveParentDirectoryInside", [&testClass]() { testClass->RemoveParentDirectoryInside(); });
    state += SoupTest::RunTest(className, "RemoveTwoParentDirectoryInside", [&testClass]() { testClass->RemoveTwoParentDirectoryInside(); });
    state += SoupTest::RunTest(className, "LeaveParentDirectoryAtStart", [&testClass]() { testClass->LeaveParentDirectoryAtStart(); });
    state += SoupTest::RunTest(className, "Concatenate_Simple", [&testClass]() { testClass->Concatenate_Simple(); });
    state += SoupTest::RunTest(className, "Concatenate_UpDirectory", [&testClass]() { testClass->Concatenate_UpDirectory(); });
    state += SoupTest::RunTest(className, "Concatenate_UpDirectoryBeginning", [&testClass]() { testClass->Concatenate_UpDirectoryBeginning(); });
    state += SoupTest::RunTest(className, "SetFileExtension_Replace", [&testClass]() { testClass->SetFileExtension_Replace(); });
    state += SoupTest::RunTest(className, "SetFileExtension_Add", [&testClass]() { testClass->SetFileExtension_Add(); });
    state += SoupTest::RunTest(className, "GetRelativeTo_Empty", [&testClass]() { testClass->GetRelativeTo_Empty(); });
    state += SoupTest::RunTest(className, "GetRelativeTo_SingleRelative", [&testClass]() { testClass->GetRelativeTo_SingleRelative(); });
    state += SoupTest::RunTest(className, "GetRelativeTo_UpParentRelative", [&testClass]() { testClass->GetRelativeTo_UpParentRelative(); });
    state += SoupTest::RunTest(className, "GetRelativeTo_MismatchRelative", [&testClass]() { testClass->GetRelativeTo_MismatchRelative(); });
    state += SoupTest::RunTest(className, "GetRelativeTo_Rooted_DifferentRoot", [&testClass]() { testClass->GetRelativeTo_Rooted_DifferentRoot(); });
    state += SoupTest::RunTest(className, "GetRelativeTo_Rooted_SingleFolder", [&testClass]() { testClass->GetRelativeTo_Rooted_SingleFolder(); });

    return state;
}