#pragma once
#include "BuildHistoryTests.h"

TestState RunBuildHistoryTests() 
 {
	auto className = "BuildHistoryTests";
	auto testClass = std::make_shared<Soup::Build::Execute::UnitTests::BuildHistoryTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "TryBuildIncludeClosure_SourceFileMissingFails", [&testClass]() { testClass->TryBuildIncludeClosure_SourceFileMissingFails(); });
	state += SoupTest::RunTest(className, "TryBuildIncludeClosure_DependencyFileMissingFails", [&testClass]() { testClass->TryBuildIncludeClosure_DependencyFileMissingFails(); });
	state += SoupTest::RunTest(className, "TryBuildIncludeClosure_NoDependencies", [&testClass]() { testClass->TryBuildIncludeClosure_NoDependencies(); });
	state += SoupTest::RunTest(className, "TryBuildIncludeClosure_MultipleDependencies", [&testClass]() { testClass->TryBuildIncludeClosure_MultipleDependencies(); });
	state += SoupTest::RunTest(className, "TryBuildIncludeClosure_CircularDependencies", [&testClass]() { testClass->TryBuildIncludeClosure_CircularDependencies(); });

	return state;
}