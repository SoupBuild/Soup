#pragma once
#include "Package/PackageReferenceTests.h"

TestState RunPackageReferenceTests() 
 {
	auto className = "PackageReferenceTests";
	auto testClass = std::make_shared<Soup::UnitTests::PackageReferenceTests>();
	TestState state = { 0, 0 };
	state += SoupTest::RunTest(className, "InitializeNameVersion", [&testClass]() { testClass->InitializeNameVersion(); });
	state += SoupTest::RunTest(className, "InitializePath", [&testClass]() { testClass->InitializePath(); });
	state += SoupTest::RunTest(className, "OperatorEqualNameVersion", [&testClass]() { testClass->OperatorEqualNameVersion(); });
	state += SoupTest::RunTest(className, "OperatorEqualPath", [&testClass]() { testClass->OperatorEqualPath(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
	state += SoupTest::RunTest(className, "OperatorNotEqualPath", [&testClass]() { testClass->OperatorNotEqualPath(); });
	state += SoupTest::RunTest(className, "ParseNameVersionValues(\"Name@1.2.3\", \"Name\", SemanticVersion(1, 2, 3))", [&testClass]() { testClass->ParseNameVersionValues("Name@1.2.3", "Name", SemanticVersion(1, 2, 3)); });
	state += SoupTest::RunTest(className, "ParsePathValues(\"../Path\")", [&testClass]() { testClass->ParsePathValues("../Path"); });
	state += SoupTest::RunTest(className, "TryParseValues(\"Package@1.2.3\", true)", [&testClass]() { testClass->TryParseValues("Package@1.2.3", true); });
	state += SoupTest::RunTest(className, "TryParseValues(\"Package@2\", false)", [&testClass]() { testClass->TryParseValues("Package@2", false); });
	state += SoupTest::RunTest(className, "ToStringNameVersionValues(\"Name\", SemanticVersion(1, 2, 3), \"Name@1.2.3\")", [&testClass]() { testClass->ToStringNameVersionValues("Name", SemanticVersion(1, 2, 3), "Name@1.2.3"); });
	state += SoupTest::RunTest(className, "ToStringPathValues(\"../Path\")", [&testClass]() { testClass->ToStringPathValues("../Path"); });

	return state;
}