#pragma once
#include "/Commands/VersionCommandTests.h"

TestState RunVersionCommandTests() 
 {
    auto className = "VersionCommandTests";
    auto testClass = std::make_shared<Soup::Client::UnitTests::VersionCommandTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

    return state;
}