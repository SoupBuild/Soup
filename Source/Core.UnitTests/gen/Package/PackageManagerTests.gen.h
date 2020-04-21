#pragma once
#include "Package/PackageManagerTests.h"

TestState RunPackageManagerTests() 
 {
	auto className = "PackageManagerTests";
	auto testClass = std::make_shared<Soup::UnitTests::PackageManagerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "InstallPackageReference_MissingRecipe_Throws", [&testClass]() { testClass->InstallPackageReference_MissingRecipe_Throws(); });
	state += SoupTest::RunTest(className, "InstallPackageReference_Latest_Success", [&testClass]() { testClass->InstallPackageReference_Latest_Success(); });
	state += SoupTest::RunTest(className, "PublishPackage_Success", [&testClass]() { testClass->PublishPackage_Success(); });

	return state;
}