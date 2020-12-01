#pragma once
#include "Tasks/ResolveToolsTaskTests.h"

TestState RunResolveToolsTaskTests() 
 {
	auto className = "ResolveToolsTaskTests";
	auto testClass = std::make_shared<Soup::Rust::UnitTests::ResolveToolsTaskTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Success", [&testClass]() { testClass->Initialize_Success(); });
	state += Soup::Test::RunTest(className, "Execute", [&testClass]() { testClass->Execute(); });

	return state;
}