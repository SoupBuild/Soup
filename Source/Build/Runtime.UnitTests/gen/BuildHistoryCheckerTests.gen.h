#pragma once
#include "BuildHistoryCheckerTests.h"

TestState RunBuildHistoryCheckerTests() 
{
	auto className = "BuildHistoryCheckerTests";
	auto testClass = std::make_shared<Soup::Build::Runtime::UnitTests::BuildHistoryCheckerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "IsOutdated_ZeroInput", [&testClass]() { testClass->IsOutdated_ZeroInput(); });
	state += Soup::Test::RunTest(className, "IsOutdated_SingleInput_UnknownTarget", [&testClass]() { testClass->IsOutdated_SingleInput_UnknownTarget(); });
	state += Soup::Test::RunTest(className, "IsOutdated_SingleInput_DeletedTarget", [&testClass]() { testClass->IsOutdated_SingleInput_DeletedTarget(); });
	state += Soup::Test::RunTest(className, "IsOutdated_SingleInput_TargetExists_DeletedInputFile", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_UnknownInputFile(); });
	state += Soup::Test::RunTest(className, "IsOutdated_SingleInput_TargetExists_DeletedInputFile", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_DeletedInputFile(); });
	state += Soup::Test::RunTest(className, "IsOutdated_SingleInput_TargetExists_Outdated", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_Outdated(); });
	state += Soup::Test::RunTest(className, "IsOutdated_SingleInput_TargetExists_UpToDate", [&testClass]() { testClass->IsOutdated_SingleInput_TargetExists_UpToDate(); });
	state += Soup::Test::RunTest(className, "IsOutdated_MultipleInputs_RelativeAndAbsolute", [&testClass]() { testClass->IsOutdated_MultipleInputs_RelativeAndAbsolute(); });

	return state;
}