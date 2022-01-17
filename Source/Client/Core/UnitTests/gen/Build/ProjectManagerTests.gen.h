#pragma once
#include "Build/ProjectManagerTests.h"

TestState RunProjectManagerTests() 
{
	auto className = "ProjectManagerTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::ProjectManagerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

	return state;
}