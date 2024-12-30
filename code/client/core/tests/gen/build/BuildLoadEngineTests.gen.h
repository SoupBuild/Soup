#pragma once
#include "build/BuildLoadEngineTests.h"

TestState RunBuildLoadEngineTests() 
{
	auto className = "BuildLoadEngineTests";
	auto testClass = std::make_shared<Soup::Core::UnitTests::BuildLoadEngineTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "Initialize", [&testClass]() { testClass->Initialize(); });
	state += Soup::Test::RunTest(className, "Load_MissingPackageLock_Throws", [&testClass]() { testClass->Load_MissingPackageLock_Throws(); });
	state += Soup::Test::RunTest(className, "Load_LanguageExtension_BuiltIn_ToolDependency_BuiltIn", [&testClass]() { testClass->Load_LanguageExtension_BuiltIn_ToolDependency_BuiltIn(); });
	state += Soup::Test::RunTest(className, "Load_LanguageExtension_External", [&testClass]() { testClass->Load_LanguageExtension_External(); });
	state += Soup::Test::RunTest(className, "Load_LanguageExtension_External_ToolDependency_External", [&testClass]() { testClass->Load_LanguageExtension_External_ToolDependency_External(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_External", [&testClass]() { testClass->Load_BuildDependency_External(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_External_ToolDependency_External", [&testClass]() { testClass->Load_BuildDependency_External_ToolDependency_External(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_Local", [&testClass]() { testClass->Load_BuildDependency_Local(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_External_ImplicitOwner_Fails", [&testClass]() { testClass->Load_BuildDependency_External_ImplicitOwner_Fails(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_External_RuntimeDependency_ImplicitOwner", [&testClass]() { testClass->Load_BuildDependency_External_RuntimeDependency_ImplicitOwner(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_External_BuildDependency_ImplicitOwner", [&testClass]() { testClass->Load_BuildDependency_External_BuildDependency_ImplicitOwner(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_External_ToolDependency_ImplicitOwner", [&testClass]() { testClass->Load_BuildDependency_External_ToolDependency_ImplicitOwner(); });
	state += Soup::Test::RunTest(className, "Load_BuildDependency_External_Override", [&testClass]() { testClass->Load_BuildDependency_External_Override(); });
	state += Soup::Test::RunTest(className, "Load_RuntimeDependency_Local_ExplicitLanguageOverride_MultipleReferences_ReuseBuildGraph", [&testClass]() { testClass->Load_RuntimeDependency_Local_ExplicitLanguageOverride_MultipleReferences_ReuseBuildGraph(); });
	state += Soup::Test::RunTest(className, "Load_RuntimeDependency_External_Triangle_BuildOnce", [&testClass]() { testClass->Load_RuntimeDependency_External_Triangle_BuildOnce(); });
	state += Soup::Test::RunTest(className, "Load_OtherDependency_ExplicitLanguage", [&testClass]() { testClass->Load_OtherDependency_ExplicitLanguage(); });

	return state;
}