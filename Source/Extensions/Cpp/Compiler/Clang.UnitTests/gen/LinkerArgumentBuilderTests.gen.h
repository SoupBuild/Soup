#pragma once
#include "LinkerArgumentBuilderTests.h"

TestState RunLinkerArgumentBuilderTests() 
 {
	auto className = "LinkerArgumentBuilderTests";
	auto testClass = std::make_shared<Soup::Compiler::Clang::UnitTests::LinkerArgumentBuilderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "ZeroObjectFiles", [&testClass]() { testClass->ZeroObjectFiles(); });
	state += Soup::Test::RunTest(className, "EmptyTargetFile_Throws", [&testClass]() { testClass->EmptyTargetFile_Throws(); });
	state += Soup::Test::RunTest(className, "StaticLibrary", [&testClass]() { testClass->StaticLibrary(); });
	state += Soup::Test::RunTest(className, "DynamicLibrary", [&testClass]() { testClass->DynamicLibrary(); });
	state += Soup::Test::RunTest(className, "Executable", [&testClass]() { testClass->Executable(); });

	return state;
}