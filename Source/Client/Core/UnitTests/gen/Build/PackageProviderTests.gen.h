#pragma once
#include "Build/PackageProviderTests.h"

TestState RunPackageProviderTests() 
{
	auto className = "PackageProviderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::PackageProviderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Initialize_NoDependencies", [&testClass]() { testClass->Initialize_NoDependencies(); });
	state += Soup::Test::RunTest(className, "Initialize_BuildDependency", [&testClass]() { testClass->Initialize_BuildDependency(); });
	state += Soup::Test::RunTest(className, "Initialize_TriangleDependency_NoRebuild", [&testClass]() { testClass->Initialize_TriangleDependency_NoRebuild(); });
	state += Soup::Test::RunTest(className, "Initialize_PackageLock_OverrideBuildDependency", [&testClass]() { testClass->Initialize_PackageLock_OverrideBuildDependency(); });

	return state;
}