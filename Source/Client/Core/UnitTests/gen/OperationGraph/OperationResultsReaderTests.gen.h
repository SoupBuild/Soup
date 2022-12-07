#pragma once
#include "OperationGraph/OperationResultsReaderTests.h"

TestState RunOperationResultsReaderTests() 
{
	auto className = "OperationResultsReaderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::OperationResultsReaderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidFileHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileVersionThrows", [&testClass]() { testClass->Deserialize_InvalidFileVersionThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFilesHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidFilesHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidResultsHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidResultsHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_Empty", [&testClass]() { testClass->Deserialize_Empty(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleSimple", [&testClass]() { testClass->Deserialize_SingleSimple(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleComplex", [&testClass]() { testClass->Deserialize_SingleComplex(); });
	state += Soup::Test::RunTest(className, "Deserialize_Multiple", [&testClass]() { testClass->Deserialize_Multiple(); });

	return state;
}