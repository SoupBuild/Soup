#pragma once
#include "Commands/PublishCommandTests.h"

TestState RunPublishCommandTests() 
 {
	auto className = "PublishCommandTests";
	auto testClass = std::make_shared<Soup::Client::UnitTests::PublishCommandTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });

	return state;
}