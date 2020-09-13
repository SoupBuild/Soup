#pragma once
#include "Package/PackageReferenceTests.h"

TestState RunPackageReferenceTests() 
 {
	auto className = "PackageReferenceTests";
	auto testClass = std::make_shared<Soup::UnitTests::PackageReferenceTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "InitializeNameVersion", [&testClass]() { testClass->InitializeNameVersion(); });
	state += Soup::Test::RunTest(className, "InitializePath", [&testClass]() { testClass->InitializePath(); });
	state += Soup::Test::RunTest(className, "OperatorEqualNameVersion", [&testClass]() { testClass->OperatorEqualNameVersion(); });
	state += Soup::Test::RunTest(className, "OperatorEqualPath", [&testClass]() { testClass->OperatorEqualPath(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualPath", [&testClass]() { testClass->OperatorNotEqualPath(); });
	state += Soup::Test::RunTest(className, "ParseNameVersionValues(\"Name@1.2.3\", \"Name\", SemanticVersion(1, 2, 3))", [&testClass]() { testClass->ParseNameVersionValues("Name@1.2.3", "Name", SemanticVersion(1, 2, 3)); });
	state += Soup::Test::RunTest(className, "ParsePathValues(\"../Path\")", [&testClass]() { testClass->ParsePathValues("../Path"); });
	state += Soup::Test::RunTest(className, "TryParseValues(\"Package@1.2.3\", true)", [&testClass]() { testClass->TryParseValues("Package@1.2.3", true); });
	state += Soup::Test::RunTest(className, "TryParseValues(\"Package@2\", false)", [&testClass]() { testClass->TryParseValues("Package@2", false); });
	state += Soup::Test::RunTest(className, "ToStringNameVersionValues(\"Name\", SemanticVersion(1, 2, 3), \"Name@1.2.3\")", [&testClass]() { testClass->ToStringNameVersionValues("Name", SemanticVersion(1, 2, 3), "Name@1.2.3"); });
	state += Soup::Test::RunTest(className, "ToStringPathValues(\"../Path\")", [&testClass]() { testClass->ToStringPathValues("../Path"); });

	return state;
}