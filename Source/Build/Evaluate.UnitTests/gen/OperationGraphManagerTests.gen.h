#pragma once
#include "OperationGraphManagerTests.h"

TestState RunOperationGraphManagerTests() 
 {
	auto className = "OperationGraphManagerTests";
	auto testClass = std::make_shared<Soup::Build::Evaluate::UnitTests::OperationGraphManagerTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "TryLoadFromFile_MissingFile", [&testClass]() { testClass->TryLoadFromFile_MissingFile(); });
	state += Soup::Test::RunTest(className, "TryLoadFromFile_GarbageFile", [&testClass]() { testClass->TryLoadFromFile_GarbageFile(); });
	state += Soup::Test::RunTest(className, "TryLoadFromFile_WrongStateId", [&testClass]() { testClass->TryLoadFromFile_WrongStateId(); });
	state += Soup::Test::RunTest(className, "TryLoadFromFile_SimpleFile", [&testClass]() { testClass->TryLoadFromFile_SimpleFile(); });
	state += Soup::Test::RunTest(className, "SaveState", [&testClass]() { testClass->SaveState(); });

	return state;
}