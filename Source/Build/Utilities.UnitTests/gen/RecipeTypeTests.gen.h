#pragma once
#include "RecipeTypeTests.h"

TestState RunRecipeTypeTests() 
 {
	auto className = "RecipeTypeTests";
	auto testClass = std::make_shared<Soup::Build::Utilities::UnitTests::RecipeTypeTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "ToStringValues(Soup::RecipeType::Executable, \"Executable\")", [&testClass]() { testClass->ToStringValues(Soup::Build::Utilities::RecipeType::Executable, "Executable"); });
	state += Soup::Test::RunTest(className, "ToStringValues(Soup::RecipeType::StaticLibrary, \"StaticLibrary\")", [&testClass]() { testClass->ToStringValues(Soup::Build::Utilities::RecipeType::StaticLibrary, "StaticLibrary"); });
	state += Soup::Test::RunTest(className, "ToStringValues(Soup::RecipeType::DynamicLibrary, \"DynamicLibrary\")", [&testClass]() { testClass->ToStringValues(Soup::Build::Utilities::RecipeType::DynamicLibrary, "DynamicLibrary"); });
	state += Soup::Test::RunTest(className, "ParseValues(\"Executable\", Soup::RecipeType::Executable)", [&testClass]() { testClass->ParseValues("Executable", Soup::Build::Utilities::RecipeType::Executable); });
	state += Soup::Test::RunTest(className, "ParseValues(\"StaticLibrary\", Soup::RecipeType::StaticLibrary)", [&testClass]() { testClass->ParseValues("StaticLibrary", Soup::Build::Utilities::RecipeType::StaticLibrary); });
	state += Soup::Test::RunTest(className, "ParseValues(\"DynamicLibrary\", Soup::RecipeType::DynamicLibrary)", [&testClass]() { testClass->ParseValues("DynamicLibrary", Soup::Build::Utilities::RecipeType::DynamicLibrary); });
	state += Soup::Test::RunTest(className, "ParseGarbageThrows", [&testClass]() { testClass->ParseGarbageThrows(); });

	return state;
}