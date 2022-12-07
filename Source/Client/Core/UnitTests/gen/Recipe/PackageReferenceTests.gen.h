#pragma once
#include "Recipe/PackageReferenceTests.h"

TestState RunPackageReferenceTests() 
 {
	auto className = "PackageReferenceTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::PackageReferenceTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "InitializeNamed", [&testClass]() { testClass->InitializeNamed(); });
	state += Soup::Test::RunTest(className, "InitializePath", [&testClass]() { testClass->InitializePath(); });
	state += Soup::Test::RunTest(className, "OperatorEqualNameVersion", [&testClass]() { testClass->OperatorEqualNameVersion(); });
	state += Soup::Test::RunTest(className, "OperatorEqualPath", [&testClass]() { testClass->OperatorEqualPath(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualLanguage", [&testClass]() { testClass->OperatorNotEqualLanguage(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualVersion", [&testClass]() { testClass->OperatorNotEqualVersion(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualPath", [&testClass]() { testClass->OperatorNotEqualPath(); });
	state += Soup::Test::RunTest(className, "ParseNamedValues(\"Name\", std::nullopt, \"Name\", std::nullopt)", [&testClass]() { testClass->ParseNamedValues("Name", std::nullopt, "Name", std::nullopt); });
	state += Soup::Test::RunTest(className, "ParseNamedValues(\"Name@1.2.3\", std::nullopt, \"Name\", SemanticVersion(1, 2, 3))", [&testClass]() { testClass->ParseNamedValues("Name@1.2.3", std::nullopt, "Name", SemanticVersion(1, 2, 3)); });
	state += Soup::Test::RunTest(className, "ParseNamedValues(\"C#|Name\", \"C#\", \"Name\", std::nullopt)", [&testClass]() { testClass->ParseNamedValues("C#|Name", "C#", "Name", std::nullopt); });
	state += Soup::Test::RunTest(className, "ParseNamedValues(\"C#|Name@1.2.3\", \"C#\", \"Name\", SemanticVersion(1, 2, 3))", [&testClass]() { testClass->ParseNamedValues("C#|Name@1.2.3", "C#", "Name", SemanticVersion(1, 2, 3)); });
	state += Soup::Test::RunTest(className, "ParsePathValues(\"../Path\")", [&testClass]() { testClass->ParsePathValues("../Path"); });
	state += Soup::Test::RunTest(className, "TryParseValues(\"Package@1.2.3\", true)", [&testClass]() { testClass->TryParseValues("Package@1.2.3", true); });
	state += Soup::Test::RunTest(className, "TryParseValues(\"Package@2\", true)", [&testClass]() { testClass->TryParseValues("Package@2", true); });
	state += Soup::Test::RunTest(className, "ToStringNamedValues(std::nullopt, \"Name\", std::nullopt, \"Name\")", [&testClass]() { testClass->ToStringNamedValues(std::nullopt, "Name", std::nullopt, "Name"); });
	state += Soup::Test::RunTest(className, "ToStringNamedValues(std::nullopt, \"Name\", SemanticVersion(1, 2, 3), \"Name@1.2.3\")", [&testClass]() { testClass->ToStringNamedValues(std::nullopt, "Name", SemanticVersion(1, 2, 3), "Name@1.2.3"); });
	state += Soup::Test::RunTest(className, "ToStringNamedValues(\"C#\", \"Name\", std::nullopt, \"C#|Name\")", [&testClass]() { testClass->ToStringNamedValues("C#", "Name", std::nullopt, "C#|Name"); });
	state += Soup::Test::RunTest(className, "ToStringNamedValues(\"C#\", \"Name\", SemanticVersion(1, 2, 3), \"C#|Name@1.2.3\")", [&testClass]() { testClass->ToStringNamedValues("C#", "Name", SemanticVersion(1, 2, 3), "C#|Name@1.2.3"); });
	state += Soup::Test::RunTest(className, "ToStringPathValues(\"../Path\")", [&testClass]() { testClass->ToStringPathValues("../Path"); });

	return state;
}