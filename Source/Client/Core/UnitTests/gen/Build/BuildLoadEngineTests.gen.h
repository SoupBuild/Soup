#pragma once
#include "Build/BuildLoadEngineTests.h"

TestState RunBuildLoadEngineTests() 
{
	auto className = "BuildLoadEngineTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::BuildLoadEngineTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Load_NoDependencies_BuiltInLanguage", [&testClass]() { testClass->Load_NoDependencies_BuiltInLanguage(); });
	state += Soup::Test::RunTest(className, "Load_NoDependencies_ExternalLanguage", [&testClass]() { testClass->Load_NoDependencies_ExternalLanguage(); });
	state += Soup::Test::RunTest(className, "Load_TriangleDependency_NoRebuild", [&testClass]() { testClass->Load_TriangleDependency_NoRebuild(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_NoPackageLock_ExternalReference", [&testClass]() { testClass->Load_BuildDependency_NoPackageLock_ExternalReference(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_NoPackageLock_Local", [&testClass]() { testClass->Load_BuildDependency_NoPackageLock_Local(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_PackageLock", [&testClass]() { testClass->Load_BuildDependency_PackageLock(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_PackageLock_Override", [&testClass]() { testClass->Load_BuildDependency_PackageLock_Override(); });
	state += Soup::Test::RunTest(className, "Load_OtherDependency_PackageLock_ExplicitLanguage", [&testClass]() { testClass->Load_OtherDependency_PackageLock_ExplicitLanguage(); });

	return state;
}