// <copyright file="CompilerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::MSVC::UnitTests
{
	class CompilerArgumentBuilderTests
	{
	public:
		// [[Fact]]
		void BuildSharedCompilerArguments_DefaultParameters()
		{
			SharedCompileArguments arguments = {};

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Theory]]
		// [[InlineData(Soup::Compiler::LanguageStandard::CPP11, "/std:c++11")]]
		// [[InlineData(Soup::Compiler::LanguageStandard::CPP14, "/std:c++14")]]
		// [[InlineData(Soup::Compiler::LanguageStandard::CPP17, "/clang:-std=c++17")]]
		void BuildSharedCompilerArguments_SingleArgument_LanguageStandard(LanguageStandard standard, std::string expectedFlag)
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = standard;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				expectedFlag,
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildSharedCompilerArguments_SingleArgument_LanguageStandard_CPP20()
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = LanguageStandard::CPP20;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++latest",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildSharedCompilerArguments_SingleArgument_OptimizationLevel_Disabled()
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = OptimizationLevel::None;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++17",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Theory]]
		// [[InlineData(Soup::OptimizationLevel::Size, "/Os")]]
		// [[InlineData(Soup::OptimizationLevel::Speed, "/Ot")]]
		void BuildSharedCompilerArguments_SingleArgument_OptimizationLevel(OptimizationLevel level, std::string expectedFlag)
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = level;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++17",
				expectedFlag,
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildSharedCompilerArguments_SingleArgument_EnableWarningsAsErrors()
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = OptimizationLevel::None;
			arguments.EnableWarningsAsErrors = true;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/WX",
				"/W4",
				"/std:c++17",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildSharedCompilerArguments_SingleArgument_GenerateDebugInformation()
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = OptimizationLevel::None;
			arguments.GenerateSourceDebugInfo = true;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/Z7",
				"/W4",
				"/std:c++17",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MTd",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildSharedCompilerArguments_SingleArgument_IncludePaths()
		{
			SharedCompileArguments arguments = {};
			arguments.IncludeDirectories = std::vector<Path>({
				Path("C:/Files/SDK/"),
				Path("my files/")
			});

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/I\"C:/Files/SDK/\"",
				"/I\"./my files/\"",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildSharedCompilerArguments_SingleArgument_PreprocessorDefinitions()
		{
			SharedCompileArguments arguments = {};
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"DEBUG",
				"VERSION=1"
			});

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/DDEBUG",
				"/DVERSION=1",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildSharedCompilerArguments_SingleArgument_Modules()
		{
			SharedCompileArguments arguments = {};
			arguments.IncludeModules = std::vector<Path>({
				Path("Module.pcm"),
				Path("Std.pcm"),
			});

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/reference",
				"\"./Module.pcm\"",
				"/reference",
				"\"./Std.pcm\"",
				"/bigobj",
				"/c",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildInterfaceUnitCompilerArguments()
		{
			InterfaceUnitCompileArguments arguments = {};
			arguments.SourceFile = Path("module.cpp");
			arguments.TargetFile = Path("module.obj");
			arguments.ModuleInterfaceTarget = Path("module.ifc");

			auto responseFile = Path("ResponseFile.txt");

			auto actualArguments = ArgumentBuilder::BuildInterfaceUnitCompilerArguments(
				arguments,
				responseFile);

			auto expectedArguments = std::vector<std::string>({
				"@./ResponseFile.txt",
				"./module.cpp",
				"/Fo\"./module.obj\"",
				"/interface",
				"/ifcOutput",
				"\"./module.ifc\"",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildTranslationUnitCompilerArguments_Simple()
		{
			TranslationUnitCompileArguments arguments = {};
			arguments.SourceFile = Path("module.cpp");
			arguments.TargetFile = Path("module.obj");

			auto responseFile = Path("ResponseFile.txt");
			auto internalModules = std::vector<Path>();

			auto actualArguments = ArgumentBuilder::BuildTranslationUnitCompilerArguments(
				arguments,
				responseFile,
				internalModules);

			auto expectedArguments = std::vector<std::string>({
				"@./ResponseFile.txt",
				"./module.cpp",
				"/Fo\"./module.obj\"",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildTranslationUnitCompilerArguments_InternalModules()
		{
			TranslationUnitCompileArguments arguments = {};
			arguments.SourceFile = Path("module.cpp");
			arguments.TargetFile = Path("module.obj");

			auto responseFile = Path("ResponseFile.txt");
			auto internalModules = std::vector<Path>({
				Path("Module1.ifc"),
				Path("Module2.ifc"),
			});

			auto actualArguments = ArgumentBuilder::BuildTranslationUnitCompilerArguments(
				arguments,
				responseFile,
				internalModules);

			auto expectedArguments = std::vector<std::string>({
				"@./ResponseFile.txt",
				"/reference",
				"\"./Module1.ifc\"",
				"/reference",
				"\"./Module2.ifc\"",
				"./module.cpp",
				"/Fo\"./module.obj\"",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}
	};
}
