#pragma once
#include "Package/RecipeJsonTests.h"

TestState RunRecipeJsonTests() 
 {
	auto className = "RecipeJsonTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::RecipeJsonTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Deserialize_GarbageThrows", [&testClass]() { testClass->Deserialize_GarbageThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingNameThrows", [&testClass]() { testClass->Deserialize_MissingNameThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_MissingVersionThrows", [&testClass]() { testClass->Deserialize_MissingVersionThrows(); });
	state += SoupTest::RunTest(className, "Deserialize_Simple", [&testClass]() { testClass->Deserialize_Simple(); });
	state += SoupTest::RunTest(className, "Deserialize_AllProperties", [&testClass]() { testClass->Deserialize_AllProperties(); });
	state += SoupTest::RunTest(className, "Serialize_Simple", [&testClass]() { testClass->Serialize_Simple(); });
	state += SoupTest::RunTest(className, "Serialize_AllProperties", [&testClass]() { testClass->Serialize_AllProperties(); });

	return state;
}