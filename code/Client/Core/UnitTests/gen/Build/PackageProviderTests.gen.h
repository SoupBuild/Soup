#pragma once
#include "Build/PackageProviderTests.h"

TestState RunPackageProviderTests() 
{
	auto className = "PackageProviderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::PackageProviderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

	return state;
}