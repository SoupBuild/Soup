#pragma once
#include "Package/PackageManagerTests.h"

TestState RunPackageManagerTests() 
 {
	auto className = "PackageManagerTests";
	auto testClass = std::make_shared<Soup::UnitTests::PackageManagerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "InstallPackages", [&testClass]() { testClass->InstallPackages(); });
	state += Soup::Test::RunTest(className, "InstallPackageReference_Version", [&testClass]() { testClass->InstallPackageReference_Version(); });
	state += Soup::Test::RunTest(className, "InstallPackageReference_NoVersion", [&testClass]() { testClass->InstallPackageReference_NoVersion(); });
	state += Soup::Test::RunTest(className, "PublishPackage", [&testClass]() { testClass->PublishPackage(); });

	return state;
}