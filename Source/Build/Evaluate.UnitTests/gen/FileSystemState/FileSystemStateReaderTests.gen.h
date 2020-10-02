#pragma once
#include "FileSystemState/FileSystemStateReaderTests.h"

TestState RunFileSystemStateReaderTests() 
{
	auto className = "FileSystemStateReaderTests";
	auto testClass = std::make_shared<Soup::Build::Evaluate::UnitTests::FileSystemStateReaderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidFileHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidFileVersionThrows", [&testClass]() { testClass->Deserialize_InvalidFileVersionThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_InvalidOperationsHeaderThrows", [&testClass]() { testClass->Deserialize_InvalidOperationsHeaderThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_ExtraContentThrows", [&testClass]() { testClass->Deserialize_ExtraContentThrows(); });
	state += Soup::Test::RunTest(className, "Deserialize_Empty", [&testClass]() { testClass->Deserialize_Empty(); });
	state += Soup::Test::RunTest(className, "Deserialize_Single", [&testClass]() { testClass->Deserialize_Single(); });
	state += Soup::Test::RunTest(className, "Deserialize_Multiple", [&testClass]() { testClass->Deserialize_Multiple(); });

	return state;
}