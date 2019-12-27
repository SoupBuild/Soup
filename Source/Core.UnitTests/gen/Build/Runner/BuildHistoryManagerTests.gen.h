#pragma once
#include "Build/Runner/BuildHistoryManagerTests.h"

TestState RunBuildHistoryManagerTests() 
 {
	auto className = "BuildHistoryManagerTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::BuildHistoryManagerTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "TryLoadFromFile_MissingFile", [&testClass]() { testClass->TryLoadFromFile_MissingFile(); });
	state += SoupTest::RunTest(className, "TryLoadFromFile_GarbageFile", [&testClass]() { testClass->TryLoadFromFile_GarbageFile(); });
	state += SoupTest::RunTest(className, "TryLoadFromFile_SimpleFile", [&testClass]() { testClass->TryLoadFromFile_SimpleFile(); });

	return state;
}