#pragma once
#include "Build/Runner/BuildHistoryCheckerTests.h"

TestState RunBuildHistoryCheckerTests() 
{
	auto className = "BuildHistoryCheckerTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::BuildHistoryCheckerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "IsOutdated_NonRootedRoot_Throws", [&testClass]() { testClass->IsOutdated_NonRootedRoot_Throws(); });
	state += SoupTest::RunTest(className, "IsOutdated_ZeroInput_Throws", [&testClass]() { testClass->IsOutdated_ZeroInput_Throws(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_MissingTarget", [&testClass]() { testClass->IsOutdated_SingleInput_MissingTarget(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_MissingInputFile", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_MissingInputFile(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_Outdated", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_Outdated(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_UpToDate", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_UpToDate(); });
	state += SoupTest::RunTest(className, "IsOutdated_MultipleInputs_RelativeAndAbsolute", [&testClass]() { testClass->IsOutdated_MultipleInputs_RelativeAndAbsolute(); });

	return state;
}