#pragma once
#include "Build/BuildRunnerTests.h"

TestState RunBuildRunnerTests() 
 {
	auto className = "BuildRunnerTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::BuildRunnerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += Soup::Test::RunTest(className, "Execute_NoDependencies", [&testClass]() { testClass->Execute_NoDependencies(); });
	state += Soup::Test::RunTest(className, "Execute_TriangleDependency_NoRebuild", [&testClass]() { testClass->Execute_TriangleDependency_NoRebuild(); });
	state += Soup::Test::RunTest(className, "Execute_BuildDependency", [&testClass]() { testClass->Execute_BuildDependency(); });
	state += Soup::Test::RunTest(className, "Execute_PackageLock_OverrideBuildDependency", [&testClass]() { testClass->Execute_PackageLock_OverrideBuildDependency(); });

	return state;
}