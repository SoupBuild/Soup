#pragma once
#include "CompilerArgumentBuilderTests.h"

TestState RunCompilerArgumentBuilderTests() 
 {
	auto className = "CompilerArgumentBuilderTests";
	auto testClass = std::make_shared<Soup::Cpp::Compiler::MSVC::UnitTests::CompilerArgumentBuilderTests>();
	TestState state = { 0, 0 };
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_DefaultParameters", [&testClass]() { testClass->BuildSharedCompilerArguments_DefaultParameters(); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_LanguageStandard(Soup::Cpp::LanguageStandard::CPP11, \"/std:c++11\")", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_LanguageStandard(Soup::Cpp::Compiler::LanguageStandard::CPP11, "/std:c++11"); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_LanguageStandard(Soup::Cpp::LanguageStandard::CPP14, \"/std:c++14\")", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_LanguageStandard(Soup::Cpp::Compiler::LanguageStandard::CPP14, "/std:c++14"); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_LanguageStandard(Soup::Cpp::LanguageStandard::CPP17, \"/std:c++17\")", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_LanguageStandard(Soup::Cpp::Compiler::LanguageStandard::CPP17, "/std:c++17"); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_LanguageStandard_CPP20", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_LanguageStandard_CPP20(); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_OptimizationLevel_Disabled", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_OptimizationLevel_Disabled(); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_OptimizationLevel(Soup::Cpp::OptimizationLevel::Size, \"/Os\")", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_OptimizationLevel(Soup::Cpp::Compiler::OptimizationLevel::Size, "/Os"); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_OptimizationLevel(Soup::Cpp::OptimizationLevel::Speed, \"/Ot\")", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_OptimizationLevel(Soup::Cpp::Compiler::OptimizationLevel::Speed, "/Ot"); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_GenerateDebugInformation", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_GenerateDebugInformation(); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_IncludePaths", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_IncludePaths(); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_PreprocessorDefinitions", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_PreprocessorDefinitions(); });
	state += Soup::Test::RunTest(className, "BuildSharedCompilerArguments_SingleArgument_Modules", [&testClass]() { testClass->BuildSharedCompilerArguments_SingleArgument_Modules(); });
	state += Soup::Test::RunTest(className, "BuildInterfaceUnitCompilerArguments", [&testClass]() { testClass->BuildInterfaceUnitCompilerArguments(); });
	state += Soup::Test::RunTest(className, "BuildTranslationUnitCompilerArguments_Simple", [&testClass]() { testClass->BuildTranslationUnitCompilerArguments_Simple(); });
	state += Soup::Test::RunTest(className, "BuildTranslationUnitCompilerArguments_InternalModules", [&testClass]() { testClass->BuildTranslationUnitCompilerArguments_InternalModules(); });

	return state;
}