#pragma once
#include "Tasks/BuildTaskTests.h"

TestState RunBuildTaskTests() 
 {
	auto className = "BuildTaskTests";
	auto testClass = std::make_shared<Soup::Rust::UnitTests::BuildTaskTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += Soup::Test::RunTest(className, "Build_Executable", [&testClass]() { testClass->Build_Executable(); });
	state += Soup::Test::RunTest(className, "Build_Executable_Optimize", [&testClass]() { testClass->Build_Executable_Optimize(); });
	state += Soup::Test::RunTest(className, "Build_Library_MultipleFiles", [&testClass]() { testClass->Build_Library_MultipleFiles(); });

	return state;
}