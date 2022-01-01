#pragma once
#include "LocalUserConfig/LocalUserConfigExtensionsTests.h"

TestState RunLocalUserConfigExtensionsTests() 
 {
	auto className = "LocalUserConfigExtensionsTests";
	auto testClass = std::make_shared<Soup::Build::Runtime::UnitTests::LocalUserConfigExtensionsTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "TryLoadLocalUserConfigFromFile_MissingFile", [&testClass]() { testClass->TryLoadLocalUserConfigFromFile_MissingFile(); });
	state += Soup::Test::RunTest(className, "TryLoadLocalUserConfigFromFile_GarbageFile", [&testClass]() { testClass->TryLoadLocalUserConfigFromFile_GarbageFile(); });
	state += Soup::Test::RunTest(className, "TryLoadLocalUserConfigFromFile_SimpleFile", [&testClass]() { testClass->TryLoadLocalUserConfigFromFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "SaveToFile_SimpleFile", [&testClass]() { testClass->SaveToFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "GetPackageReferencePath_IsRooted", [&testClass]() { testClass->GetPackageReferencePath_IsRooted(); });
	state += Soup::Test::RunTest(className, "GetPackageReferencePath_NotRooted", [&testClass]() { testClass->GetPackageReferencePath_NotRooted(); });

	return state;
}