#pragma once
#include "/Build/BuildEngineTests.h"

TestState RunBuildEngineTests() 
 {
    auto className = "BuildEngineTests";
    auto testClass = std::make_shared<Soup::UnitTests::BuildEngineTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "InitialzeEngine", [&testClass]() { testClass->InitialzeEngine(); });

    return state;
}