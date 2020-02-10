#pragma once
#include "Package/PackageManagerTests.h"

TestState RunPackageManagerTests() 
 {
	auto className = "PackageManagerTests";
	auto testClass = std::make_shared<Soup::UnitTests::PackageManagerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "InstallPackage_MissingRecipeThrows", [&testClass]() { testClass->InstallPackage_MissingRecipeThrows(); });
	state += SoupTest::RunTest(className, "InstallPackage_SingleSuccess", [&testClass]() { testClass->InstallPackage_SingleSuccess(); });

	return state;
}