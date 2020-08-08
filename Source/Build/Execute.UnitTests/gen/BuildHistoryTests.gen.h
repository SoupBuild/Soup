#pragma once
#include "BuildHistoryTests.h"

TestState RunBuildHistoryTests() 
 {
	auto className = "BuildHistoryTests";
	auto testClass = std::make_shared<Soup::Build::Execute::UnitTests::BuildHistoryTests>();
	TestState state = { 0, 0 };

	return state;
}