#pragma once
#include "/LinkerArgumentBuilderTests.h"

TestState RunLinkerArgumentBuilderTests() 
 {
	auto className = "LinkerArgumentBuilderTests";
	auto testClass = std::make_shared<Soup::Compiler::MSVC::UnitTests::LinkerArgumentBuilderTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "ZeroObjectFiles", [&testClass]() { testClass->ZeroObjectFiles(); });
	state += SoupTest::RunTest(className, "EmptyTargetFile_Throws", [&testClass]() { testClass->EmptyTargetFile_Throws(); });
	state += SoupTest::RunTest(className, "StaticLibrary", [&testClass]() { testClass->StaticLibrary(); });
	state += SoupTest::RunTest(className, "StaticLibrary_LibraryPaths", [&testClass]() { testClass->StaticLibrary_LibraryPaths(); });
	state += SoupTest::RunTest(className, "DynamicLibrary", [&testClass]() { testClass->DynamicLibrary(); });
	state += SoupTest::RunTest(className, "Executable", [&testClass]() { testClass->Executable(); });

	return state;
}