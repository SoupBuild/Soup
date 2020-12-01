#pragma once
#include "OperationGraph/OperationGraphReaderTests.h"

TestState RunOperationGraphReaderTests() 
{
	auto className = "OperationGraphReaderTests";
	auto testClass = std::make_shared<Soup::Build::Runtime::UnitTests::OperationGraphReaderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidFileHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileVersionThrows", [&testClass]() { testClass->Deserialize_InvalidFileVersionThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFilesHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidFilesHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidOperationsHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidOperationsHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_ExtraContentThrows", [&testClass]() { testClass->Deserialize_ExtraContentThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_Empty", [&testClass]() { testClass->Deserialize_Empty(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleSimple", [&testClass]() { testClass->Deserialize_SingleSimple(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleComplex", [&testClass]() { testClass->Deserialize_SingleComplex(); });
	state += Soup::Test::RunTest(className, "Deserialize_Multiple", [&testClass]() { testClass->Deserialize_Multiple(); });

	return state;
}