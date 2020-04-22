#pragma once
#include "Package/PackageManagerTests.h"

TestState RunPackageManagerTests() 
 {
	auto className = "PackageManagerTests";
	auto testClass = std::make_shared<Soup::UnitTests::PackageManagerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "InstallPackages_NoDependencies_Success", [&testClass]() { testClass->InstallPackages_NoDependencies_Success(); });
	state += SoupTest::RunTest(className, "InstallPackages_OnlyLocalDependencies_Success", [&testClass]() { testClass->InstallPackages_OnlyLocalDependencies_Success(); });
	state += SoupTest::RunTest(className, "InstallPackages_SingleDependency_Success", [&testClass]() { testClass->InstallPackages_SingleDependency_Success(); });
	state += SoupTest::RunTest(className, "InstallPackageReference_MissingRecipe_Throws", [&testClass]() { testClass->InstallPackageReference_MissingRecipe_Throws(); });
	state += SoupTest::RunTest(className, "InstallPackageReference_Version_Success", [&testClass]() { testClass->InstallPackageReference_Version_Success(); });
	state += SoupTest::RunTest(className, "InstallPackageReference_Recursive_Success", [&testClass]() { testClass->InstallPackageReference_Recursive_Success(); });
	state += SoupTest::RunTest(className, "InstallPackageReference_Latest_Success", [&testClass]() { testClass->InstallPackageReference_Latest_Success(); });
	state += SoupTest::RunTest(className, "PublishPackage_Success", [&testClass]() { testClass->PublishPackage_Success(); });
	state += SoupTest::RunTest(className, "PublishPackage_PackageMissing_Success", [&testClass]() { testClass->PublishPackage_PackageMissing_Success(); });

	return state;
}