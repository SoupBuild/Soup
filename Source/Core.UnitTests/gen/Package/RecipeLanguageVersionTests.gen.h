#pragma once
#include "Package/RecipeLanguageVersionTests.h"

TestState RunRecipeLanguageVersionTests() 
 {
	auto className = "RecipeLanguageVersionTests";
	auto testClass = std::make_shared<Soup::UnitTests::RecipeLanguageVersionTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeLanguageVersion::CPP11, \"C++11\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeLanguageVersion::CPP11, "C++11"); });
	state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeLanguageVersion::CPP14, \"C++14\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeLanguageVersion::CPP14, "C++14"); });
	state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeLanguageVersion::CPP17, \"C++17\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeLanguageVersion::CPP17, "C++17"); });
	state += SoupTest::RunTest(className, "ToStringValues(Soup::RecipeLanguageVersion::CPP20, \"C++17\")", [&testClass]() { testClass->ToStringValues(Soup::RecipeLanguageVersion::CPP20, "C++20"); });
	state += SoupTest::RunTest(className, "ParseValues(\"C++11\", Soup::RecipeLanguageVersion::CPP11)", [&testClass]() { testClass->ParseValues("C++11", Soup::RecipeLanguageVersion::CPP11); });
	state += SoupTest::RunTest(className, "ParseValues(\"C++14\", Soup::RecipeLanguageVersion::CPP14)", [&testClass]() { testClass->ParseValues("C++14", Soup::RecipeLanguageVersion::CPP14); });
	state += SoupTest::RunTest(className, "ParseValues(\"C++17\", Soup::RecipeLanguageVersion::CPP17)", [&testClass]() { testClass->ParseValues("C++17", Soup::RecipeLanguageVersion::CPP17); });
	state += SoupTest::RunTest(className, "ParseValues(\"C++20\", Soup::RecipeLanguageVersion::CPP20)", [&testClass]() { testClass->ParseValues("C++20", Soup::RecipeLanguageVersion::CPP20); });
	state += SoupTest::RunTest(className, "ParseGarbageThrows", [&testClass]() { testClass->ParseGarbageThrows(); });

	return state;
}