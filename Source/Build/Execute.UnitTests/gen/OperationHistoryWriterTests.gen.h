#pragma once
#include "OperationHistoryWriterTests.h"

TestState RunOperationHistoryWriterTests() 
{
	auto className = "OperationHistoryWriterTests";
	auto testClass = std::make_shared<Soup::Build::Execute::UnitTests::OperationHistoryWriterTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Serialize_Empty", [&testClass]() { testClass->Serialize_Empty(); });
	state += Soup::Test::RunTest(className, "Serialize_Single", [&testClass]() { testClass->Serialize_Single(); });
	state += Soup::Test::RunTest(className, "Serialize_Multiple", [&testClass]() { testClass->Serialize_Multiple(); });

	return state;
}