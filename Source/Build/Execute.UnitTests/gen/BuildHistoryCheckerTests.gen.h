#pragma once
#include "BuildHistoryCheckerTests.h"

TestState RunBuildHistoryCheckerTests() 
{
	auto className = "BuildHistoryCheckerTests";
	auto testClass = std::make_shared<Soup::Build::Execute::UnitTests::BuildHistoryCheckerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "IsOutdated_ZeroInput", [&testClass]() { testClass->IsOutdated_ZeroInput(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_MissingTarget", [&testClass]() { testClass->IsOutdated_SingleInput_MissingTarget(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_MissingInputFile", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_MissingInputFile(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_Outdated", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_Outdated(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_UpToDate", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_UpToDate(); });
	state += SoupTest::RunTest(className, "IsOutdated_MultipleInputs_RelativeAndAbsolute", [&testClass]() { testClass->IsOutdated_MultipleInputs_RelativeAndAbsolute(); });

	return state;
}