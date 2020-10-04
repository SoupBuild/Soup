#pragma once
#include "Tasks/BuildTaskTests.h"

TestState RunBuildTaskTests() 
 {
	auto className = "BuildTaskTests";
	auto testClass = std::make_shared<Soup::Cpp::UnitTests::BuildTaskTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	// state += Soup::Test::RunTest(className, "Build_UnknownCompilerFails", [&testClass]() { testClass->Build_UnknownCompilerFails(); });
	state += Soup::Test::RunTest(className, "Build_Executable", [&testClass]() { testClass->Build_Executable(); });
	state += Soup::Test::RunTest(className, "Build_Executable_OptimizeSpeed", [&testClass]() { testClass->Build_Executable_OptimizeSpeed(); });
	state += Soup::Test::RunTest(className, "Build_Executable_OptimizeSize", [&testClass]() { testClass->Build_Executable_OptimizeSize(); });
	state += Soup::Test::RunTest(className, "Build_Library_MultipleFiles", [&testClass]() { testClass->Build_Library_MultipleFiles(); });
	state += Soup::Test::RunTest(className, "Build_Library_ModuleInterface", [&testClass]() { testClass->Build_Library_ModuleInterface(); });
	state += Soup::Test::RunTest(className, "Build_Library_ModuleInterfaceNoSource", [&testClass]() { testClass->Build_Library_ModuleInterfaceNoSource(); });

	return state;
}