#pragma once
#include "Config/LocalUserConfigExtensionsTests.h"

TestState RunLocalUserConfigExtensionsTests() 
 {
	auto className = "LocalUserConfigExtensionsTests";
	auto testClass = std::make_shared<Soup::UnitTests::LocalUserConfigExtensionsTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "GetLocalUserConfigFilePath", [&testClass]() { testClass->GetLocalUserConfigFilePath(); });
	state += Soup::Test::RunTest(className, "LoadFromFile_MissingFileThrows", [&testClass]() { testClass->LoadFromFile_MissingFileThrows(); });
	state += Soup::Test::RunTest(className, "LoadFromFile_GarbageFileThrows", [&testClass]() { testClass->LoadFromFile_GarbageFileThrows(); });
	state += Soup::Test::RunTest(className, "LoadFromFile_SimpleFile", [&testClass]() { testClass->LoadFromFile_SimpleFile(); });

	return state;
}