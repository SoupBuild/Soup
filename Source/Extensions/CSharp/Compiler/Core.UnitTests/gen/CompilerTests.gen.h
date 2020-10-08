#pragma once
#include "CompilerTests.h"

TestState RunCompilerTests() 
 {
	auto className = "CompilerTests";
	auto testClass = std::make_shared<Soup::CSharp::Compiler::UnitTests::CompilerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Compile_Library_Simple", [&testClass]() { testClass->Compile_Library_Simple(); });
	state += Soup::Test::RunTest(className, "Compile_Executable_Simple", [&testClass]() { testClass->Compile_Executable_Simple(); });

	return state;
}