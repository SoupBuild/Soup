#pragma once
#include "Package/RecipeJsonTests.h"

TestState RunRecipeJsonTests() 
 {
	auto className = "RecipeJsonTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::RecipeJsonTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Deserialize_GarbageThrows", [&testClass]() { testClass->Deserialize_GarbageThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_MissingNameThrows", [&testClass]() { testClass->Deserialize_MissingNameThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_MissingVersionThrows", [&testClass]() { testClass->Deserialize_MissingVersionThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_Simple", [&testClass]() { testClass->Deserialize_Simple(); });
	state += Soup::Test::RunTest(className, "Deserialize_AllProperties", [&testClass]() { testClass->Deserialize_AllProperties(); });
	state += Soup::Test::RunTest(className, "Serialize_Simple", [&testClass]() { testClass->Serialize_Simple(); });
	state += Soup::Test::RunTest(className, "Serialize_AllProperties", [&testClass]() { testClass->Serialize_AllProperties(); });

	return state;
}