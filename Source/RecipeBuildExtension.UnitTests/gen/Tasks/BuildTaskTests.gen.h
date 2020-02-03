#pragma once
#include "Tasks/BuildTaskTests.h"

TestState RunBuildTaskTests() 
 {
	auto className = "BuildTaskTests";
	auto testClass = std::make_shared<RecipeBuild::UnitTests::BuildTaskTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	// state += SoupTest::RunTest(className, "Build_UnknownCompilerFails", [&testClass]() { testClass->Build_UnknownCompilerFails(); });
	state += SoupTest::RunTest(className, "Build_Executable", [&testClass]() { testClass->Build_Executable(); });
	state += SoupTest::RunTest(className, "Build_Executable_OptimizeSpeed", [&testClass]() { testClass->Build_Executable_OptimizeSpeed(); });
	state += SoupTest::RunTest(className, "Build_Executable_OptimizeSize", [&testClass]() { testClass->Build_Executable_OptimizeSize(); });
	state += SoupTest::RunTest(className, "Build_Library_MultipleFiles", [&testClass]() { testClass->Build_Library_MultipleFiles(); });
	state += SoupTest::RunTest(className, "Build_Library_ModuleInterface", [&testClass]() { testClass->Build_Library_ModuleInterface(); });
	state += SoupTest::RunTest(className, "Build_Library_ModuleInterfaceNoSource", [&testClass]() { testClass->Build_Library_ModuleInterfaceNoSource(); });

	return state;
}