#pragma once
#include "/Build/BuildStateCheckerTests.h"

TestState RunBuildStateCheckerTests() 
{
	auto className = "BuildStateCheckerTests";
	auto testClass = std::make_shared<Soup::UnitTests::BuildStateCheckerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "IsOutdated_ZeroInput_Throws", [&testClass]() { testClass->IsOutdated_ZeroInput_Throws(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_MissingTarget", [&testClass]() { testClass->IsOutdated_SingleInput_MissingTarget(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_MissingInputFile", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_MissingInputFile(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_Outdated", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_Outdated(); });
	state += SoupTest::RunTest(className, "IsOutdated_SingleInput_TargetExists_UpToDate", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_UpToDate(); });
	state += SoupTest::RunTest(className, "IsOutdated_MultipleInputs_RelativeAndAbsolute", [&testClass]() { testClass->IsOutdated_MultipleInputs_RelativeAndAbsolute(); });

	return state;
}