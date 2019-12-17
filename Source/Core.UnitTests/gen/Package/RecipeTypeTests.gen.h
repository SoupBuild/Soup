#pragma once
#include "Package/RecipeTypeTests.h"

TestState RunRecipeTypeTests() 
 {
	auto className = "RecipeTypeTests";
	auto testClass = std::make_shared<Soup::UnitTests::RecipeTypeTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeType::Executable, \"Executable\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeType::Executable, "Executable"); });
	state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeType::StaticLibrary, \"StaticLibrary\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeType::StaticLibrary, "StaticLibrary"); });
	state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeType::DynamicLibrary, \"DynamicLibrary\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeType::DynamicLibrary, "DynamicLibrary"); });
	state += SoupTest::RunTest(className, "ParseValues(\"Executable\", Soup::RecipeType::Executable)", [&testClass]() { testClass->ParseValues("Executable", Soup::RecipeType::Executable); });
	state += SoupTest::RunTest(className, "ParseValues(\"StaticLibrary\", Soup::RecipeType::StaticLibrary)", [&testClass]() { testClass->ParseValues("StaticLibrary", Soup::RecipeType::StaticLibrary); });
	state += SoupTest::RunTest(className, "ParseValues(\"DynamicLibrary\", Soup::RecipeType::DynamicLibrary)", [&testClass]() { testClass->ParseValues("DynamicLibrary", Soup::RecipeType::DynamicLibrary); });
	state += SoupTest::RunTest(className, "ParseGarbageThrows", [&testClass]() { testClass->ParseGarbageThrows(); });

	return state;
}