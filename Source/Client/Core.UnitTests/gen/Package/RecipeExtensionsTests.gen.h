#pragma once
#include "Package/RecipeExtensionsTests.h"

TestState RunRecipeExtensionsTests() 
 {
	auto className = "RecipeExtensionsTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::RecipeExtensionsTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "TryLoadFromFile_MissingFile", [&testClass]() { testClass->TryLoadFromFile_MissingFile(); });
	state += Soup::Test::RunTest(className, "TryLoadFromFile_GarbageFile", [&testClass]() { testClass->TryLoadFromFile_GarbageFile(); });
	state += Soup::Test::RunTest(className, "TryLoadFromFile_SimpleFile", [&testClass]() { testClass->TryLoadFromFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "SaveToFile_SimpleFile", [&testClass]() { testClass->SaveToFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "GetRecipeOutputPath_MissingRecipeThrows", [&testClass]() { testClass->GetRecipeOutputPath_MissingRecipeThrows(); });
	state += Soup::Test::RunTest(className, "GetRecipeOutputPath_Exists", [&testClass]() { testClass->GetRecipeOutputPath_Exists(); });
	state += Soup::Test::RunTest(className, "GetPackageReferencePath_IsRooted", [&testClass]() { testClass->GetPackageReferencePath_IsRooted(); });
	state += Soup::Test::RunTest(className, "GetPackageReferencePath_NotRooted", [&testClass]() { testClass->GetPackageReferencePath_NotRooted(); });

	return state;
}