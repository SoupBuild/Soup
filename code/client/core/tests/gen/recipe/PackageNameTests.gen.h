#pragma once
#include "recipe/PackageNameTests.h"

TestState RunPackageNameTests() 
 {
	auto className = "PackageNameTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::PackageNameTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "OperatorEqual", [&testClass]() { testClass->OperatorEqual(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualOwner", [&testClass]() { testClass->OperatorNotEqualOwner(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += Soup::Test::RunTest(className, "ParseValues(\"Name\", std::nullopt, \"Name\")", [&testClass]() { testClass->ParseValues("Name", std::nullopt, "Name"); });
	state += Soup::Test::RunTest(className, "ParseValues(\"User1|Name\", \"User1\", \"Name\")", [&testClass]() { testClass->ParseValues("User1|Name", "User1", "Name"); });
	state += Soup::Test::RunTest(className, "TryParseValues(\"Package@1.2.3\", true)", [&testClass]() { testClass->TryParseValues("Package", true); });
	state += Soup::Test::RunTest(className, "ToStringValues(std::nullopt, \"Name\", \"Name\")", [&testClass]() { testClass->ToStringValues(std::nullopt, "Name", "Name"); });
	state += Soup::Test::RunTest(className, "ToStringValues(\"User1\", \"Name\", \"User1|Name\")", [&testClass]() { testClass->ToStringValues("User1", "Name", "User1|Name"); });

	return state;
}