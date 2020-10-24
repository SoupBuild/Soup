#pragma once
#include "Package/RecipeExtensionsTests.h"

TestState RunRecipeExtensionsTests() 
 {
	auto className = "RecipeExtensionsTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::RecipeExtensionsTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "TryLoadRecipeFromFile_MissingFile", [&testClass]() { testClass->TryLoadRecipeFromFile_MissingFile(); });
	state += Soup::Test::RunTest(className, "TryLoadRecipeFromFile_GarbageFile", [&testClass]() { testClass->TryLoadRecipeFromFile_GarbageFile(); });
	state += Soup::Test::RunTest(className, "TryLoadRecipeFromFile_SimpleFile", [&testClass]() { testClass->TryLoadRecipeFromFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "SaveToFile_SimpleFile", [&testClass]() { testClass->SaveToFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "GetPackageReferencePath_IsRooted", [&testClass]() { testClass->GetPackageReferencePath_IsRooted(); });
	state += Soup::Test::RunTest(className, "GetPackageReferencePath_NotRooted", [&testClass]() { testClass->GetPackageReferencePath_NotRooted(); });

	return state;
}