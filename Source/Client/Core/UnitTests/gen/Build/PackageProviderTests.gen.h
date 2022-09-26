#pragma once
#include "Build/PackageProviderTests.h"

TestState RunPackageProviderTests() 
{
	auto className = "PackageProviderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::PackageProviderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "LoadClosure_NoDependencies", [&testClass]() { testClass->LoadClosure_NoDependencies(); });
	state += Soup::Test::RunTest(className, "LoadClosure_BuildDependency", [&testClass]() { testClass->LoadClosure_BuildDependency(); });
	state += Soup::Test::RunTest(className, "LoadClosure_TriangleDependency_NoRebuild", [&testClass]() { testClass->LoadClosure_TriangleDependency_NoRebuild(); });
	state += Soup::Test::RunTest(className, "LoadClosure_PackageLock_OverrideBuildDependency", [&testClass]() { testClass->LoadClosure_PackageLock_OverrideBuildDependency(); });

	return state;
}