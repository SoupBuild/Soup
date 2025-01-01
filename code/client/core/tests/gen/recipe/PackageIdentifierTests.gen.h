#pragma once
#include "recipe/PackageIdentifierTests.h"

TestState RunPackageIdentifierTests() 
 {
	auto className = "PackageIdentifierTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::PackageIdentifierTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "OperatorEqual", [&testClass]() { testClass->OperatorEqual(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualLanguage", [&testClass]() { testClass->OperatorNotEqualLanguage(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualOwner", [&testClass]() { testClass->OperatorNotEqualOwner(); });
	state += Soup::Test::RunTest(className, "OperatorNotEqualName", [&testClass]() { testClass->OperatorNotEqualName(); });
	state += Soup::Test::RunTest(className, "ParseValues(\"Name\", std::nullopt, std::nullopt, \"Name\")", [&testClass]() { testClass->ParseValues("Name", std::nullopt, std::nullopt, "Name"); });
	state += Soup::Test::RunTest(className, "ParseValues(\"[C#]Name\", \"C#\", std::nullopt, \"Name\")", [&testClass]() { testClass->ParseValues("[C#]Name", "C#", std::nullopt, "Name"); });
	state += Soup::Test::RunTest(className, "ParseValues(\"User1|Name\", std::nullopt, \"User1\", \"Name\")", [&testClass]() { testClass->ParseValues("User1|Name", std::nullopt, "User1", "Name"); });
	state += Soup::Test::RunTest(className, "ParseValues(\"[C#]User1|Name\", \"C#\", \"User1\", \"Name\")", [&testClass]() { testClass->ParseValues("[C#]User1|Name", "C#", "User1", "Name"); });
	state += Soup::Test::RunTest(className, "TryParseValues(\"Package@1.2.3\", true)", [&testClass]() { testClass->TryParseValues("Package", true); });
	state += Soup::Test::RunTest(className, "ToStringValues(std::nullopt, std::nullopt, \"Name\", \"Name\")", [&testClass]() { testClass->ToStringValues(std::nullopt, std::nullopt, "Name", "Name"); });
	state += Soup::Test::RunTest(className, "ToStringValues(\"C#\", std::nullopt, \"Name\", \"[C#]Name\")", [&testClass]() { testClass->ToStringValues("C#", std::nullopt, "Name", "[C#]Name"); });
	state += Soup::Test::RunTest(className, "ToStringValues(std::nullopt, \"User1\", \"Name\", \"User1|Name\")", [&testClass]() { testClass->ToStringValues(std::nullopt, "User1", "Name", "User1|Name"); });
	state += Soup::Test::RunTest(className, "ToStringValues(\"C#\", \"User1\", \"Name\", \"[C#]User1|Name\")", [&testClass]() { testClass->ToStringValues("C#", "User1", "Name", "[C#]User1|Name"); });

	return state;
}