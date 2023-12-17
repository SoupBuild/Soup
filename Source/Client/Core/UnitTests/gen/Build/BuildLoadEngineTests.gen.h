#pragma once
#include "Build/BuildLoadEngineTests.h"

TestState RunBuildLoadEngineTests() 
{
	auto className = "BuildLoadEngineTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::BuildLoadEngineTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Load_LanguageExtension_BuiltInVersion", [&testClass]() { testClass->Load_LanguageExtension_BuiltInVersion(); });
	state += Soup::Test::RunTest(className, "Load_LanguageExtension_BuiltInVersion_ToolDependency", [&testClass]() { testClass->Load_LanguageExtension_BuiltInVersion_ToolDependency(); });
	state += Soup::Test::RunTest(className, "Load_LanguageExtension_ExternalLanguage_ExactMatch", [&testClass]() { testClass->Load_LanguageExtension_ExternalLanguage_ExactMatch(); });
	state += Soup::Test::RunTest(className, "Load_LanguageExtension_PackageLock_ToolDependency", [&testClass]() { testClass->Load_LanguageExtension_PackageLock_ToolDependency(); });
	state += Soup::Test::RunTest(className, "Load_TriangleDependency_NoRebuild", [&testClass]() { testClass->Load_TriangleDependency_NoRebuild(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_NoPackageLock_ExternalReference", [&testClass]() { testClass->Load_BuildDependency_NoPackageLock_ExternalReference(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_NoPackageLock_ExternalReference_ToolDependency", [&testClass]() { testClass->Load_BuildDependency_NoPackageLock_ExternalReference_ToolDependency(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_NoPackageLock_Local", [&testClass]() { testClass->Load_BuildDependency_NoPackageLock_Local(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_ImplicitOwner_FailsOnLocal", [&testClass]() { testClass->Load_BuildDependency_ImplicitOwner_FailsOnLocal(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_PackageLock", [&testClass]() { testClass->Load_BuildDependency_PackageLock(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_PackageLock_ToolDependency", [&testClass]() { testClass->Load_BuildDependency_PackageLock_ToolDependency(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_PackageLock_ToolDependency_ImplicitDependencyOwner", [&testClass]() { testClass->Load_BuildDependency_PackageLock_ToolDependency_ImplicitDependencyOwner(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_PackageLock_Override", [&testClass]() { testClass->Load_BuildDependency_PackageLock_Override(); });
	state += Soup::Test::RunTest(className, "Load_OtherDependency_PackageLock_ExplicitLanguage", [&testClass]() { testClass->Load_OtherDependency_PackageLock_ExplicitLanguage(); });
	state += Soup::Test::RunTest(className, "Load_RuntimeDependency_PackageLock_ExplicitLanguageOverride_MultipleReferences_ReuseBuildGraph", [&testClass]() { testClass->Load_RuntimeDependency_PackageLock_ExplicitLanguageOverride_MultipleReferences_ReuseBuildGraph(); });

	return state;
}