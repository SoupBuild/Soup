#pragma once
#include "BuildGenerateEngineTests.h"

TestState RunBuildGenerateEngineTests() 
 {
	auto className = "BuildGenerateEngineTests";
	auto testClass = std::make_shared<Soup::Build::Runtime::UnitTests::BuildGenerateEngineTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Generate_Simple", [&testClass]() { testClass->Generate_Simple(); });
	state += Soup::Test::RunTest(className, "Generate_SingleRuntimeDependency", [&testClass]() { testClass->Generate_SingleRuntimeDependency(); });

	return state;
}