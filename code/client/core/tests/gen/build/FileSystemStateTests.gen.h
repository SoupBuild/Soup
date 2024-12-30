#pragma once
#include "build/FileSystemStateTests.h"

TestState RunFileSystemStateTests() 
 {
	auto className = "FileSystemStateTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::FileSystemStateTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize_Default", [&testClass]() { testClass->Initialize_Default(); });
	state += Soup::Test::RunTest(className, "Initialize_ListOperations_Single", [&testClass]() { testClass->Initialize_ListOperations_Single(); });
	state += Soup::Test::RunTest(className, "GetFilePath_MissingThrows", [&testClass]() { testClass->GetFilePath_MissingThrows(); });
	state += Soup::Test::RunTest(className, "GetFilePath_Found", [&testClass]() { testClass->GetFilePath_Found(); });
	state += Soup::Test::RunTest(className, "GetLastWriteTime_Missing", [&testClass]() { testClass->GetLastWriteTime_Missing(); });
	state += Soup::Test::RunTest(className, "GetLastWriteTime_Found", [&testClass]() { testClass->GetLastWriteTime_Found(); });
	state += Soup::Test::RunTest(className, "TryFindFileId_Missing", [&testClass]() { testClass->TryFindFileId_Missing(); });
	state += Soup::Test::RunTest(className, "TryFindFileId_Found", [&testClass]() { testClass->TryFindFileId_Found(); });
	state += Soup::Test::RunTest(className, "ToFileId_Existing", [&testClass]() { testClass->ToFileId_Existing(); });
	state += Soup::Test::RunTest(className, "ToFileId_Unknown", [&testClass]() { testClass->ToFileId_Unknown(); });

	return state;
}