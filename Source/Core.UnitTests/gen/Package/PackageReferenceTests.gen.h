#pragma once
#include "/Package/PackageReferenceTests.h"

TestState RunPackageReferenceTests() 
 {
    auto className = "PackageReferenceTests";
    auto testClass = std::make_shared<Soup::UnitTests::PackageReferenceTests>();
    TestState state = { 0, 0 };
    state += RunTest(className, "InitialzeNameVersion", [&testClass]() { testClass->InitialzeNameVersion(); });
    state += RunTest(className, "InitialzePath", [&testClass]() { testClass->InitialzePath(); });
    state += RunTest(className, "OperatorEqualNameVersion", [&testClass]() { testClass->OperatorEqualNameVersion(); });
    state += RunTest(className, "OperatorEqualPath", [&testClass]() { testClass->OperatorEqualPath(); });
    state += RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
    state += RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
    state += RunTest(className, "OperatorNotEqualPath", [&testClass]() { testClass->OperatorNotEqualPath(); });
    state += RunTest(className, "ParseNameVersionValues(\"Name@1.2.3\", \"Name\", Soup::SemanticVersion(1, 2, 3))", [&testClass]() { testClass->ParseNameVersionValues("Name@1.2.3", "Name", Soup::SemanticVersion(1, 2, 3)); });
    state += RunTest(className, "ParsePathValues(\"../Path\")", [&testClass]() { testClass->ParsePathValues("../Path"); });
    state += RunTest(className, "TryParseValues(\"Package@1.2.3\", true)", [&testClass]() { testClass->TryParseValues("Package@1.2.3", true); });
    state += RunTest(className, "TryParseValues(\"Package@2\", false)", [&testClass]() { testClass->TryParseValues("Package@2", false); });
    state += RunTest(className, "ToStringNameVersionValues(\"Name\", Soup::SemanticVersion(1, 2, 3), \"Name@1.2.3\")", [&testClass]() { testClass->ToStringNameVersionValues("Name", Soup::SemanticVersion(1, 2, 3), "Name@1.2.3"); });
    state += RunTest(className, "ToStringPathValues(\"../Path\")", [&testClass]() { testClass->ToStringPathValues("../Path"); });

    return state;
}