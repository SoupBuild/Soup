#pragma once
#include "Build/Runner/BuildHistoryJsonTests.h"

TestState RunBuildHistoryJsonTests() 
{
	auto className = "BuildHistoryJsonTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::BuildHistoryJsonTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Deserialize_GarbageThrows", [&testClass]() { testClass->Deserialize_GarbageThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingKnownFiles", [&testClass]() { testClass->Deserialize_MissingKnownFiles(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingFileThrows", [&testClass]() { testClass->Deserialize_MissingFileThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingIncludesThrows", [&testClass]() { testClass->Deserialize_MissingIncludesThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_Simple", [&testClass]() { testClass->Deserialize_Simple(); });
	state += SoupTest::RunTest(className, "Deserialize_Multiple", [&testClass]() { testClass->Deserialize_Multiple(); });
	state += SoupTest::RunTest(className, "Serialize_Simple", [&testClass]() { testClass->Serialize_Simple(); });
	state += SoupTest::RunTest(className, "Serialize_Multipl", [&testClass]() { testClass->Serialize_Multipl(); });

	return state;
}