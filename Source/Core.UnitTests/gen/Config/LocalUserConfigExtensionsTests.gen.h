#pragma once
#include "Config/LocalUserConfigExtensionsTests.h"

TestState RunLocalUserConfigExtensionsTests() 
 {
	auto className = "LocalUserConfigExtensionsTests";
	auto testClass = std::make_shared<Soup::UnitTests::LocalUserConfigExtensionsTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "GetLocalUserConfigFilePath", [&testClass]() { testClass->GetLocalUserConfigFilePath(); });
	state += SoupTest::RunTest(className, "LoadFromFile_MissingFileThrows", [&testClass]() { testClass->LoadFromFile_MissingFileThrows(); });
	state += SoupTest::RunTest(className, "LoadFromFile_GarbageFileThrows", [&testClass]() { testClass->LoadFromFile_GarbageFileThrows(); });
	state += SoupTest::RunTest(className, "LoadFromFile_SimpleFile", [&testClass]() { testClass->LoadFromFile_SimpleFile(); });

	return state;
}