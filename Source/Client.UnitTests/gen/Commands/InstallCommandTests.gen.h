#pragma once
#include "/Commands/InstallCommandTests.h"

TestState RunInstallCommandTests() 
 {
    auto className = "InstallCommandTests";
    auto testClass = std::make_shared<Soup::Client::UnitTests::InstallCommandTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

    return state;
}