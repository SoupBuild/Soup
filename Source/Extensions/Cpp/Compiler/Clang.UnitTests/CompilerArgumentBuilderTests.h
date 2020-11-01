// <copyright file="CompilerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::Clang::UnitTests
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
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Theory]]
		// [[InlineData(Soup::LanguageStandard::CPP11, "-std=c++11")]]
		// [[InlineData(Soup::LanguageStandard::CPP14, "-std=c++14")]]
		// [[InlineData(Soup::LanguageStandard::CPP17, "-std=c++17")]]
		void BuildSharedCompilerArguments_SingleArgument_LanguageStandard(LanguageStandard standard, std::string expectedFlag)
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = standard;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				expectedFlag,
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
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++2a",
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
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++17",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Theory]]
		// [[InlineData(Soup::OptimizationLevel::Size, "-Oz")]]
		// [[InlineData(Soup::OptimizationLevel::Speed, "-O3")]]
		void BuildSharedCompilerArguments_SingleArgument_OptimizationLevel(OptimizationLevel level, std::string expectedFlag)
		{
			SharedCompileArguments arguments = {};
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = level;

			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++17",
				expectedFlag,
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
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-g",
				"-std=c++17",
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
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"-isystem \"C:/Files/SDK/\"",
				"-I\"./my files/\"",
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

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildSharedCompilerArguments(
				arguments);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"-DDEBUG",
				"-DVERSION=1",
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
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"-fmodule-file=\"./Module.pcm\"",
				"-fmodule-file=\"./Std.pcm\"",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildInterfaceUnitCompilerArguments()
		{
			InterfaceUnitCompileArguments arguments = {};
			arguments.SourceFile = Path("module.cpp");
			arguments.TargetFile = Path("module.o");
			arguments.ModuleInterfaceTarget = Path("module.pcm");

			auto responseFile = Path("ResponseFile.txt");

			auto actualArguments = ArgumentBuilder::BuildInterfaceUnitCompilerArguments(
				arguments,
				responseFile);

			auto expectedArguments = std::vector<std::string>({
				"@./ResponseFile.txt",
				"--precompile",
				"./module.cpp",
				"-o",
				"./module.pcm",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void BuildTranslationUnitCompilerArguments()
		{
			TranslationUnitCompileArguments arguments = {};
			arguments.SourceFile = Path("module.cpp");
			arguments.TargetFile = Path("module.o");

			auto responseFile = Path("ResponseFile.txt");

			auto actualArguments = ArgumentBuilder::BuildTranslationUnitCompilerArguments(
				arguments,
				responseFile);

			auto expectedArguments = std::vector<std::string>({
				"@./ResponseFile.txt",
				"-c",
				"./module.cpp",
				"-o",
				"./module.o",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}
	};
}
