#pragma once
#include "/Build/BuildStateJsonTests.h"

TestState RunBuildStateJsonTests() 
 {
    auto className = "BuildStateJsonTests";
    auto testClass = std::make_shared<Soup::UnitTests::BuildStateJsonTests>();
    TestState state = { 0, 0 };
    state += SoupTest::RunTest(className, "Deserialize_GarbageThrows", [&testClass]() { testClass->Deserialize_GarbageThrows(); });
    state += SoupTest::RunTest(className, "Deserialize_MissingKnownFiles", [&testClass]() { testClass->Deserialize_MissingKnownFiles(); });
    state += SoupTest::RunTest(className, "Deserialize_MissingNameThrows", [&testClass]() { testClass->Deserialize_MissingNameThrows(); });
    state += SoupTest::RunTest(className, "Deserialize_MissingIncludesThrows", [&testClass]() { testClass->Deserialize_MissingIncludesThrows(); });
    state += SoupTest::RunTest(className, "Deserialize_Simple", [&testClass]() { testClass->Deserialize_Simple(); });
    state += SoupTest::RunTest(className, "Serialize_Simple", [&testClass]() { testClass->Serialize_Simple(); });

    return state;
}