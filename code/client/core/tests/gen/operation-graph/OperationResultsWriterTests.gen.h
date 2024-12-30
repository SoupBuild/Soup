#pragma once
#include "operation-graph/OperationResultsWriterTests.h"

TestState RunOperationResultsWriterTests() 
{
	auto className = "OperationResultsWriterTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::OperationResultsWriterTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Serialize_Empty", [&testClass]() { testClass->Serialize_Empty(); });
	state += Soup::Test::RunTest(className, "Serialize_SingleSimple", [&testClass]() { testClass->Serialize_SingleSimple(); });
	state += Soup::Test::RunTest(className, "Serialize_SingleComplex", [&testClass]() { testClass->Serialize_SingleComplex(); });
	state += Soup::Test::RunTest(className, "Serialize_Multiple", [&testClass]() { testClass->Serialize_Multiple(); });

	return state;
}