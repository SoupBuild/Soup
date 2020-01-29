#pragma once
#include "Build/BuildEngineTests.h"

TestState RunBuildEngineTests() 
 {
	auto className = "BuildEngineTests";
	auto testClass = std::make_shared<Soup::Build::UnitTests::BuildEngineTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += SoupTest::RunTest(className, "Build_Executable", [&testClass]() { testClass->Build_Executable(); });
	state += SoupTest::RunTest(className, "Build_Library_MultipleFiles", [&testClass]() { testClass->Build_Library_MultipleFiles(); });
	state += SoupTest::RunTest(className, "Build_Library_ModuleInterface", [&testClass]() { testClass->Build_Library_ModuleInterface(); });
	state += SoupTest::RunTest(className, "Build_Library_ModuleInterfaceNoSource", [&testClass]() { testClass->Build_Library_ModuleInterfaceNoSource(); });

	return state;
}