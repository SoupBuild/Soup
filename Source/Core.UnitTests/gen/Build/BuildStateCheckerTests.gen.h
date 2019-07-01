#pragma once
#include "/Build/BuildStateCheckerTests.h"

TestState RunBuildStateCheckerTests() 
 {
    auto className = "BuildStateCheckerTests";
    auto testClass = std::make_shared<Soup::UnitTests::BuildStateCheckerTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "IsOutdated_SingleInput_MissingTarget", [&testClass]() { testClass->IsOutdated_SingleInput_MissingTarget(); });
    state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_MissingInput", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_MissingInput(); });
    state += SoupTest::RunTest(className, "IsOutdated_SingleInput_ExistsOut_Outdated", [&testClass]() { testClass->IsOutdated_SingleInput_ExistsOut_Outdated(); });
    state += SoupTest::RunTest(className, "IsOutdated_SingleInput_ExistsOut_UpToDate", [&testClass]() { testClass->IsOutdated_SingleInput_ExistsOut_UpToDate(); });

    return state;
}