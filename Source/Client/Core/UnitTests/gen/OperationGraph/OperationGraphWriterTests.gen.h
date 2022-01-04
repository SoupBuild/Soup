#pragma once
#include "OperationGraph/OperationGraphWriterTests.h"

TestState RunOperationGraphWriterTests() 
{
	auto className = "OperationGraphWriterTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::OperationGraphWriterTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Serialize_Empty", [&testClass]() { testClass->Serialize_Empty(); });
	state += Soup::Test::RunTest(className, "Serialize_SingleSimple", [&testClass]() { testClass->Serialize_SingleSimple(); });
	state += Soup::Test::RunTest(className, "Serialize_SingleComplex", [&testClass]() { testClass->Serialize_SingleComplex(); });
	state += Soup::Test::RunTest(className, "Serialize_Multiple", [&testClass]() { testClass->Serialize_Multiple(); });

	return state;
}