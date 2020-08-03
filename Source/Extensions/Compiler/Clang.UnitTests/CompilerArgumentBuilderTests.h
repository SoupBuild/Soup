// <copyright file="CompilerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::Clang::UnitTests
{
	class CompilerArgumentBuilderTests
	{
	public:
		[[Fact]]
		void EmptySourceFile_Throws()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("");
			arguments.TargetFile = Path("File.o");

			Assert::ThrowsRuntimeError([&arguments]() {
				auto actualInput = std::vector<Path>();
				auto actualOutput = std::vector<Path>();
				auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
					arguments,
					actualInput,
					actualOutput);
			});
		}

		[[Fact]]
		void EmptyTargetFile_Throws()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("");

			Assert::ThrowsRuntimeError([&arguments]() {
				auto actualInput = std::vector<Path>();
				auto actualOutput = std::vector<Path>();
				auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
					arguments,
					actualInput,
					actualOutput);
			});
		}

		[[Fact]]
		void DefaultParameters()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Theory]]
		[[InlineData(Soup::LanguageStandard::CPP11, "-std=c++11")]]
		[[InlineData(Soup::LanguageStandard::CPP14, "-std=c++14")]]
		[[InlineData(Soup::LanguageStandard::CPP17, "-std=c++17")]]
		void SingleArgument_LanguageStandard(LanguageStandard standard, std::string expectedFlag)
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.Standard = standard;

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				expectedFlag,
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void SingleArgument_LanguageStandard_CPP20()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.Standard = LanguageStandard::CPP20;

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++2a",
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void SingleArgument_OptimizationLevel_Disabled()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = OptimizationLevel::None;

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++17",
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Theory]]
		[[InlineData(Soup::OptimizationLevel::Size, "-Oz")]]
		[[InlineData(Soup::OptimizationLevel::Speed, "-O3")]]
		void SingleArgument_OptimizationLevel(OptimizationLevel level, std::string expectedFlag)
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = level;

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++17",
				expectedFlag,
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void SingleArgument_GenerateDebugInformation()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = OptimizationLevel::None;
			arguments.GenerateSourceDebugInfo = true;

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-g",
				"-std=c++17",
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void SingleArgument_IncludePaths()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.IncludeDirectories = std::vector<Path>({
				Path("C:/Files/SDK/"),
				Path("my files/")
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"-isystem \"C:/Files/SDK/\"",
				"-I\"my files/\"",
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void SingleArgument_PreprocessorDefinitions()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"DEBUG",
				"VERSION=1"
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"-DDEBUG",
				"-DVERSION=1",
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void SingleArgument_Modules()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.o");
			arguments.IncludeModules = std::vector<Path>({
				Path("Module.pcm"),
				Path("Std.pcm"),
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"-fmodule-file=\"Module.pcm\"",
				"-fmodule-file=\"Std.pcm\"",
				"-c",
				"File.cpp",
				"-o",
				"File.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("Module.pcm"),
				Path("Std.pcm"),
				Path("File.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("File.o"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void SingleArgument_ExportModule_SingleSource()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("module.cpp");
			arguments.TargetFile = Path("module.pcm");
			arguments.ExportModule = true;

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(
				arguments,
				actualInput,
				actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"-nostdinc",
				"-Wno-unknown-attributes",
				"-Xclang",
				"-flto-visibility-public-std",
				"-std=c++11",
				"--precompile",
				"module.cpp",
				"-o",
				"module.pcm",
			});
			auto expectedInput = std::vector<Path>({
				Path("module.cpp"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("module.pcm"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}
	};
}
