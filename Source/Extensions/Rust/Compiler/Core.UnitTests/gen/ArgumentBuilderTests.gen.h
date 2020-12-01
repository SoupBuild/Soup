#pragma once
#include "ArgumentBuilderTests.h"

TestState RunArgumentBuilderTests() 
 {
	auto className = "ArgumentBuilderTests";
	auto testClass = std::make_shared<Soup::Rust::Compiler::UnitTests::ArgumentBuilderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "DefaultParameters", [&testClass]() { testClass->DefaultParameters(); });
	state += Soup::Test::RunTest(className, "SingleArgument_OptimizationLevel_Disabled", [&testClass]() { testClass->SingleArgument_OptimizationLevel_Disabled(); });
	state += Soup::Test::RunTest(className, "SingleArgument_OptimizationLevel_Enabled", [&testClass]() { testClass->SingleArgument_OptimizationLevel_Enabled(); });
	state += Soup::Test::RunTest(className, "SingleArgument_GenerateDebugInformation", [&testClass]() { testClass->SingleArgument_GenerateDebugInformation(); });
	state += Soup::Test::RunTest(className, "SingleArgument_PreprocessorDefinitions", [&testClass]() { testClass->SingleArgument_PreprocessorDefinitions(); });

	return state;
}