#pragma once
#include "BuildEngineTests.h"

TestState RunBuildEngineTests() 
 {
	auto className = "BuildEngineTests";
	auto testClass = std::make_shared<Soup::CSharp::Compiler::UnitTests::BuildEngineTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += Soup::Test::RunTest(className, "Build_Executable", [&testClass]() { testClass->Build_Executable(); });
	state += Soup::Test::RunTest(className, "Build_Library_NoSource", [&testClass]() { testClass->Build_Library_NoSource(); });
	state += Soup::Test::RunTest(className, "Build_Library_MultipleFiles", [&testClass]() { testClass->Build_Library_MultipleFiles(); });

	return state;
}