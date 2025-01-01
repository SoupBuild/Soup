#pragma once
#include "recipe/RecipeExtensionsTests.h"

TestState RunRecipeExtensionsTests() 
 {
	auto className = "RecipeExtensionsTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::RecipeExtensionsTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "TryLoadRecipeFromFile_MissingFile", [&testClass]() { testClass->TryLoadRecipeFromFile_MissingFile(); });
	state += Soup::Test::RunTest(className, "TryLoadRecipeFromFile_GarbageFile", [&testClass]() { testClass->TryLoadRecipeFromFile_GarbageFile(); });
	state += Soup::Test::RunTest(className, "TryLoadRecipeFromFile_SimpleFile", [&testClass]() { testClass->TryLoadRecipeFromFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "SaveToFile_SimpleFile", [&testClass]() { testClass->SaveToFile_SimpleFile(); });

	return state;
}