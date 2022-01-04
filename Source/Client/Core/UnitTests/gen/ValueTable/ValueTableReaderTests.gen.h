#pragma once
#include "ValueTable/ValueTableReaderTests.h"

TestState RunValueTableReaderTests() 
{
	auto className = "ValueTableReaderTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::ValueTableReaderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidFileHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileVersionThrows", [&testClass]() { testClass->Deserialize_InvalidFileVersionThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidTableHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidTableHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidValueTypeThrows", [&testClass]() { testClass->Deserialize_InvalidValueTypeThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_ExtraContentThrows", [&testClass]() { testClass->Deserialize_ExtraContentThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_Empty", [&testClass]() { testClass->Deserialize_Empty(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleTable", [&testClass]() { testClass->Deserialize_SingleTable(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleList", [&testClass]() { testClass->Deserialize_SingleList(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleString", [&testClass]() { testClass->Deserialize_SingleString(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleInteger", [&testClass]() { testClass->Deserialize_SingleInteger(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleFloat", [&testClass]() { testClass->Deserialize_SingleFloat(); });
	state += Soup::Test::RunTest(className, "Deserialize_SingleBoolean", [&testClass]() { testClass->Deserialize_SingleBoolean(); });

	return state;
}